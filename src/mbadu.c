/**
 * @file mbadu.c
 * @brief Implementation of Modbus Serial Application Data Unit handling
 * @author Jonas Almås
 */

/*
 * Copyright (c) 2025 Siemens Energy AS
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OR CONDITIONS OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OR CONDITIONS
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
 * EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE) OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authorized representative: Edgar Vorland, SE TI EAD MF&P SUS OMS, Group Manager Electronics
 */

#include "mbadu.h"
#include "mbpdu.h"
#include "mbcrc.h"
#include "utils/endian.h"

/**
 * @brief Add Slave addres and CRC to response ADU
 */
static size_t prep_res(uint8_t slave_addr, uint8_t *res, size_t pdu_size)
{
	size_t res_size;
	uint16_t crc;

	res[0] = slave_addr;
	res_size = 1 + pdu_size;

	crc = mbcrc16(res, res_size);
	u16tole(crc, res+res_size);
	res_size += 2;

	return res_size;
}

extern size_t mbadu_handle_req(
	struct mbinst_s *inst,
	const uint8_t *req,
	size_t req_len,
	uint8_t *res)
{
	uint8_t recv_slave_addr;
	uint16_t recv_crc;
	int crc_err;
	size_t pdu_size;

	if (!inst || !req || !res) return 0;
	if (req_len<MBADU_SIZE_MIN || req_len>MBADU_SIZE_MAX) return 0;

	++inst->state.bus_msg_counter;

	recv_slave_addr = req[0];
	if (recv_slave_addr != inst->serial.slave_addr
			&& recv_slave_addr != MBADU_ADDR_BROADCAST
			&& (!inst->serial.enable_def_resp || recv_slave_addr != MBADU_ADDR_DEFAULT_RESP)) {
		return 0;
	}

	/* CRC is in the last two bytes, little endian */
	recv_crc = letou16(req + req_len - 2);
	crc_err = recv_crc != mbcrc16(req, req_len - 2);

	/* Create receive communication log entry */
	if (crc_err
			|| recv_slave_addr==MBADU_ADDR_BROADCAST
			|| inst->state.is_listen_only) {
		mb_add_comm_event(
			inst,
			MB_COMM_EVENT_IS_RECV
			| (crc_err ? MB_COMM_EVENT_RECV_COMM_ERR : 0)
			| (inst->state.is_listen_only ? MB_COMM_EVENT_RECV_LISTEN_MODE : 0)
			| (recv_slave_addr==MBADU_ADDR_BROADCAST ? MB_COMM_EVENT_RECV_BROADCAST : 0));
	}

	if (crc_err) {
		++inst->state.bus_comm_err_counter;
		return 0;
	}

	pdu_size = mbpdu_handle_req(
		inst,
		req + 1, /* PDU starts after slave address */
		req_len - 3, /* - Slave addres and crc */
		res + 1);

	/* Requests sent to the broadcast address shall never get a response */
	if (pdu_size==0u || recv_slave_addr==MBADU_ADDR_BROADCAST) {
		++inst->state.no_resp_counter;
		return 0;
	}

	return prep_res(recv_slave_addr, res, pdu_size);
}

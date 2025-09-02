/**
 * @file mbinst.c
 * @brief Implementation of Modbus Instance
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

#include "mbinst.h"

extern void mbinst_init(struct mbinst_s *inst)
{
	inst->state.is_listen_only = 0;
	inst->state.status = 0u;
	inst->state.comm_event_counter = 0u;

	inst->state.event_log_write_pos = 0;
	inst->state.event_log_count = 0;

	inst->state.bus_msg_counter = 0u;
	inst->state.bus_comm_err_counter = 0u;
	inst->state.exception_counter = 0u;
	inst->state.msg_counter = 0u;
	inst->state.no_resp_counter = 0u;
	inst->state.nak_counter = 0u;
	inst->state.busy_counter = 0u;
	inst->state.bus_char_overrun_counter = 0u;

	inst->state.ascii_delimiter = '\n';
}

extern void mb_add_comm_event(struct mbinst_s *inst, uint8_t event)
{
	inst->state.event_log[inst->state.event_log_write_pos] = event;
	inst->state.event_log_write_pos = (inst->state.event_log_write_pos + 1) % MB_COMM_EVENT_LOG_LEN;
	if (inst->state.event_log_count < MB_COMM_EVENT_LOG_LEN) {
		++inst->state.event_log_count;
	}
}

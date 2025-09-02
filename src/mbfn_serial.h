/**
 * @file mbfn_serial.h
 * @brief Modbus serial-specific function handlers
 * @author Jonas Almås
 *
 * @details This module implements Modbus function codes specific to serial communication.
 *
 * @see mbinst.h for instance configuration
 * @see mbpdu.h for protocol data unit handling
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

#ifndef MBFN_SERIAL_H_INCLUDED
#define MBFN_SERIAL_H_INCLUDED

#include "mbdef.h"
#include "mbinst.h"
#include "mbpdu.h"

/**
 * @brief Reads the exception status of a Modbus slave device
 *
 * Implements Modbus function code 0x07 (Read Exception Status) which is a serial-only function.
 * This function returns device-specific exception status information as defined by the slave.
 * The exception status is typically used to indicate error conditions, communication problems,
 * or device-specific diagnostic information.
 *
 * @param inst Modbus instance containing configuration and callback
 * @param req Pointer to the request PDU (function code + request data)
 * @param req_len Length of the request PDU (expected to be 1 - just function code)
 * @param res Pointer to the response PDU structure to populate
 *
 * @retval MB_OK Success - exception status read and response prepared
 * @retval MB_DEV_FAIL Invalid parameters or callback not configured
 * @retval MB_ILLEGAL_DATA_VAL Invalid request format (length != 1)
 *
 * @note This function requires inst->serial.read_exception_status_cb to be configured
 * @note This is a serial-only function and not applicable for TCP/IP communication
 * @note The meaning of status bits is device-specific and defined by the implementer
 * @note Request format: [function_code] - No additional data required
 */
extern enum mbstatus_e mbfn_read_exception_status(
	const struct mbinst_s *inst,
	const uint8_t *req,
	size_t req_len,
	struct mbpdu_buf_s *res);

#endif /* MBFN_SERIAL_H_INCLUDED */

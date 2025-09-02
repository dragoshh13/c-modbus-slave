/**
 * @file mbcrc.h
 * @brief Modbus CRC-16 calculation functions
 * @author Jonas Almås
 *
 * @details Fast CRC-16 implementation for Modbus using lookup table.
 * Provides optimized checksum calculation for Modbus RTU serial communications.
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

#ifndef MBCRC_H_INCLUDED
#define MBCRC_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Calculate Modbus CRC-16
 *
 * Fast implementation of CRC-16 for Modbus using a precomputed lookup table.
 * This is significantly faster than the bit-by-bit calculation method.
 *
 * @param data Pointer to data buffer to calculate CRC for
 * @param size Number of bytes in the data buffer
 *
 * @return 16-bit CRC value for Modbus
 */
extern uint16_t mbcrc16(const uint8_t *data, size_t size);

#endif /* MBCRC_H_INCLUDED */

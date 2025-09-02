/**
 * @file endian.h
 * @brief Endian Conversion Utilities - Platform-independent byte order conversion
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

#ifndef UTILS__ENDIAN_H_INCLUDED
#define UTILS__ENDIAN_H_INCLUDED

#include <stdint.h>

/**
 * @brief Converts a big-endian byte array to a 16-bit unsigned integer.
 *
 * @param buf Pointer to a big-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 2 bytes.
 *
 * @return 16-bit unsigned integer representation of the byte array.
 */
extern uint16_t betou16(const uint8_t *buf);

/**
 * @brief Converts a big-endian byte array to a 32-bit unsigned integer.
 *
 * @param buf Pointer to a big-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 4 bytes.
 *
 * @return 32-bit unsigned integer representation of the byte array.
 */
extern uint32_t betou32(const uint8_t *buf);

/**
 * @brief Converts a big-endian byte array to a 64-bit unsigned integer.
 *
 * @param buf Pointer to a big-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 8 bytes.
 *
 * @return 64-bit unsigned integer representation of the byte array.
 */
extern uint64_t betou64(const uint8_t *buf);

/**
 * @brief Converts a big-endian byte array to a 16-bit signed integer.
 *
 * @param buf Pointer to a big-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 2 bytes.
 *
 * @return 16-bit signed integer representation of the byte array.
 */
extern int16_t betoi16(const uint8_t *buf);

/**
 * @brief Converts a big-endian byte array to a 32-bit signed integer.
 *
 * @param buf Pointer to a big-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 4 bytes.
 *
 * @return 32-bit signed integer representation of the byte array.
 */
extern int32_t betoi32(const uint8_t *buf);

/**
 * @brief Converts a big-endian byte array to a 64-bit signed integer.
 *
 * @param buf Pointer to a big-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 8 bytes.
 *
 * @return 64-bit signed integer representation of the byte array.
 */
extern int64_t betoi64(const uint8_t *buf);

/**
 * @brief Converts a big-endian byte array to a 32-bit single-precision float.
 *
 * @param buf Pointer to a big-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 4 bytes.
 *
 * @return 32-bit single-precision float representation of the byte array.
 */
extern float betof32(const uint8_t *buf);

/**
 * @brief Converts a big-endian byte array to a 64-bit double-precision float.
 *
 * @param buf Pointer to a big-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 8 bytes.
 *
 * @return 64-bit double-precision float representation of the byte array.
 */
extern double betof64(const uint8_t *buf);

/**
 * @brief Converts a little-endian byte array to a 16-bit unsigned integer.
 *
 * @param buf Pointer to a little-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 2 bytes.
 *
 * @return 16-bit unsigned integer representation of the byte array.
 */
extern uint16_t letou16(const uint8_t *buf);

/**
 * @brief Converts a little-endian byte array to a 32-bit unsigned integer.
 *
 * @param buf Pointer to a little-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 4 bytes.
 *
 * @return 32-bit unsigned integer representation of the byte array.
 */
extern uint32_t letou32(const uint8_t *buf);

/**
 * @brief Converts a little-endian byte array to a 64-bit unsigned integer.
 *
 * @param buf Pointer to a little-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 8 bytes.
 *
 * @return 64-bit unsigned integer representation of the byte array.
 */
extern uint64_t letou64(const uint8_t *buf);

/**
 * @brief Converts a little-endian byte array to a 16-bit signed integer.
 *
 * @param buf Pointer to a little-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 2 bytes.
 *
 * @return 16-bit signed integer representation of the byte array.
 */
extern int16_t letoi16(const uint8_t *buf);

/**
 * @brief Converts a little-endian byte array to a 32-bit signed integer.
 *
 * @param buf Pointer to a little-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 4 bytes.
 *
 * @return 32-bit signed integer representation of the byte array.
 */
extern int32_t letoi32(const uint8_t *buf);

/**
 * @brief Converts a little-endian byte array to a 64-bit signed integer.
 *
 * @param buf Pointer to a little-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 8 bytes.
 *
 * @return 64-bit signed integer representation of the byte array.
 */
extern int64_t letoi64(const uint8_t *buf);

/**
 * @brief Converts a little-endian byte array to a 32-bit single-precision float.
 *
 * @param buf Pointer to a little-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 4 bytes.
 *
 * @return 32-bit single-precision float representation of the byte array.
 */
extern float letof32(const uint8_t *buf);

/**
 * @brief Converts a little-endian byte array to a 64-bit double-precision float.
 *
 * @param buf Pointer to a little-endian byte array.
 *        Must not be NULL.
 *        Must contain at least 8 bytes.
 *
 * @return 64-bit double-precision float representation of the byte array.
 */
extern double letof64(const uint8_t *buf);

/**
 * @brief Converts a uint16 to its big-endian representation.
 *
 * @param val 16-bit unsigned integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 2 bytes of space.
 */
extern void u16tobe(uint16_t val, uint8_t *dst);

/**
 * @brief Converts a uint32 to its big-endian representation.
 *
 * @param val 32-bit unsigned integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 4 bytes of space.
 */
extern void u32tobe(uint32_t val, uint8_t *dst);

/**
 * @brief Converts a uint64 to its big-endian representation.
 *
 * @param val 64-bit unsigned integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 8 bytes of space.
 */
extern void u64tobe(uint64_t val, uint8_t *dst);

/**
 * @brief Converts a int16 to its big-endian representation.
 *
 * @param val 16-bit signed integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 2 bytes of space.
 */
extern void i16tobe(int16_t val, uint8_t *dst);

/**
 * @brief Converts a int32 to its big-endian representation.
 *
 * @param val 32-bit signed integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 4 bytes of space.
 */
extern void i32tobe(int32_t val, uint8_t *dst);

/**
 * @brief Converts a int64 to its big-endian representation.
 *
 * @param val 64-bit signed integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 8 bytes of space.
 */
extern void i64tobe(int64_t val, uint8_t *dst);

/**
 * @brief Converts a float to its big-endian representation.
 *
 * @param val 32-bit single-precision float value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 4 bytes of space.
 */
extern void f32tobe(float val, uint8_t *dst);

/**
 * @brief Converts a double to its big-endian representation.
 *
 * @param val 64-bit double-precision float value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 8 bytes of space.
 */
extern void f64tobe(double val, uint8_t *dst);

/**
 * @brief Converts a uint16 to its little-endian representation.
 *
 * @param val 16-bit unsigned integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 2 bytes of space.
 */
extern void u16tole(uint16_t val, uint8_t *dst);

/**
 * @brief Converts a uint32 to its little-endian representation.
 *
 * @param val 32-bit unsigned integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 4 bytes of space.
 */
extern void u32tole(uint32_t val, uint8_t *dst);

/**
 * @brief Converts a uint64 to its little-endian representation.
 *
 * @param val 64-bit unsigned integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 8 bytes of space.
 */
extern void u64tole(uint64_t val, uint8_t *dst);

/**
 * @brief Converts a int16 to its little-endian representation.
 *
 * @param val 16-bit signed integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 2 bytes of space.
 */
extern void i16tole(int16_t val, uint8_t *dst);

/**
 * @brief Converts a int32 to its little-endian representation.
 *
 * @param val 32-bit signed integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 4 bytes of space.
 */
extern void i32tole(int32_t val, uint8_t *dst);

/**
 * @brief Converts a int64 to its little-endian representation.
 *
 * @param val 64-bit signed integer value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 8 bytes of space.
 */
extern void i64tole(int64_t val, uint8_t *dst);

/**
 * @brief Converts a float to its little-endian representation.
 *
 * @param val 32-bit single-precision float value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 4 bytes of space.
 */
extern void f32tole(float val, uint8_t *dst);

/**
 * @brief Converts a double to its little-endian representation.
 *
 * @param val 64-bit double-precision float value.
 * @param dst Pointer to the destination byte array.
 *        Must not be NULL.
 *        Must have at least 8 bytes of space.
 */
extern void f64tole(double val, uint8_t *dst);

#endif /* UTILS__ENDIAN_H_INCLUDED */

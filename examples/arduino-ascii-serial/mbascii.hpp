#ifndef MBASCII_HPP_INCLUDED
#define MBASCII_HPP_INCLUDED

/**
 * @brief Initialize Modbus ASCII communication
 *
 * Sets up the serial port with the specified baud rate
 *
 * @param baud Serial communication baud rate (e.g., 9600, 19200, 38400)
 */
extern void mbascii_init(unsigned long baud);

/**
 * @brief Process Modbus ASCII communication
 *
 * Handles incoming serial data, detects frame boundaries, processes
 * Modbus requests, and sends responses. Call this continuously in loop().
 *
 * @note This function is non-blocking and handles timing automatically
 */
extern void mbascii_proc(void);

#endif /* MBASCII_HPP_INCLUDED */

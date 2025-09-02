#ifndef MBRTU_HPP_INCLUDED
#define MBRTU_HPP_INCLUDED

/**
 * @brief Initialize Modbus RTU communication
 *
 * Sets up the serial port with the specified baud rate and
 * calculates timing parameters for RTU frame detection.
 *
 * @param baud Serial communication baud rate (e.g., 9600, 19200, 38400)
 *
 * @note Uses 8 data bits, even parity, 1 stop bit (8E1) as per Modbus standard
 */
extern void mbrtu_init(unsigned long baud);

/**
 * @brief Process Modbus RTU communication
 *
 * Handles incoming serial data, detects frame boundaries, processes
 * Modbus requests, and sends responses. Call this continuously in loop().
 *
 * @note This function is non-blocking and handles timing automatically
 */
extern void mbrtu_proc(void);

#endif /* MBRTU_HPP_INCLUDED */

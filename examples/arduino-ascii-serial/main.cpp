/**
 * @file main.cpp
 * @brief Arduino Modbus ASCII Slave Example
 * @author Jonas Almås
 *
 * This example demonstrates a complete Modbus ASCII slave implementation
 * for Arduino. It includes all four Modbus data types:
 * - Discrete Inputs (read-only bits)
 * - Coils (read/write bits)
 * - Input Registers (read-only 16-bit values)
 * - Holding Registers (read/write 16-bit values)
 */

#include "modbus.hpp"
#include "mbascii.hpp"
#include <Arduino.h>

void setup(void)
{
	pinMode(LED_BUILTIN, OUTPUT);

	modbus_init();
	mbascii_init(MODBUS_BAUD);
}

void loop(void)
{
	mbascii_proc();
}

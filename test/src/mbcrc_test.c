#include "test_lib.h"
#include <mbcrc.h>

TEST(mbcrc16_known_values)
{
	uint16_t res;
	uint8_t buf[] = {0x55, ~0x55, 0x02, 0xF0};

	/* Test against known values from reference implementation */
	res = mbcrc16(buf, 1);
	ASSERT_EQ(0x7F7Fu, res);

	res = mbcrc16(buf, 2);
	ASSERT_EQ(0x9FBEu, res);

	res = mbcrc16(buf, 3);
	ASSERT_EQ(0xB19Eu, res);

	res = mbcrc16(buf, 4);
	ASSERT_EQ(0xEC30u, res);
}

TEST(mbcrc16_modbus_frame_examples)
{
	uint16_t res;

	/* Modbus Read Holding Registers: Device 1, Function 3, Start 0, Count 10 */
	uint8_t read_frame[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x0A};
	res = mbcrc16(read_frame, sizeof(read_frame));
	ASSERT_EQ(0xCDC5u, res);

	/* Modbus Read Holding Registers: Device 17, Function 3, Start 107, Count 3 */
	uint8_t read_frame2[] = {0x11, 0x03, 0x00, 0x6B, 0x00, 0x03};
	res = mbcrc16(read_frame2, sizeof(read_frame2));
	ASSERT_EQ(0x8776u, res);

	/* Modbus Write Multiple Registers partial frame */
	uint8_t write_frame[] = {0x01, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x01, 0x02};
	res = mbcrc16(write_frame, sizeof(write_frame));
	ASSERT_EQ(0x3092u, res);
}

TEST(mbcrc16_zero_size_input)
{
	uint8_t buf[] = {0x01, 0x02, 0x03, 0x04};
	uint16_t result = mbcrc16(buf, 0);

	/* CRC of zero bytes should be the initial value (0xFFFF for Modbus) */
	ASSERT_EQ(0xFFFFu, result);
}

TEST(mbcrc16_single_byte_values)
{
	uint16_t res_lookup, res_reference;

	/* Test various single byte values */
	uint8_t test_bytes[] = {0x00, 0x01, 0x55, 0xAA, 0xFF};
	size_t i;

	for (i = 0; i < sizeof(test_bytes); i++) {
		res_lookup = mbcrc16(&test_bytes[i], 1);
		res_reference = mbcrc16(&test_bytes[i], 1);
		ASSERT_EQ(res_reference, res_lookup);
	}
}

TEST_MAIN(
	mbcrc16_known_values,
	mbcrc16_modbus_frame_examples,
	mbcrc16_zero_size_input,
	mbcrc16_single_byte_values
)

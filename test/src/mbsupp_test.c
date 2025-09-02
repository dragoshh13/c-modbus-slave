#include "test_lib.h"
#include <mbsupp.h>

TEST(mbsupp_break_us_low_baud_rates)
{
	/* At very low baud rates, calculated break time should be larger than minimum */
	uint32_t break_time = mbsupp_break_us(1200);
	ASSERT(break_time > 1750); /* Should be much larger than minimum 1.75ms */

	/* Calculate expected value: (1,000,000 * 11 / 1200) * 3.5 = 32083 */
	ASSERT_EQ(32083uL, break_time);
}

TEST(mbsupp_break_us_standard_baud_rates)
{
	/* 9600 baud: (1,000,000 * 11 / 9600) * 3.5 = 4010 */
	uint32_t break_time_9600 = mbsupp_break_us(9600);
	ASSERT_EQ(4010, break_time_9600);
	ASSERT(break_time_9600 > 1750);

	/* 19200 baud: (1,000,000 * 11 / 19200) * 3.5 = 2005 */
	uint32_t break_time_19200 = mbsupp_break_us(19200);
	ASSERT_EQ(2005uL, break_time_19200);
}

TEST(mbsupp_break_us_high_baud_rates_minimum_enforced)
{
	/* At high baud rates, minimum 1750µs should be enforced */

	/* 38400 baud: calculated = 1002, but minimum 1750 should be returned */
	uint32_t break_time_38400 = mbsupp_break_us(38400);
	ASSERT_EQ(1750, break_time_38400);

	/* 57600 baud: calculated = 668, but minimum 1750 should be returned */
	uint32_t break_time_57600 = mbsupp_break_us(57600);
	ASSERT_EQ(1750, break_time_57600);

	/* 115200 baud: calculated = 334, but minimum 1750 should be returned */
	uint32_t break_time_115200 = mbsupp_break_us(115200);
	ASSERT_EQ(1750, break_time_115200);
}

TEST(mbsupp_break_us_edge_case_baud_1)
{
	/* Test with baud rate of 1 (extreme low value) */
	uint32_t break_time = mbsupp_break_us(1);
	ASSERT_EQ(38500000uL, break_time);
}

TEST(mbsupp_break_us_calculation_precision)
{
	/* Test specific values to verify calculation precision */

	/* 4800 baud: (1,000,000 * 11 / 4800) * 3.5 = 8020 */
	uint32_t break_time_4800 = mbsupp_break_us(4800);
	ASSERT_EQ(8020uL, break_time_4800);

	/* 14400 baud: (1,000,000 * 11 / 14400) * 3.5 = 2673 */
	uint32_t break_time_14400 = mbsupp_break_us(14400);
	ASSERT_EQ(2673uL, break_time_14400);
}

TEST_MAIN(
	mbsupp_break_us_low_baud_rates,
	mbsupp_break_us_standard_baud_rates,
	mbsupp_break_us_high_baud_rates_minimum_enforced,
	mbsupp_break_us_edge_case_baud_1,
	mbsupp_break_us_calculation_precision
);

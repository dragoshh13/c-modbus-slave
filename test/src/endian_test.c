#include "test_lib.h"
#include <endian.h>

TEST(endian_betou16_works)
{
	uint8_t input[] = {0xFF, 0x11};
	uint16_t res = betou16(input);
	ASSERT_EQ(0xFF11u, res);
}

TEST(endian_betou32_works)
{
	uint8_t input[] = {0xFF, 0x11, 0xAA, 0xB2};
	uint32_t res = betou32(input);
	ASSERT_EQ(0xFF11AAB2u, res);
}

TEST(endian_betou64_works)
{
	uint8_t input[] = {0xFF, 0x11, 0xAA, 0xB2, 0xDD, 0xF1, 0x54, 0x12};
	uint64_t res = betou64(input);
	ASSERT_EQ(0xFF11AAB2DDF15412u, res);
}

TEST(endian_betoi16_works)
{
	uint8_t input[] = {0xFF, 0x11};
	int16_t res = betoi16(input);
	ASSERT_EQ((int16_t)0xFF11, res);
}

TEST(endian_betoi32_works)
{
	uint8_t input[] = {0xFF, 0x11, 0xAA, 0xB2};
	int32_t res = betoi32(input);
	ASSERT_EQ((int32_t)0xFF11AAB2, res);
}

TEST(endian_betoi64_works)
{
	uint8_t input[] = {0xFF, 0x11, 0xAA, 0xB2, 0xDD, 0xF1, 0x54, 0x12};
	int64_t res = betoi64(input);
	ASSERT_EQ((int64_t)0xFF11AAB2DDF15412, res);
}

TEST(endian_betof32_works)
{
	uint8_t input[] = {0x44, 0xB3, 0x00, 0x65};
	float res = betof32(input);
	ASSERT_EQ(1432.0123f, res);
}

TEST(endian_betof64_works)
{
	uint8_t input[] = {0x40, 0x09, 0x21, 0xFB, 0x54, 0x44, 0x2D, 0x18};
	double res = betof64(input);
	ASSERT_EQ(3.141592653589793, res);
}

TEST(endian_letou16_works)
{
	uint8_t input[] = {0xFF, 0x11};
	uint16_t res = letou16(input);
	ASSERT_EQ(0x11FFu, res);
}

TEST(endian_letou32_works)
{
	uint8_t input[] = {0xFF, 0x11, 0xAA, 0xB2};
	uint32_t res = letou32(input);
	ASSERT_EQ(0xB2AA11FFu, res);
}

TEST(endian_letou64_works)
{
	uint8_t input[] = {0xFF, 0x11, 0xAA, 0xB2, 0xDD, 0xF1, 0x54, 0x12};
	uint64_t res = letou64(input);
	ASSERT_EQ(0x1254F1DDB2AA11FFu, res);
}

TEST(endian_letoi16_works)
{
	uint8_t input[] = {0x11, 0xFF};
	int16_t res = letoi16(input);
	ASSERT_EQ((int16_t)0xFF11, res);
}

TEST(endian_letoi32_works)
{
	uint8_t input[] = {0xB2, 0xAA, 0x11, 0xFF};
	int32_t res = letoi32(input);
	ASSERT_EQ((int32_t)0xFF11AAB2, res);
}

TEST(endian_letoi64_works)
{
	uint8_t input[] = {0x12, 0x54, 0xF1, 0xDD, 0xB2, 0xAA, 0x11, 0xFF};
	int64_t res = letoi64(input);
	ASSERT_EQ((int64_t)0xFF11AAB2DDF15412, res);
}

TEST(endian_letof32_works)
{
	uint8_t input[] = {0x65, 0x00, 0xB3, 0x44};
	float res = letof32(input);
	ASSERT_EQ(1432.0123f, res);
}

TEST(endian_letof64_works)
{
	uint8_t input[] = {0x18, 0x2D, 0x44, 0x54, 0xFB, 0x21, 0x09, 0x40};
	double res = letof64(input);
	ASSERT_EQ(3.141592653589793, res);
}

TEST(endian_u16tobe_works)
{
	uint8_t buf[2] = {0};

	u16tobe(0xA71F, buf);
	ASSERT_EQ(0xA7, buf[0]);
	ASSERT_EQ(0x1F, buf[1]);
}

TEST(endian_u32tobe_works)
{
	uint8_t buf[4] = {0};

	u32tobe(0xA71FB544, buf);
	ASSERT_EQ(0xA7, buf[0]);
	ASSERT_EQ(0x1F, buf[1]);
	ASSERT_EQ(0xB5, buf[2]);
	ASSERT_EQ(0x44, buf[3]);
}

TEST(endian_u64tobe_works)
{
	uint8_t buf[8] = {0};

	u64tobe(0x1254F1DDA71FB544, buf);
	ASSERT_EQ(0x12, buf[0]);
	ASSERT_EQ(0x54, buf[1]);
	ASSERT_EQ(0xF1, buf[2]);
	ASSERT_EQ(0xDD, buf[3]);
	ASSERT_EQ(0xA7, buf[4]);
	ASSERT_EQ(0x1F, buf[5]);
	ASSERT_EQ(0xB5, buf[6]);
	ASSERT_EQ(0x44, buf[7]);
}

TEST(endian_i16tobe_works)
{
	uint8_t buf[2] = {0};

	i16tobe(-12345, buf);
	ASSERT_EQ(0xCF, buf[0]);
	ASSERT_EQ(0xC7, buf[1]);
}

TEST(endian_i32tobe_works)
{
	uint8_t buf[4] = {0};

	i32tobe(-123456789, buf);
	ASSERT_EQ(0xF8, buf[0]);
	ASSERT_EQ(0xA4, buf[1]);
	ASSERT_EQ(0x32, buf[2]);
	ASSERT_EQ(0xEB, buf[3]);
}

TEST(endian_i64tobe_works)
{
	uint8_t buf[8] = {0};

	i64tobe(-1234567890123456789LL, buf);
	ASSERT_EQ(0xEE, buf[0]);
	ASSERT_EQ(0xDD, buf[1]);
	ASSERT_EQ(0xEF, buf[2]);
	ASSERT_EQ(0x0B, buf[3]);
	ASSERT_EQ(0x82, buf[4]);
	ASSERT_EQ(0x16, buf[5]);
	ASSERT_EQ(0x7E, buf[6]);
	ASSERT_EQ(0xEB, buf[7]);
}

TEST(endian_f32tobe_works)
{
	uint8_t buf[4] = {0};

	f32tobe(1432.0123f, buf);
	ASSERT_EQ(0x44, buf[0]);
	ASSERT_EQ(0xB3, buf[1]);
	ASSERT_EQ(0x00, buf[2]);
	ASSERT_EQ(0x65, buf[3]);

	f32tobe(2.0f, buf);
	ASSERT_EQ(0x40, buf[0]);
	ASSERT_EQ(0x00, buf[1]);
	ASSERT_EQ(0x00, buf[2]);
	ASSERT_EQ(0x00, buf[3]);
}

TEST(endian_f64tobe_works)
{
	uint8_t buf[8] = {0};

	f64tobe(3.141592653589793, buf);
	ASSERT_EQ(0x40, buf[0]);
	ASSERT_EQ(0x09, buf[1]);
	ASSERT_EQ(0x21, buf[2]);
	ASSERT_EQ(0xFB, buf[3]);
	ASSERT_EQ(0x54, buf[4]);
	ASSERT_EQ(0x44, buf[5]);
	ASSERT_EQ(0x2D, buf[6]);
	ASSERT_EQ(0x18, buf[7]);
}

TEST(endian_u16tole_works)
{
	uint8_t buf[2] = {0};

	u16tole(0xF2A5, buf);
	ASSERT_EQ(0xA5, buf[0]);
	ASSERT_EQ(0xF2, buf[1]);
}

TEST(endian_u32tole_works)
{
	uint8_t buf[4] = {0};

	u32tole(0xF2A511B6, buf);
	ASSERT_EQ(0xB6, buf[0]);
	ASSERT_EQ(0x11, buf[1]);
	ASSERT_EQ(0xA5, buf[2]);
	ASSERT_EQ(0xF2, buf[3]);
}

TEST(endian_u64tole_works)
{
	uint8_t buf[8] = {0};

	u64tole(0xF2A511B6DDF15412, buf);
	ASSERT_EQ(0x12, buf[0]);
	ASSERT_EQ(0x54, buf[1]);
	ASSERT_EQ(0xF1, buf[2]);
	ASSERT_EQ(0xDD, buf[3]);
	ASSERT_EQ(0xB6, buf[4]);
	ASSERT_EQ(0x11, buf[5]);
	ASSERT_EQ(0xA5, buf[6]);
	ASSERT_EQ(0xF2, buf[7]);
}

TEST(endian_i16tole_works)
{
	uint8_t buf[2] = {0};

	i16tole(-12345, buf);
	ASSERT_EQ(0xC7, buf[0]);
	ASSERT_EQ(0xCF, buf[1]);
}

TEST(endian_i32tole_works)
{
	uint8_t buf[4] = {0};

	i32tole(-123456789, buf);
	ASSERT_EQ(0xEB, buf[0]);
	ASSERT_EQ(0x32, buf[1]);
	ASSERT_EQ(0xA4, buf[2]);
	ASSERT_EQ(0xF8, buf[3]);
}

TEST(endian_i64tole_works)
{
	uint8_t buf[8] = {0};

	i64tole(-1234567890123456789LL, buf);
	ASSERT_EQ(0xEB, buf[0]);
	ASSERT_EQ(0x7E, buf[1]);
	ASSERT_EQ(0x16, buf[2]);
	ASSERT_EQ(0x82, buf[3]);
	ASSERT_EQ(0x0B, buf[4]);
	ASSERT_EQ(0xEF, buf[5]);
	ASSERT_EQ(0xDD, buf[6]);
	ASSERT_EQ(0xEE, buf[7]);
}

TEST(endian_f32tole_works)
{
	uint8_t buf[4] = {0};

	f32tole(1432.0123f, buf);
	ASSERT_EQ(0x65, buf[0]);
	ASSERT_EQ(0x00, buf[1]);
	ASSERT_EQ(0xB3, buf[2]);
	ASSERT_EQ(0x44, buf[3]);

	f32tole(2.f, buf);
	ASSERT_EQ(0x00, buf[0]);
	ASSERT_EQ(0x00, buf[1]);
	ASSERT_EQ(0x00, buf[2]);
	ASSERT_EQ(0x40, buf[3]);
}

TEST(endian_f64tole_works)
{
	uint8_t buf[8] = {0};

	f64tole(3.141592653589793, buf);
	ASSERT_EQ(0x18, buf[0]);
	ASSERT_EQ(0x2D, buf[1]);
	ASSERT_EQ(0x44, buf[2]);
	ASSERT_EQ(0x54, buf[3]);
	ASSERT_EQ(0xFB, buf[4]);
	ASSERT_EQ(0x21, buf[5]);
	ASSERT_EQ(0x09, buf[6]);
	ASSERT_EQ(0x40, buf[7]);
}

TEST_MAIN(
	endian_betou16_works,
	endian_betou32_works,
	endian_betou64_works,
	endian_betoi16_works,
	endian_betoi32_works,
	endian_betoi64_works,
	endian_betof32_works,
	endian_betof64_works,
	endian_letou16_works,
	endian_letou32_works,
	endian_letou64_works,
	endian_letoi16_works,
	endian_letoi32_works,
	endian_letoi64_works,
	endian_letof32_works,
	endian_letof64_works,
	endian_u16tobe_works,
	endian_u32tobe_works,
	endian_u64tobe_works,
	endian_i16tobe_works,
	endian_i32tobe_works,
	endian_i64tobe_works,
	endian_f32tobe_works,
	endian_f64tobe_works,
	endian_u16tole_works,
	endian_u32tole_works,
	endian_u64tole_works,
	endian_i16tole_works,
	endian_i32tole_works,
	endian_i64tole_works,
	endian_f32tole_works,
	endian_f64tole_works)

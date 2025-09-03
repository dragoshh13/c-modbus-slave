/**
 * @file test_lib.h
 * @brief A slim C testing library
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

#ifndef TEST_LIB_H_INCLUDED
#define TEST_LIB_H_INCLUDED

#include <stdio.h>
#include <string.h>

#define PRINTF(val) \
	printf(_Generic((val), \
		char: "%c", \
		short: "%d", \
		int: "%d", \
		long: "%ld", \
		long long: "%lld", \
		unsigned char: "%u", \
		unsigned short: "%u", \
		unsigned int: "%u", \
		unsigned long: "%lu", \
		unsigned long long: "%llu", \
		float: "%.2f", \
		double: "%.2f", \
		char*: "\"%s\"", \
		const char*: "\"%s\"", \
		default: "%p"), (val))

#define FILENAME (strrchr("/" __FILE__, '/') + 1)

#define INTERNAL_ASSERT(expr, assert) \
	if (!(expr)) { \
		printf("%s:%d - expected<%s>\n", FILENAME, __LINE__, #expr); \
		*test_failed = 1; \
		if (assert) return; \
	}

#define INTERNAL_ASSERT_EQ(expect, actual, assert) \
	if ((expect) != (actual)) { \
		printf("%s:%d - expected<%s=", FILENAME, __LINE__, #expect); \
		PRINTF(expect); \
		printf("> actual<%s=", #actual); \
		PRINTF(actual); \
		printf(">\n"); \
		*test_failed = 1; \
		if (assert) return; \
	}

#define EXPECT(expr) INTERNAL_ASSERT((expr), 0)
#define ASSERT(expr) INTERNAL_ASSERT((expr), 1)

#define EXPECT_EQ(expect, actual) INTERNAL_ASSERT_EQ(expect, actual, 0)
#define ASSERT_EQ(expect, actual) INTERNAL_ASSERT_EQ(expect, actual, 1)

struct test_info_s {
	void (*fn)(int *test_failed);
	const char *name;
};

#define TEST(name) \
	static void name##_fn(int *test_failed); \
	static const struct test_info_s name = {name##_fn, #name}; \
	static void name##_fn(int *test_failed)

#define TEST_MAIN(...) \
	int main(void) \
	{ \
		const struct test_info_s tests[] = {__VA_ARGS__}; \
		int i, test_failed, failed_tests=0; \
		for (i=0; i < sizeof tests / sizeof(struct test_info_s); ++i) { \
			test_failed = 0; \
			tests[i].fn(&test_failed); \
			if (test_failed) ++failed_tests; \
			printf("%s: %s\n", test_failed ? "FAILED" : "PASSED", tests[i].name); \
		} \
		return failed_tests ? 1 : 0; \
	}

#endif /* TEST_LIB_H_INCLUDED */

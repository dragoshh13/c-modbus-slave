#include "test_lib.h"
#include <mbadu_ascii.h>

TEST(mbadu_ascii_works)
{
	uint16_t reg_val = 0x1234;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x10u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	uint8_t rx_buf[] = ":010300100001EB\r\n";

	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);

	ASSERT_EQ(15, res_size);
	ASSERT_EQ(':', tx_buf[0]);
	ASSERT_EQ('0', tx_buf[1]); /* Slave addr H */
	ASSERT_EQ('1', tx_buf[2]); /* Slave addr L */
	ASSERT_EQ('0', tx_buf[3]); /* Function code H */
	ASSERT_EQ('3', tx_buf[4]); /* Function code L */
	ASSERT_EQ('0', tx_buf[5]); /* Byte count H */
	ASSERT_EQ('2', tx_buf[6]); /* Byte count L */
	ASSERT_EQ('1', tx_buf[7]); /* Data */
	ASSERT_EQ('2', tx_buf[8]); /* Data */
	ASSERT_EQ('3', tx_buf[9]); /* Data */
	ASSERT_EQ('4', tx_buf[10]); /* Data */
	ASSERT_EQ('B', tx_buf[11]); /* LRC H */
	ASSERT_EQ('4', tx_buf[12]); /* LRC L */
	ASSERT_EQ('\r', tx_buf[13]);
	ASSERT_EQ('\n', tx_buf[14]);
}

TEST(mbadu_ascii_null_pointers)
{
	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	uint8_t rx_buf[] = ":010300100001EB\r\n";
	struct mbinst_s inst = {.serial={.slave_addr=1u}};
	mbinst_init(&inst);

	/* Null instance */
	size_t res_size = mbadu_ascii_handle_req(NULL, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);

	/* Null request */
	res_size = mbadu_ascii_handle_req(&inst, NULL, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);

	/* Null response buffer */
	res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, NULL);
	ASSERT_EQ(0, res_size);
}

TEST(mbadu_ascii_invalid_length)
{
	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	struct mbinst_s inst = {.serial={.slave_addr=1u}};
	mbinst_init(&inst);

	/* Too short */
	uint8_t rx_short[] = ":0103\r\n";
	size_t res_size = mbadu_ascii_handle_req(&inst, rx_short, (sizeof rx_short)-1, tx_buf);
	ASSERT_EQ(0, res_size);

	/* Too long (exceeds max size) */
	uint8_t rx_long[MBADU_ASCII_SIZE_MAX + 10];
	memset(rx_long, '0', sizeof rx_long);
	rx_long[0] = ':';
	rx_long[(sizeof rx_long)-2] = '\r';
	rx_long[(sizeof rx_long)-1] = '\n';
	res_size = mbadu_ascii_handle_req(&inst, rx_long, (sizeof rx_long), tx_buf);
	ASSERT_EQ(0, res_size);
}

TEST(mbadu_ascii_invalid_slave_address)
{
	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];

	/* Instance with invalid slave address (too low) */
	struct mbinst_s inst_low = {.serial={.slave_addr=0u}};
	mbinst_init(&inst_low);
	uint8_t rx_buf[] = ":010300100001EB\r\n";
	size_t res_size = mbadu_ascii_handle_req(&inst_low, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);

	/* Instance with invalid slave address (too high) */
	struct mbinst_s inst_high = {.serial={.slave_addr=248u}};
	mbinst_init(&inst_high);
	res_size = mbadu_ascii_handle_req(&inst_high, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);
}

TEST(mbadu_ascii_missing_start_char)
{
	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	struct mbinst_s inst = {.serial={.slave_addr=1u}};
	mbinst_init(&inst);

	/* Missing start character */
	uint8_t rx_buf[] = "010300100001EB\r\n";
	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);

	/* Wrong start character */
	uint8_t rx_buf2[] = ";010300100001EB\r\n";
	res_size = mbadu_ascii_handle_req(&inst, rx_buf2, (sizeof rx_buf2)-1, tx_buf);
	ASSERT_EQ(0, res_size);
}

TEST(mbadu_ascii_missing_crlf)
{
	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	struct mbinst_s inst = {.serial={.slave_addr=1u}};
	mbinst_init(&inst);

	/* Missing CR/LF */
	uint8_t rx_buf[] = ":010300100001EB";
	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);

	/* Missing CR */
	uint8_t rx_buf2[] = ":010300100001EB\n";
	res_size = mbadu_ascii_handle_req(&inst, rx_buf2, (sizeof rx_buf2)-1, tx_buf);
	ASSERT_EQ(0, res_size);

	/* Missing LF */
	uint8_t rx_buf3[] = ":010300100001EB\r";
	res_size = mbadu_ascii_handle_req(&inst, rx_buf3, (sizeof rx_buf3)-1, tx_buf);
	ASSERT_EQ(0, res_size);

	/* Wrong order */
	uint8_t rx_buf4[] = ":010300100001EB\n\r";
	res_size = mbadu_ascii_handle_req(&inst, rx_buf4, (sizeof rx_buf4)-1, tx_buf);
	ASSERT_EQ(0, res_size);
}

TEST(mbadu_ascii_odd_length_pdu)
{
	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	struct mbinst_s inst = {.serial={.slave_addr=1u}};
	mbinst_init(&inst);

	/* Odd number of hex characters */
	uint8_t rx_buf[] = ":01030010000EB\r\n"; /* Missing one hex digit */
	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);
}

TEST(mbadu_ascii_invalid_hex_chars)
{
	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	struct mbinst_s inst = {.serial={.slave_addr=1u}};
	mbinst_init(&inst);

	/* Invalid hex character 'G' */
	uint8_t rx_buf[] = ":G10300100001EB\r\n";
	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);

	/* Invalid hex character 'Z' */
	uint8_t rx_buf2[] = ":010300100001ZB\r\n";
	res_size = mbadu_ascii_handle_req(&inst, rx_buf2, (sizeof rx_buf2)-1, tx_buf);
	ASSERT_EQ(0, res_size);

	/* Space character */
	uint8_t rx_buf3[] = ":01030010000 EB\r\n";
	res_size = mbadu_ascii_handle_req(&inst, rx_buf3, (sizeof rx_buf3)-1, tx_buf);
	ASSERT_EQ(0, res_size);
}

TEST(mbadu_ascii_wrong_slave_address)
{
	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	struct mbinst_s inst = {.serial={.slave_addr=5u}};
	mbinst_init(&inst);

	/* Request for different slave address */
	uint8_t rx_buf[] = ":010300100001EB\r\n"; /* Slave address 1, but instance expects 5 */
	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);
}

TEST(mbadu_ascii_broadcast_address)
{
	uint16_t reg_val = 0x1234;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x10u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	/* Broadcast address (00) - should never respond */
	uint8_t rx_buf[] = ":000300100001ED\r\n";

	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size); /* No response for broadcast */
}

TEST(mbadu_ascii_default_response_address)
{
	uint16_t reg_val = 0x1234;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x10u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};

	/* Instance with default response enabled */
	struct mbinst_s inst_enabled = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
		.serial={.slave_addr=1u, .enable_def_resp=1}
	};
	mbinst_init(&inst_enabled);

	/* Instance with default response disabled */
	struct mbinst_s inst_disabled = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
		.serial={.slave_addr=1u, .enable_def_resp=0}
	};
	mbinst_init(&inst_disabled);

	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	/* Default response address (F8 = 248) */
	uint8_t rx_buf[] = ":F80300100001F4\r\n";

	/* Should respond when enabled */
	size_t res_size = mbadu_ascii_handle_req(&inst_enabled, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(15, res_size);

	/* Should not respond when disabled */
	res_size = mbadu_ascii_handle_req(&inst_disabled, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);
}

TEST(mbadu_ascii_invalid_lrc)
{
	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	struct mbinst_s inst = {.serial={.slave_addr=1u}};
	mbinst_init(&inst);

	/* Correct: ":010300100001EB\r\n" has LRC = EB */
	/* Wrong LRC */
	uint8_t rx_buf[] = ":010300100001EC\r\n"; /* LRC should be EB, not EC */
	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(0, res_size);
}

TEST(mbadu_ascii_lowercase_hex)
{
	uint16_t reg_val = 0xABCD;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x10u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	/* Using lowercase hex characters */
	uint8_t rx_buf[] = ":010300100001eb\r\n";

	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(15, res_size);
	/* Response should be in uppercase */
	ASSERT_EQ(':', tx_buf[0]);
	ASSERT_EQ('0', tx_buf[1]);
	ASSERT_EQ('1', tx_buf[2]);
	ASSERT_EQ('0', tx_buf[3]);
	ASSERT_EQ('3', tx_buf[4]);
	ASSERT_EQ('0', tx_buf[5]);
	ASSERT_EQ('2', tx_buf[6]);
	ASSERT_EQ('A', tx_buf[7]); /* Uppercase A */
	ASSERT_EQ('B', tx_buf[8]); /* Uppercase B */
	ASSERT_EQ('C', tx_buf[9]); /* Uppercase C */
	ASSERT_EQ('D', tx_buf[10]); /* Uppercase D */
}

TEST(mbadu_ascii_mixed_case_hex)
{
	uint16_t reg_val = 0x1234;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x10u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	/* Mixed case hex characters */
	uint8_t rx_buf[] = ":010300100001eB\r\n";

	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(15, res_size);
}

TEST(mbadu_ascii_high_slave_address)
{
	uint16_t reg_val = 0x5678;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x10u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
		.serial={.slave_addr=247u} /* Max valid slave address */
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_ASCII_SIZE_MAX];
	/* Request for slave address 247 (F7) */
	uint8_t rx_buf[] = ":F70300100001F5\r\n";

	size_t res_size = mbadu_ascii_handle_req(&inst, rx_buf, (sizeof rx_buf)-1, tx_buf);
	ASSERT_EQ(15, res_size);
	ASSERT_EQ(':', tx_buf[0]);
	ASSERT_EQ('F', tx_buf[1]); /* Slave addr H */
	ASSERT_EQ('7', tx_buf[2]); /* Slave addr L */
	ASSERT_EQ('0', tx_buf[3]); /* Function code H */
	ASSERT_EQ('3', tx_buf[4]); /* Function code L */
	ASSERT_EQ('0', tx_buf[5]); /* Byte count H */
	ASSERT_EQ('2', tx_buf[6]); /* Byte count L */
	ASSERT_EQ('5', tx_buf[7]); /* Data */
	ASSERT_EQ('6', tx_buf[8]); /* Data */
	ASSERT_EQ('7', tx_buf[9]); /* Data */
	ASSERT_EQ('8', tx_buf[10]); /* Data */
}

TEST_MAIN(
	mbadu_ascii_works,
	mbadu_ascii_null_pointers,
	mbadu_ascii_invalid_length,
	mbadu_ascii_invalid_slave_address,
	mbadu_ascii_missing_start_char,
	mbadu_ascii_missing_crlf,
	mbadu_ascii_odd_length_pdu,
	mbadu_ascii_invalid_hex_chars,
	mbadu_ascii_wrong_slave_address,
	mbadu_ascii_broadcast_address,
	mbadu_ascii_default_response_address,
	mbadu_ascii_invalid_lrc,
	mbadu_ascii_lowercase_hex,
	mbadu_ascii_mixed_case_hex,
	mbadu_ascii_high_slave_address
)

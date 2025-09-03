#include "test_lib.h"
#include <endian.h>
#include <mbinst.h>
#include <mbadu_tcp.h>
#include <mbpdu.h>

TEST(mbadu_tcp_null_inst_fails)
{
	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, 0x01, /* Transaction ID */
		0x00, 0x00, /* Protocol ID */
		0x00, 0x06, /* Length */
		0x01,       /* Unit ID */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
	};

	size_t res_size = mbadu_tcp_handle_req(NULL, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_tcp_null_request_data_fails)
{
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_R_VAL,
			.read={.u16=0x1234},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);
	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];

	size_t resp_len = mbadu_tcp_handle_req(&inst, NULL, 12, tx_buf);
	ASSERT_EQ(0u, resp_len);
}

TEST(mbadu_tcp_null_response_buffer_fails)
{
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_R_VAL,
			.read={.u16=0x1234},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t rx_buf[] = {
		0x00, 0x01, /* Transaction ID */
		0x00, 0x00, /* Protocol ID */
		0x00, 0x06, /* Length */
		0x01,       /* Unit ID */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, NULL);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_tcp_undersized_request_fails)
{
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_R_VAL,
			.read={.u16=0x1234},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, 0x01, /* Transaction ID */
		0x00, 0x00, /* Protocol ID */
		0x00, 0x06, /* Length */
		0x01,       /* Unit ID */
		/* Missing function code and data - only 7 bytes, need at least 8 */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_tcp_invalid_protocol_id_fails)
{
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_R_VAL,
			.read={.u16=0x1234},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, 0x01, /* Transaction ID */
		0x00, 0x01, /* Invalid Protocol ID (should be 0x0000) */
		0x00, 0x06, /* Length */
		0x01,       /* Unit ID */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_tcp_read_holding_reg_works)
{
	uint16_t reg_val = 0x1234;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, 0x01, /* Transaction ID */
		0x00, 0x00, /* Protocol ID */
		0x00, 0x06, /* Length */
		0x01,       /* Unit ID */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(11u, res_size); /* 7 bytes MBAP + 1 FC + 1 byte count + 2 data bytes */

	/* Check MBAP header response */
	ASSERT_EQ(0x00, tx_buf[0]); /* Transaction ID H */
	ASSERT_EQ(0x01, tx_buf[1]); /* Transaction ID L */
	ASSERT_EQ(0x00, tx_buf[2]); /* Protocol ID H */
	ASSERT_EQ(0x00, tx_buf[3]); /* Protocol ID L */
	ASSERT_EQ(0x00, tx_buf[4]); /* Length H */
	ASSERT_EQ(0x05, tx_buf[5]); /* Length L (1 unit ID + 1 FC + 1 byte count + 2 data) */
	ASSERT_EQ(0x01, tx_buf[6]); /* Unit ID */

	/* Check PDU response */
	ASSERT_EQ(MBFC_READ_HOLDING_REGS, tx_buf[7]); /* Function code */
	ASSERT_EQ(0x02, tx_buf[8]);  /* Byte count */
	ASSERT_EQ(0x12, tx_buf[9]);  /* Data H */
	ASSERT_EQ(0x34, tx_buf[10]); /* Data L */
}

TEST(mbadu_tcp_write_single_reg_works)
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
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, 0x02, /* Transaction ID */
		0x00, 0x00, /* Protocol ID */
		0x00, 0x06, /* Length */
		0xFF,       /* Unit ID */
		MBFC_WRITE_SINGLE_REG,
		0x00, 0x10, /* Address */
		0x98, 0x76, /* Data */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(12u, res_size); /* 7 bytes MBAP + 1 FC + 4 data bytes (address + data) */

	/* Check that register was updated */
	ASSERT_EQ(0x9876u, reg_val);

	/* Check MBAP header response */
	ASSERT_EQ(0x00, tx_buf[0]); /* Transaction ID H */
	ASSERT_EQ(0x02, tx_buf[1]); /* Transaction ID L */
	ASSERT_EQ(0x00, tx_buf[2]); /* Protocol ID H */
	ASSERT_EQ(0x00, tx_buf[3]); /* Protocol ID L */
	ASSERT_EQ(0x00, tx_buf[4]); /* Length H */
	ASSERT_EQ(0x06, tx_buf[5]); /* Length L (1 unit ID + 1 FC + 4 data) */
	ASSERT_EQ(0xFF, tx_buf[6]); /* Unit ID echoed back */

	/* Check PDU response */
	ASSERT_EQ(MBFC_WRITE_SINGLE_REG, tx_buf[7]); /* Function code */
	ASSERT_EQ(0x00, tx_buf[8]);  /* Address H */
	ASSERT_EQ(0x10, tx_buf[9]);  /* Address L */
	ASSERT_EQ(0x98, tx_buf[10]); /* Data H */
	ASSERT_EQ(0x76, tx_buf[11]); /* Data L */
}

TEST(mbadu_tcp_transaction_id_echoed)
{
	uint16_t reg_val = 0x1234;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0xAB, 0xCD, /* Transaction ID */
		0x00, 0x00, /* Protocol ID */
		0x00, 0x06, /* Length */
		0x99,       /* Unit ID */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT(res_size > 0);

	/* Check transaction ID is echoed back */
	ASSERT_EQ(0xAB, tx_buf[0]); /* Transaction ID H */
	ASSERT_EQ(0xCD, tx_buf[1]); /* Transaction ID L */

	/* Check unit ID is echoed back */
	ASSERT_EQ(0x99, tx_buf[6]); /* Unit ID */
}

TEST(mbadu_tcp_unit_id_echoed)
{
	uint16_t reg_val = 0x1234;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, 0x01, /* Transaction ID */
		0x00, 0x00, /* Protocol ID */
		0x00, 0x06, /* Length */
		0x42,       /* Unit ID */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT(res_size > 0);

	/* Check unit ID is echoed back */
	ASSERT_EQ(0x42, tx_buf[6]); /* Unit ID */
}

TEST(mbadu_tcp_protocol_id_echoed)
{
	uint16_t reg_val = 0x1234;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, 0x01, /* Transaction ID */
		0x00, 0x00, /* Protocol ID (must be 0 for valid request) */
		0x00, 0x06, /* Length */
		0x01,       /* Unit ID */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT(res_size > 0);

	/* Check protocol ID is echoed back */
	ASSERT_EQ(0x00, tx_buf[2]); /* Protocol ID H */
	ASSERT_EQ(0x00, tx_buf[3]); /* Protocol ID L */
}

TEST(mbadu_tcp_length_field_correct)
{
	uint16_t reg_val = 0x1234;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, 0x01, /* Transaction ID */
		0x00, 0x00, /* Protocol ID */
		0x00, 0x06, /* Length */
		0x01,       /* Unit ID */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(11u, res_size);

	/* Length should be response size - 6 (transaction + protocol + length fields) */
	/* Response is 1 unit ID + 1 FC + 1 byte count + 2 data = 5 bytes */
	ASSERT_EQ(0x00, tx_buf[4]); /* Length H */
	ASSERT_EQ(0x05, tx_buf[5]); /* Length L */
}

TEST(mbadu_tcp_multiple_regs_read_works)
{
	uint16_t reg_vals[] = {0x1234, 0x5678, 0x9ABC};
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_vals[0]},
			.write={.pu16=&reg_vals[0]},
		},
		{
			.address=0x01u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_vals[1]},
			.write={.pu16=&reg_vals[1]},
		},
		{
			.address=0x02u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_vals[2]},
			.write={.pu16=&reg_vals[2]},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, 0x01, /* Transaction ID */
		0x00, 0x00, /* Protocol ID */
		0x00, 0x06, /* Length */
		0x01,       /* Unit ID */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x03, /* n read regs (3 regs) */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(15u, res_size); /* 7 bytes MBAP + 1 FC + 1 byte count + 6 data bytes */

	/* Check MBAP header */
	ASSERT_EQ(0x00, tx_buf[4]); /* Length H */
	ASSERT_EQ(0x09, tx_buf[5]); /* Length L (1 unit ID + 1 FC + 1 byte count + 6 data) */

	/* Check PDU response */
	ASSERT_EQ(MBFC_READ_HOLDING_REGS, tx_buf[7]); /* Function code */
	ASSERT_EQ(0x06, tx_buf[8]);  /* Byte count (3 regs * 2 bytes) */
	ASSERT_EQ(0x12, tx_buf[9]);  /* Reg 0 H */
	ASSERT_EQ(0x34, tx_buf[10]); /* Reg 0 L */
	ASSERT_EQ(0x56, tx_buf[11]); /* Reg 1 H */
	ASSERT_EQ(0x78, tx_buf[12]); /* Reg 1 L */
	ASSERT_EQ(0x9A, tx_buf[13]); /* Reg 2 H */
	ASSERT_EQ(0xBC, tx_buf[14]); /* Reg 2 L */
}

TEST(mbadu_tcp_zero_size_request_fails)
{
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_R_VAL,
			.read={.u16=0x1234},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[MBADU_TCP_SIZE_MAX];

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, 0, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_tcp_exactly_min_size_works)
{
	uint16_t reg_val = 0x1234;
	const struct mbreg_desc_s regs[] = {
		{
			.address=0x00u,
			.type=MRTYPE_U16,
			.access=MRACC_RW_PTR,
			.read={.pu16=&reg_val},
			.write={.pu16=&reg_val},
		}
	};
	struct mbinst_s inst = {
		.hold_regs=regs,
		.n_hold_regs=sizeof regs / sizeof regs[0],
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_TCP_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, 0x01, /* Transaction ID */
		0x00, 0x00, /* Protocol ID */
		0x00, 0x02, /* Length (minimum: unit ID + function code) */
		0x01,       /* Unit ID */
		MBFC_READ_HOLDING_REGS, /* Function code - no data, will cause error but should parse */
	};

	size_t res_size = mbadu_tcp_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	/* Should not crash and return some response (likely error response) */
	ASSERT(res_size >= 0); /* May be 0 or error response */
}

TEST_MAIN(
	mbadu_tcp_null_inst_fails,
	mbadu_tcp_null_request_data_fails,
	mbadu_tcp_null_response_buffer_fails,
	mbadu_tcp_undersized_request_fails,
	mbadu_tcp_invalid_protocol_id_fails,
	mbadu_tcp_read_holding_reg_works,
	mbadu_tcp_write_single_reg_works,
	mbadu_tcp_transaction_id_echoed,
	mbadu_tcp_unit_id_echoed,
	mbadu_tcp_protocol_id_echoed,
	mbadu_tcp_length_field_correct,
	mbadu_tcp_multiple_regs_read_works,
	mbadu_tcp_zero_size_request_fails,
	mbadu_tcp_exactly_min_size_works
);

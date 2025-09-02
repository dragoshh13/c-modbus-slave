#include "test_lib.h"
#include <mbinst.h>
#include <mbadu.h>
#include <mbcrc.h>
#include <utils/endian.h>

TEST(mbadu_null_inst_fails)
{
	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t slave_addr = 1u;
	uint8_t rx_buf[] = {
		slave_addr,
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
		0x00, 0x00, /* CRC */
	};
	u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);

	size_t res_size = mbadu_handle_req(NULL, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_null_request_data_fails)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_SIZE_MAX];

	size_t resp_len = mbadu_handle_req(&inst, NULL, 8, tx_buf);
	ASSERT_EQ(0u, resp_len);
}

TEST(mbadu_null_response_buffer_fails)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t rx_buf[] = {
		inst.serial.slave_addr,
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
		0x00, 0x00, /* CRC */
	};
	u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, NULL);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_zero_size_request_fails)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[MBADU_SIZE_MAX];

	size_t res_size = mbadu_handle_req(&inst, rx_buf, 0, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_undersized_request_fails)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[] = {inst.serial.slave_addr, MBFC_READ_HOLDING_REGS, 0x00}; /* Only 3 bytes, need at least 4 */

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_unmatched_slave_address_fails)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	/* Request for slave address 5, but we're slave 1 */
	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, /* Not this device */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00,
		0x00, 0x01,
		0x00, 0x00
	};
	u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_broadcast_slave_addr_works)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x00, /* Slave addr (Broadcast) */
		MBFC_WRITE_SINGLE_REG,
		0x00, 0x00, /* Address */
		0x98, 0x76, /* Data */
		0x00, 0x00, /* CRC */
	};
	u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);

	ASSERT_EQ(0u, res_size); /* No response */
	ASSERT_EQ(0x9876u, reg_val); /* But data was modified */
}

TEST(mbadu_response_slave_address_works)
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

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[] = {
		inst.serial.slave_addr,
		MBFC_READ_HOLDING_REGS,
		0x00, 0x10, /* Start addr */
		0x00, 0x01, /* n read regs */
		0x00, 0x00, /* CRC */
	};
	u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT(res_size > 0);
	ASSERT_EQ(inst.serial.slave_addr, tx_buf[0]); /* Response should echo slave address */
}

TEST(mbadu_default_response_slave_addr_works)
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

		.serial={
			.slave_addr=1u,
			.enable_def_resp=1 /* Must be enabled for this test */
		}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[] = {
		0xF8, /* Slave addr (248 - MbADU_ADDR_DEFAULT_RESP) */
		MBFC_WRITE_SINGLE_REG,
		0x00, 0x10, /* Address */
		0x98, 0x76, /* Data */
		0x00, 0x00, /* CRC */
	};
	u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(8u, res_size);
	ASSERT_EQ(0xF8, tx_buf[0]) /* Slave address */
	ASSERT_EQ(MBFC_WRITE_SINGLE_REG, tx_buf[1]) /* Function code  */
	ASSERT_EQ(0x00, tx_buf[2]) /* Address H  */
	ASSERT_EQ(0x10, tx_buf[3]) /* Address L  */
	ASSERT_EQ(0x98, tx_buf[4]) /* Data H  */
	ASSERT_EQ(0x76, tx_buf[5]) /* Data L  */

	ASSERT_EQ(0x9876, reg_val);
}

TEST(mbadu_malformed_crc_fails)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[] = {
		0x02, /* Slave addr (Not this device) */
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
		0x00, 0x00, /* CRC */
	};
	u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_response_crc_works)
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

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[] = {
		inst.serial.slave_addr,
		MBFC_READ_HOLDING_REGS,
		0x00, 0x10, /* Start addr */
		0x00, 0x01, /* n read regs */
		0x00, 0x00, /* CRC */
	};
	u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT(res_size > 0);
	ASSERT_EQ(mbcrc16(tx_buf, res_size - 2), letou16(tx_buf + res_size - 2));
}

TEST(mbadu_read_holding_reg_works)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[] = {
		inst.serial.slave_addr,
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
		0x00, 0x00, /* CRC */
	};
	u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(7, res_size);
	ASSERT_EQ(inst.serial.slave_addr, tx_buf[0]);
	ASSERT_EQ(0x03, tx_buf[1]); /* Function code */
	ASSERT_EQ(2u, tx_buf[2]); /* Byte count */
	ASSERT_EQ(0x12, tx_buf[3]); /* Data H */
	ASSERT_EQ(0x34, tx_buf[4]); /* Data L */
}

TEST(mbadu_less_than_min_size_fails)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t slave_addr = 1u;
	uint8_t rx_buf[] = {
		inst.serial.slave_addr,
		/*MBFC_READ_HOLDING_REGS,*/
		/*0x00, 0x00,*/ /* Start addr */
		/*0x00, 0x02,*/ /* n read regs */
		0x00, 0x00, /* CRC */
	};
	u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_more_than_max_size_fails)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[MBADU_SIZE_MAX+1];

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST(mbadu_incorrect_crc_fails)
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
		.serial={.slave_addr=1u}
	};
	mbinst_init(&inst);

	uint8_t tx_buf[MBADU_SIZE_MAX];
	uint8_t rx_buf[] = {
		inst.serial.slave_addr,
		MBFC_READ_HOLDING_REGS,
		0x00, 0x00, /* Start addr */
		0x00, 0x01, /* n read regs */
		0x00, 0x00, /* CRC */
	};
	/*u16tole(mbcrc16(rx_buf, sizeof rx_buf - 2), rx_buf + sizeof rx_buf - 2);*/

	size_t res_size = mbadu_handle_req(&inst, rx_buf, sizeof rx_buf, tx_buf);
	ASSERT_EQ(0u, res_size);
}

TEST_MAIN(
	mbadu_null_inst_fails,
	mbadu_null_request_data_fails,
	mbadu_null_response_buffer_fails,
	mbadu_zero_size_request_fails,
	mbadu_undersized_request_fails,
	mbadu_unmatched_slave_address_fails,
	mbadu_broadcast_slave_addr_works,
	mbadu_response_slave_address_works,
	mbadu_default_response_slave_addr_works,
	mbadu_malformed_crc_fails,
	mbadu_response_crc_works,
	mbadu_read_holding_reg_works,
	mbadu_less_than_min_size_fails,
	mbadu_more_than_max_size_fails
);

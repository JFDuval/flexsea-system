#ifdef __cplusplus
extern "C" {
#endif

#include "flexsea-system_test-all.h"

//****************************************************************************
// Unit test(s)
//****************************************************************************

//Control Mode:
//=============

void test_tx_cmd_ctrl_mode_w(void)
{
	int i = 0;
	uint8_t controller = 5;
	prepTxCmdTest();

	tx_cmd_ctrl_mode_w(tmpPayload, &cmdCode, &cmdType, &len, controller);

	TEST_ASSERT_EQUAL(CMD_CTRL_MODE, cmdCode);
	TEST_ASSERT_EQUAL(CMD_WRITE, cmdType);
	TEST_ASSERT_EQUAL(1, len);

	TEST_ASSERT_EQUAL(controller, tmpPayload[0]);
	for(i = 1; i < PAYLOAD_BUF_LEN; i++)
	{
		TEST_ASSERT_EQUAL(FILLER, tmpPayload[i]);
	}
}

void test_tx_cmd_ctrl_mode_r(void)
{
	int i = 0;
	prepTxCmdTest();

	tx_cmd_ctrl_mode_r(tmpPayload, &cmdCode, &cmdType, &len);

	TEST_ASSERT_EQUAL(CMD_CTRL_MODE, cmdCode);
	TEST_ASSERT_EQUAL(CMD_READ, cmdType);
	TEST_ASSERT_EQUAL(0, len);

	for(i = 0; i < PAYLOAD_BUF_LEN; i++)
	{
		TEST_ASSERT_EQUAL(FILLER, tmpPayload[i]);
	}
}

//TODO:
void test_rx_cmd_ctrl_mode_w(void)
{

}

void test_rx_cmd_ctrl_mode_rw(void)
{

}

void test_rx_cmd_ctrl_mode_rr(void)
{

}

//Current Setpoint:
//=================

void test_tx_cmd_ctrl_i_w(void)
{
	int i = 0;
	int16_t setP = 3456, res = 0;
	uint16_t index = 0;
	prepTxCmdTest();

	tx_cmd_ctrl_i_w(tmpPayload, &cmdCode, &cmdType, &len, setP);

	TEST_ASSERT_EQUAL(CMD_CTRL_I, cmdCode);
	TEST_ASSERT_EQUAL(CMD_WRITE, cmdType);
	TEST_ASSERT_EQUAL(4, len);

	//Only testing the 2nd word:
	res = (int16_t) REBUILD_UINT16(&tmpPayload[2], &index);
	TEST_ASSERT_EQUAL(setP, res);
	
	for(i = 4; i < PAYLOAD_BUF_LEN; i++)
	{
		TEST_ASSERT_EQUAL(FILLER, tmpPayload[i]);
	}
}

void test_tx_cmd_ctrl_i_r(void)
{
	int i = 0;
	prepTxCmdTest();

	tx_cmd_ctrl_i_r(tmpPayload, &cmdCode, &cmdType, &len);

	TEST_ASSERT_EQUAL(CMD_CTRL_I, cmdCode);
	TEST_ASSERT_EQUAL(CMD_READ, cmdType);
	TEST_ASSERT_EQUAL(0, len);

	for(i = 0; i < PAYLOAD_BUF_LEN; i++)
	{
		TEST_ASSERT_EQUAL(FILLER, tmpPayload[i]);
	}
}

//TODO:
void test_rx_cmd_ctrl_i_w(void)
{

}

void test_rx_cmd_ctrl_i_rw(void)
{

}

void test_rx_cmd_ctrl_i_rr(void)
{

}

//****************************************************************************
// Public interface:
//****************************************************************************

void test_flexsea_cmd_external(void)
{
	//Add all your unit tests to this list:
	RUN_TEST(test_tx_cmd_ctrl_mode_w);
	RUN_TEST(test_tx_cmd_ctrl_mode_r);
	//RUN_TEST(test_rx_cmd_ctrl_mode_w);
	//RUN_TEST(test_rx_cmd_ctrl_mode_rw);
	//RUN_TEST(test_rx_cmd_ctrl_mode_rr);
	RUN_TEST(test_tx_cmd_ctrl_i_w);
	RUN_TEST(test_tx_cmd_ctrl_i_r);
	//RUN_TEST(test_rx_cmd_ctrl_i_w);
	//RUN_TEST(test_rx_cmd_ctrl_i_rw);
	//RUN_TEST(test_rx_cmd_ctrl_i_rr);
	//...
}

#ifdef __cplusplus
}
#endif

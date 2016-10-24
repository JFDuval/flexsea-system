#ifdef __cplusplus
extern "C" {
#endif

#include "flexsea-system_test-all.h"

//****************************************************************************
// Unit test(s)
//****************************************************************************

void test_tx_cmd_data_read_all_r(void)
{
	int i = 0;
	prepTxCmdTest();

	tx_cmd_data_read_all_r(tmpPayload, &cmdCode, &cmdType, &len);

	TEST_ASSERT_EQUAL(CMD_READ_ALL, cmdCode);
	TEST_ASSERT_EQUAL(CMD_READ, cmdType);
	TEST_ASSERT_EQUAL(0, len);

	for(i = 0; i < PAYLOAD_BUF_LEN; i++)
	{
		TEST_ASSERT_EQUAL(FILLER, tmpPayload[i]);
	}
}

void test_tx_cmd_data_read_all_w(void)
{
	uint16_t expectedLen = 0;
	prepTxCmdTest();

	tx_cmd_data_read_all_w(tmpPayload, &cmdCode, &cmdType, &len);

	TEST_ASSERT_EQUAL(CMD_READ_ALL, cmdCode);
	TEST_ASSERT_EQUAL(CMD_WRITE, cmdType);

	//Length depends on board type:

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
	expectedLen = 29;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

	#ifdef BOARD_TYPE_FLEXSEA_MANAGE
	expectedLen = 12;
	#endif  //BOARD_TYPE_FLEXSEA_MANAGE

	#ifdef BOARD_TYPE_FLEXSEA_GOSSIP
	expectedLen = 18;
	#endif  //BOARD_TYPE_FLEXSEA_GOSSIP

	#ifdef BOARD_TYPE_FLEXSEA_STRAIN_AMP
	expectedLen = 9;
	#endif 	//BOARD_TYPE_FLEXSEA_STRAIN_AMP

	TEST_ASSERT_EQUAL_MESSAGE(expectedLen, len, "Data length (manual input)");
}

//****************************************************************************
// Public interface:
//****************************************************************************

void test_flexsea_cmd_data(void)
{
	//Add all your unit tests to this list:
	RUN_TEST(test_tx_cmd_data_read_all_r);
	RUN_TEST(test_tx_cmd_data_read_all_w);
	//...
}

#ifdef __cplusplus
}
#endif

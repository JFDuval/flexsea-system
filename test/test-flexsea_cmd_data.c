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

	tx_cmd_data_read_all_r(test_tmpPayload, &test_cmdCode, &test_cmdType, \
						   &test_len);

	TEST_ASSERT_EQUAL(CMD_READ_ALL, test_cmdCode);
	TEST_ASSERT_EQUAL(CMD_READ, test_cmdType);
	TEST_ASSERT_EQUAL(0, test_len);

	for(i = 0; i < PAYLOAD_BUF_LEN; i++)
	{
		TEST_ASSERT_EQUAL(FILLER, test_tmpPayload[i]);
	}
}

void test_tx_cmd_data_read_all_w(void)
{
	uint16_t expectedLen = 0;
	prepTxCmdTest();

	tx_cmd_data_read_all_w(test_tmpPayload, &test_cmdCode, &test_cmdType, \
						   &test_len);

	TEST_ASSERT_EQUAL(CMD_READ_ALL, test_cmdCode);
	TEST_ASSERT_EQUAL(CMD_WRITE, test_cmdType);

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

	TEST_ASSERT_EQUAL_MESSAGE(expectedLen, test_len, \
							  "Data length (manual input)");
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

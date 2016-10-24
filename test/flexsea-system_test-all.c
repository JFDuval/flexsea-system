#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "unity.h"
#include "flexsea-system_test-all.h"

//****************************************************************************
// Variables used for these sets of tests:
//****************************************************************************

uint8_t tmpPayload[PAYLOAD_BUF_LEN];
uint8_t transferBuf[COMM_STR_BUF_LEN];
uint8_t cmdCode = 0, cmdType = 0;
uint16_t len = 0;

//****************************************************************************
// Helper function(s):
//****************************************************************************

void prepTxCmdTest(void)
{
	memset(tmpPayload, FILLER, PAYLOAD_BUF_LEN);
	memset(transferBuf, FILLER, COMM_STR_BUF_LEN);
	cmdCode = 0;
	cmdType = 0;
	len = 0;
}

//****************************************************************************
// Main test function:
//****************************************************************************

//Call this function to test the 'flexsea-system' stack:
int flexsea_system_test(void)
{
	UNITY_BEGIN();

	//One call per file here:
	test_flexsea_system();
	test_flexsea_cmd_application();
	test_flexsea_cmd_control();
	test_flexsea_cmd_data();
	test_flexsea_cmd_external();
	test_flexsea_cmd_sensors();

	return UNITY_END();
}

#ifdef __cplusplus
}
#endif

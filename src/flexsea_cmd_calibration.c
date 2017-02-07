#include "flexsea_cmd_calibration.h"
#include "flexsea_system.h"
#include "flexsea_board.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
	#include "calibration_tools.h"
#endif //BOARD_TYPE_FLEXSEA_EXECUTE

void init_flexsea_payload_ptr_calibration(void)
{
	//Control mode:
	flexsea_payload_ptr[CMD_CALIBRATION_MODE][RX_PTYPE_READ] = &rx_cmd_calibration_mode_rw;
	flexsea_payload_ptr[CMD_CALIBRATION_MODE][RX_PTYPE_WRITE] = &rx_cmd_calibration_mode_w;
	flexsea_payload_ptr[CMD_CALIBRATION_MODE][RX_PTYPE_REPLY] = &rx_cmd_calibration_mode_rr;
}

void tx_cmd_calibration_mode_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t calibrationMode)
{	
	uint8_t index = 0;
	(*cmd) = CMD_CALIBRATION_MODE;
	(*cmdType) = CMD_READ;
	
	shBuf[index++] = calibrationMode;
	
	(*len) = index;
	(void)shBuf;
}

void tx_cmd_calibration_mode_rw(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t calibrationMode)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CALIBRATION_MODE;
	(*cmdType) = CMD_WRITE;

	//Data:
	shBuf[index++] = calibrationMode;

	//Payload length:
	(*len) = index;
}

void rx_cmd_calibration_mode_rr(uint8_t *buf, uint8_t *info)
{
	(void)buf;
	(void)info;
}

void rx_cmd_calibration_mode_rw(uint8_t *buf, uint8_t *info)
{
	uint8_t index = P_DATA1;
	uint8_t procedureRequested = buf[index];
	
	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
	
		if(!isRunningCalibrationProcedure() && isLegalCalibrationProcedure(procedureRequested))
		{
			calibrationFlags |= procedureRequested;
		}

		tx_cmd_calibration_mode_rw(TX_N_DEFAULT, calibrationFlags);
		packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, SEND_TO_MASTER);

	#endif //BOARD_TYPE_FLEXSEA_EXECUTE
}

void rx_cmd_calibration_mode_w(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	uint8_t procedure = buf[index];
	
	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		if(!isRunningCalibrationProcedure() && isLegalCalibrationProcedure(procedure))
		{
			calibrationFlags |= procedure;
		}

	#else

		(void)buf;
		
	#endif //BOARD_TYPE_FLEXSEA_EXECUTE
	
	(void)info;
}


#ifdef __cplusplus
}
#endif

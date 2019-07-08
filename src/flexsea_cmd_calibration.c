/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'flexsea-system' System commands & functions
	Copyright (C) 2016 Dephy, Inc. <http://dephy.com/>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************
	[Lead developer] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors]
*****************************************************************************
	[This file] flexsea_cmd_calibration: commands specific to the calibration tools
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2017-02-07 | dweisdorf | Initial GPL-3.0 release
	*
****************************************************************************/

#include "flexsea_cmd_calibration.h"
#include <flexsea.h>
#include "flexsea_system.h"
#include <flexsea_board.h>
#include "i2t-current-limit.h"
#include <stdio.h>
#ifdef INCLUDE_UPROJ_DPEB42
	#include "cmd-UTT.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if((defined BOARD_TYPE_FLEXSEA_EXECUTE) || (defined BOARD_TYPE_FLEXSEA_MANAGE))
	#include "calibration_tools.h"
#endif

#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
	#include "control.h"
#endif //BOARD_TYPE_FLEXSEA_EXECUTE

#if (defined BOARD_TYPE_FLEXSEA_MANAGE && defined BOARD_SUBTYPE_RIGID)
	#include "rigid.h"
	// this is being added for shut off command on rigid 3.0 boards with bilateral xbees
	#if(HW_VER == 30)
		#include "bilateral.h"
	#endif
#endif //(defined BOARD_TYPE_FLEXSEA_MANAGE && defined BOARD_SUBTYPE_RIGID)

#ifndef NULL
#define NULL   ((void *) 0)
#endif

#ifdef BOARD_TYPE_FLEXSEA_PLAN
#include "flexsea_global_structs.h"
struct i2t_s i2tBattW;
uint16_t uvlo = 0;
uint16_t getUVLO(void){return uvlo;}
int8_t currOffs = 0;
int8_t getCurrOffs(void){return currOffs;}
#endif

#if((defined BOARD_TYPE_FLEXSEA_MANAGE && BOARD_SUBTYPE_RIGID) || (defined BOARD_TYPE_FLEXSEA_PLAN))
struct i2t_s i2tBattR;
#endif

#if(defined BOARD_TYPE_FLEXSEA_EXECUTE)
struct i2t_s i2tMotor;
#endif

uint8_t handleCalibrationMessage(uint8_t *buf, uint8_t write);

void init_flexsea_payload_ptr_calibration(void)
{
	//Control mode:
	flexsea_payload_ptr[CMD_CALIBRATION_MODE][RX_PTYPE_READ] = &rx_cmd_calibration_mode_rw;
	flexsea_payload_ptr[CMD_CALIBRATION_MODE][RX_PTYPE_WRITE] = &rx_cmd_calibration_mode_w;
	flexsea_payload_ptr[CMD_CALIBRATION_MODE][RX_PTYPE_REPLY] = &rx_cmd_calibration_mode_rr;

	flexsea_multipayload_ptr[CMD_CALIBRATION_MODE][RX_PTYPE_READ] = &rx_multi_cmd_calibration_mode_rw;
	flexsea_multipayload_ptr[CMD_CALIBRATION_MODE][RX_PTYPE_WRITE] = &rx_multi_cmd_calibration_mode_w;
	flexsea_multipayload_ptr[CMD_CALIBRATION_MODE][RX_PTYPE_REPLY] = &rx_multi_cmd_calibration_mode_rr;
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
	//printf("Calib command called");
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CALIBRATION_MODE;
	(*cmdType) = CMD_WRITE;

	//Data:
	shBuf[index++] = calibrationMode;
	if(calibrationMode & CALIBRATION_UVLO)
	{
		#if(defined BOARD_TYPE_FLEXSEA_PLAN || (defined BOARD_TYPE_FLEXSEA_MANAGE && BOARD_SUBTYPE_RIGID))
		SPLIT_16(getUVLO(), shBuf, &index);
		#endif
	}
	else if(calibrationMode & CALIBRATION_CURRENT_OFFSET)
	{
		#if(defined BOARD_TYPE_FLEXSEA_PLAN || (defined BOARD_TYPE_FLEXSEA_MANAGE && BOARD_SUBTYPE_RIGID))
		SPLIT_16((uint16_t)getCurrOffs(), shBuf, &index);
		#endif
	}
	else if(calibrationMode & CALIBRATION_POWER_OFF)
	{
		//printf("Power off");
		#if(defined BOARD_TYPE_FLEXSEA_PLAN || defined BOARD_TYPE_FLEXSEA_MANAGE && !defined BOARD_SUBTYPE_HABSOLUTE)
		SPLIT_16((uint16_t)0, shBuf, &index);
		#endif
	}
	else if(calibrationMode & CALIBRATION_POWER_ON)
	{
		//printf("Power on");
		#if(defined BOARD_TYPE_FLEXSEA_PLAN || defined BOARD_TYPE_FLEXSEA_MANAGE && !defined BOARD_SUBTYPE_HABSOLUTE)
		SPLIT_16((uint16_t)0, shBuf, &index);
		#endif
	}
	else if(calibrationMode & CALIBRATION_I2T)
	{
		//Split I2t struct in bytes:
		#ifdef BOARD_TYPE_FLEXSEA_PLAN
		struct i2t_s *i2tTmp = &i2tBattW;
		#endif

		#if (defined BOARD_TYPE_FLEXSEA_MANAGE && BOARD_SUBTYPE_RIGID)
		struct i2t_s *i2tTmp = &i2tBatt;
		#endif
		
		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
		struct i2t_s *i2tTmp = &i2tMotor;
		#endif
		
		#if(defined BOARD_TYPE_FLEXSEA_EXECUTE || (defined BOARD_TYPE_FLEXSEA_MANAGE && BOARD_SUBTYPE_RIGID) || \
			defined BOARD_TYPE_FLEXSEA_PLAN)

		SPLIT_16((uint16_t)i2tTmp->leak, shBuf, &index);				//I2T_LEAK
		SPLIT_32((uint32_t)i2tTmp->limit, shBuf, &index);				//I2T_LIMIT
		SPLIT_16((uint16_t)i2tTmp->nonLinThreshold, shBuf, &index);		//I2T_NON_LIN_THRESHOLD
		shBuf[index++] = i2tTmp->config;								//I2T_CONFIG
		
		#else
			
		//Board not supported
		SPLIT_16(0, shBuf, &index);
		SPLIT_32(0, shBuf, &index);
		SPLIT_16(0, shBuf, &index);
		shBuf[index++] = 0;
		
		#endif
	}

	//Payload length:
	(*len) = index;
}

//Overloading this function to take an extra parameter
void tx_cmd_calibration_mode_long_rw(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t calibrationMode, uint16_t v)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CALIBRATION_MODE;
	(*cmdType) = CMD_WRITE;

	//Data:
	shBuf[index++] = calibrationMode;
	SPLIT_16(v, shBuf, &index);
	printf("tx_cmd_calibration_mode_long_rw");

	//Payload length:
	(*len) = index;
}

//Pack tx_cmd_calibration_mode_rw()
void ptx_cmd_calibration_mode_rw(uint8_t slaveId, uint16_t *numb, uint8_t *commStr, uint8_t calibrationMode)
{
	tx_cmd_calibration_mode_rw(TX_N_DEFAULT, calibrationMode);
	pack(P_AND_S_DEFAULT, slaveId, NULL, numb, commStr);
}

void rx_cmd_calibration_mode_rr(uint8_t *buf, uint8_t *info)
{
	MultiPacketInfo mInfo;
	fillMultiInfoFromBuf(&mInfo, buf, info);
	mInfo.portOut = info[1];
	rx_multi_cmd_calibration_mode_rr( buf + P_DATA1, &mInfo, tmpPayload, &cmdLen );
}

void rx_multi_cmd_calibration_mode_rr(uint8_t *msgBuf, MultiPacketInfo *mInfo, uint8_t *responseBuf, uint16_t* responseLen)
{
	(void)msgBuf;
	(void)mInfo;
	(void)responseBuf;
	(void)responseLen;

	#ifdef BOARD_TYPE_FLEXSEA_PLAN
	printf("rx_multi_cmd_calibration_mode_rr");
	uint16_t index = 1;
	if(msgBuf[0] & CALIBRATION_UVLO)
	{
		uvlo = REBUILD_UINT16(msgBuf, &index);
	}
	else if(msgBuf[0] & CALIBRATION_CURRENT_OFFSET)
	{
		currOffs = (int8_t)REBUILD_UINT16(msgBuf, &index);
	}
	else if(msgBuf[0] & CALIBRATION_I2T)
	{
		i2tBattR.leak = REBUILD_UINT16(msgBuf, &index);
		i2tBattR.limit = REBUILD_UINT32(msgBuf, &index);
		i2tBattR.nonLinThreshold = REBUILD_UINT16(msgBuf, &index);
		i2tBattR.config = msgBuf[index++];
	}
	#endif
}

void rx_cmd_calibration_mode_rw(uint8_t *buf, uint8_t *info)
{
	MultiPacketInfo mInfo;
	fillMultiInfoFromBuf(&mInfo, buf, info);
	mInfo.portOut = info[1];
	rx_multi_cmd_calibration_mode_rw( buf + P_DATA1, &mInfo, tmpPayload, &cmdLen );
	packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, SEND_TO_MASTER);
}

void rx_multi_cmd_calibration_mode_rw(uint8_t *msgBuf, MultiPacketInfo *mInfo, uint8_t *responseBuf, uint16_t* responseLen)
{
	uint8_t response = handleCalibrationMessage(msgBuf, 0);
	response = msgBuf[0];
	tx_cmd_calibration_mode_rw(responseBuf, &cmdCode, &cmdType, responseLen, response);
}

void rx_cmd_calibration_mode_w(uint8_t *buf, uint8_t *info)
{
	MultiPacketInfo mInfo;
	fillMultiInfoFromBuf(&mInfo, buf, info);
	mInfo.portOut = info[1];
	rx_multi_cmd_calibration_mode_w( buf + P_DATA1, &mInfo, tmpPayload, &cmdLen );
}

void rx_multi_cmd_calibration_mode_w(uint8_t *msgBuf, MultiPacketInfo *mInfo, uint8_t *responseBuf, uint16_t* responseLen)
{
	handleCalibrationMessage(msgBuf, 1);

	#ifndef BOARD_TYPE_FLEXSEA_EXECUTE
		(void)msgBuf;
	#endif //BOARD_TYPE_FLEXSEA_EXECUTE

	(void)mInfo;
}

uint8_t handleCalibrationMessage(uint8_t *buf, uint8_t write)
{
	uint16_t index = 0;
	uint8_t procedure = buf[index++];
	uint16_t v = 0;
	int8_t co = 0;

	uint8_t calibrationFlagToRunOrIsRunning = 0;

	if(write)
	{
		#if((defined BOARD_TYPE_FLEXSEA_EXECUTE) || (defined BOARD_TYPE_FLEXSEA_MANAGE && BOARD_SUBTYPE_RIGID))

			if(!isRunningCalibrationProcedure() && isLegalCalibrationProcedure(procedure))
			{
				calibrationFlags |= procedure;
				calibrationNew = 1;
				#if(defined BOARD_TYPE_FLEXSEA_EXECUTE)
				control_strategy(buf[0], 0);
				#endif

				#if(defined BOARD_TYPE_FLEXSEA_MANAGE)
				if(isUVLO())
				{
					v = REBUILD_UINT16(buf, &index);
					saveUVLO(v);
				}
				else if(isCurrentOffset())
				{
					co = (int8_t)REBUILD_UINT16(buf, &index);
					#ifndef BOARD_SUBTYPE_HABSOLUTE
					saveCurrOffs(co);
					#endif
				}
				else if(isI2T())
				{
					i2tBattR.leak = REBUILD_UINT16(buf, &index);
					i2tBattR.limit = REBUILD_UINT32(buf, &index);
					i2tBattR.nonLinThreshold = REBUILD_UINT16(buf, &index);
					i2tBattR.config = buf[index++];
					saveI2tRe(i2tBattR);
				}
				#ifdef INCLUDE_UPROJ_DPEB42
				else if(isPoweringOn())
				{
					// TODO: request other to boot to turn on
					utt.val[0][9] = 1;
					calibrationFlags = 0;
					#if(HW_VER == 30)
						bilateral_send_command(BILATERAL_POWER_ON_COMMAND);
					#endif
				}
				else if(isPoweringOff())
				{
					// TODO: request other boot to shut off
					utt.val[0][9] = 0;
					calibrationFlags = 0;
					#if(HW_VER == 30)
						bilateral_send_command(BILATERAL_POWER_OFF_COMMAND);
					#endif
				}
				#endif	//INCLUDE_UPROJ_DPEB42
				#endif	//(defined BOARD_TYPE_FLEXSEA_MANAGE)
			}
			calibrationFlagToRunOrIsRunning = calibrationFlags;

		#else
			//Unused variables:
			(void)procedure;
			(void)v;
			(void)co;
		#endif
	}

	return calibrationFlagToRunOrIsRunning;
}


#ifdef __cplusplus
}
#endif

/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'flexsea-system' System commands & functions
	Copyright (C) 2017 Dephy, Inc. <http://dephy.com/>

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
	[Lead developper] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors]
*****************************************************************************
	[This file] flexsea_cmd_tools: Tools used to support FlexSEA's
	developpment and usage.
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// Include(s)
//****************************************************************************

#include "../inc/flexsea_comm.h"
#include "main.h"
#include "../inc/flexsea_system.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

//Will change this, but for now the payloads will be stored in:
uint8_t tmp_payload_xmit[PAYLOAD_BUF_LEN];

//Comm Test:
uint8_t randomArrayTx[COMM_STR_BUF_LEN], randomArrayRx[COMM_STR_BUF_LEN];
uint8_t arrLen = 0;
int32_t sentPackets = 0, goodPackets = 0, badPackets = 0;

//****************************************************************************
// Function(s)
//****************************************************************************

//This gets called by flexsea_system's init_flexsea_payload_ptr(). Map all
//functions from this file to the array here. Failure to do so will send all
//commands to flexsea_payload_catchall().
void init_flexsea_payload_ptr_tools(void)
{
	//Comm. Test:
	flexsea_payload_ptr[CMD_COMM_TEST][RX_PTYPE_READ] = &rx_cmd_tools_comm_test_rw;
	//flexsea_payload_ptr[CMD_COMM_TEST][RX_PTYPE_WRITE] = &rx_cmd_tools_comm_test_w;
	flexsea_payload_ptr[CMD_COMM_TEST][RX_PTYPE_REPLY] = &rx_cmd_tools_comm_test_rr;
}

//Transmit Comm. Test:
//====================

//Test code? No
void tx_cmd_tools_comm_test_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t offset, uint8_t packetIndex)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_COMM_TEST;
	(*cmdType) = CMD_WRITE;

	//Data:
	shBuf[index++] = offset;
	shBuf[index++] = packetIndex;

	//We only use _w to send a reply. We send the received array:
	memcpy(&shBuf[index], randomArrayRx, arrLen);
	index += arrLen;

	//Payload length:
	(*len) = index;
}

//Test code? No
void tx_cmd_tools_comm_test_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t offset, uint8_t randomArrayLen)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_COMM_TEST;
	(*cmdType) = CMD_READ;

	//Data:
	shBuf[index++] = offset;

	if(offset == 0)
	{
		//Use this for stats and diagnostic
	}
	else
	{
		//Use this for the actual test:

		if(randomArrayLen <= PAYLOAD_BYTES)
		{
			arrLen = randomArrayLen;
		}
		else
		{
			//Use maximum:
			arrLen = PAYLOAD_BYTES;
		}

		//We save an array to a global for future comparison, and we copy it
		//to be sent:
		generateRandomUint8Array(randomArrayTx, arrLen);
		memcpy(&shBuf[index], randomArrayTx, arrLen);
		index += arrLen;
		sentPackets++;
	}

	//Payload length:
	(*len) = index;
}

//Receive Comm. Test:
//===================

/*
//Test code? No
void rx_cmd_tools_comm_test_w(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		pwro_output(buf[P_DATA1]);

	#else
		(void)buf;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}
*/

//Test code? No
void rx_cmd_tools_comm_test_rw(uint8_t *buf, uint8_t *info)
{
	(void)info;

	uint8_t offset = buf[P_DATA1];
	uint8_t packetIndex = buf[P_DATA1+1];

	tx_cmd_tools_comm_test_w(TX_N_DEFAULT, offset, packetIndex);
	packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, 0);
}

//Test code? No
void rx_cmd_tools_comm_test_rr(uint8_t *buf, uint8_t *info)
{
	int cmpResult = 0;

	(void)info;

	//Decode data:
	uint8_t offset = buf[P_DATA1];
	uint8_t packetIndex = buf[P_DATA1+1];

	//Save received array:
	memcpy(randomArrayRx, &buf[P_DATA1+2], arrLen);
	//Compare it to what we initially sent:
	cmpResult = memcmp(randomArrayRx, randomArrayTx, arrLen);
	cmpResult == 0? goodPackets++ : badPackets++;
}

#ifdef __cplusplus
}
#endif

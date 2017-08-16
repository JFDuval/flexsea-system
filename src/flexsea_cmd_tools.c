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

#include <string.h>
#include <flexsea.h>
#include <flexsea_system.h>
#include <flexsea_board.h>

//****************************************************************************
// Variable(s)
//****************************************************************************

//Comm Test:
uint8_t randomArray[MAX_PACKETS_BEHIND][COMM_STR_BUF_LEN];
uint8_t indexOfLastWritten = 255;
uint8_t randomArrayRx[COMM_STR_BUF_LEN];

uint8_t arrLen = 0, lastTxPacketIndex = 0, lastRxPacketIndex = 0;
int32_t sentPackets = 0, goodPackets = 0, badPackets = 0;
uint8_t packetOffset = 0;

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
						uint16_t *len, uint8_t offset, uint8_t packetNum)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_COMM_TEST;
	(*cmdType) = CMD_WRITE;

	//Data:
	shBuf[index++] = offset;
	shBuf[index++] = packetNum;
	shBuf[index++] = arrLen;

	if(offset == 0)
	{
		//...
	}
	else
	{
		//We only use _w to send a reply. We send the received array:
		memcpy(&shBuf[index], randomArrayRx, arrLen);
		index += arrLen;
	}

	//Payload length:
	(*len) = index;
}

//Test code? No
void tx_cmd_tools_comm_test_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t offset, uint8_t randomArrayLen, \
						uint8_t packetNum)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_COMM_TEST;
	(*cmdType) = CMD_READ;

	//Data:
	shBuf[index++] = offset;
	shBuf[index++] = packetNum;

	lastTxPacketIndex = packetNum;

	arrLen = (randomArrayLen <= PAYLOAD_BYTES) ? randomArrayLen : PAYLOAD_BYTES;
	shBuf[index++] = arrLen;

	if(offset == 0)
	{
		//Use this for stats and diagnostic
	}
	else
	{
		//Use this for the actual test:
		int indexToWriteTo = (indexOfLastWritten + 1) % MAX_PACKETS_BEHIND;
		
		#if(defined BOARD_TYPE_FLEXSEA_PLAN || defined TEST_PC)
		uint8_t* writeTo = &randomArray[indexToWriteTo][0];
		generateRandomUint8_tArray(writeTo, arrLen);
		memcpy(&shBuf[index], writeTo, arrLen);
		#endif	//(defined BOARD_TYPE_FLEXSEA_PLAN || defined TEST_PC)
		
		indexOfLastWritten = indexToWriteTo;
		//Generate and send new Tx array:
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
	//...
}
*/

//Test code? No
void rx_cmd_tools_comm_test_rw(uint8_t *buf, uint8_t *info)
{
	(void)info;

	uint8_t offset = buf[P_DATA1];
	uint8_t packetNum = buf[P_DATA1+1];
	arrLen = buf[P_DATA1+2];

	//Save received array:
	arrLen = (arrLen <= 48) ? arrLen : 48;
	memcpy(randomArrayRx, buf + (P_DATA1+3), arrLen);

	/*
	//Test code: corrupt 1 byte
	//randomArrayRx[0]++;
	//Test 2: corrupt 5%
	static uint8_t cnt = 0;
	cnt++;
	cnt %= 20;
	if(!cnt)
	{
		randomArrayRx[0]++;
	}
	*/
	/*
	//Test #3: skip 5% of responses:
	static uint8_t cnt = 0;
	cnt++;
	cnt %= 20;
	if(!cnt)
	{
		return;
	}
	*/

	tx_cmd_tools_comm_test_w(TX_N_DEFAULT, offset, packetNum);
	packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, SEND_TO_MASTER);
}

//Test code? No
void rx_cmd_tools_comm_test_rr(uint8_t *buf, uint8_t *info)
{
	int cmpResult = 0;
	uint8_t len = 0;

	(void)info;

	//Decode data:
	//uint8_t offset = buf[P_DATA1];
	uint8_t packetNum = buf[P_DATA1+1];
	len = buf[P_DATA1+2];

	lastRxPacketIndex = packetNum;
	packetOffset = (uint8_t)lastTxPacketIndex - (uint8_t)lastRxPacketIndex;

	#ifdef BOARD_TYPE_FLEXSEA_PLAN
	//Save received array:
	memcpy(randomArrayRx, &buf[P_DATA1+3], len);

	if(packetOffset >= MAX_PACKETS_BEHIND)
	{
		badPackets++;
		return;
	}

	int indexToRead = indexOfLastWritten >= packetOffset
			? (indexOfLastWritten - packetOffset)
			: (MAX_PACKETS_BEHIND + indexOfLastWritten - packetOffset);

	uint8_t* arrayToCompare = &randomArray[indexToRead][0];
	cmpResult = memcmp(randomArrayRx, arrayToCompare, len);
	cmpResult == 0 ? goodPackets++ : badPackets++;
	#endif
}

#ifdef __cplusplus
}
#endif

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
	[Lead developper] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors]
*****************************************************************************
	[This file] flexsea_cmd_stream: commands allowing plan to put execute into a streaming mode
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2017-03-13 | dudds4 | Initial GPL-3.0 release
****************************************************************************/

#include "flexsea_cmd_stream.h"
#include <flexsea.h>
#include "flexsea_system.h"
#include <flexsea_board.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t isStreaming = 0;

int streamCmds[MAX_STREAMS] = {-1 , -1 };
uint16_t streamPeriods[MAX_STREAMS] = {1, 1};
uint16_t streamReceivers[MAX_STREAMS] = {0, 0};
uint8_t streamPortInfos[MAX_STREAMS] = {PORT_NONE, PORT_NONE};
/*
int streamCmd = -1;
uint16_t streamPeriod = 1;
uint8_t streamReceiver = 0;
uint8_t streamPortInfo = PORT_USB;
*/
void tx_cmd_stream_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t cmdToStream, uint8_t periodInMS, uint8_t startStop)
{
	uint16_t index = 0;
	(*cmd) = CMD_STREAM;
	(*cmdType) = CMD_WRITE;

	shBuf[index++] = cmdToStream;
	shBuf[index++] = periodInMS;
	shBuf[index++] = startStop;

	*len = index;
}

void tx_cmd_stream_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len)
{
	uint16_t index = 0;
	(*cmd) = CMD_STREAM;
	(*cmdType) = CMD_READ;

	(void)shBuf;

	*len = index;
}

uint8_t isLegalStreamCmd(uint8_t cmd)
{
	//We have a couple system commands that are streamable
	//and we allow streaming for all user cmds?
	//These are basically all the commands available in slavecomm in Plan GUI

	return
		cmd == CMD_READ_ALL || cmd == CMD_IN_CONTROL || cmd == CMD_BATT ||
		(cmd > 99 && cmd < 127);
}

void rx_cmd_stream_w(uint8_t *buf, uint8_t *info)
{
	(void) info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
		uint16_t index = P_DATA1;

		uint8_t cmdToStream = buf[index++];
		uint8_t periodInMS = buf[index++];
		uint8_t startStop = buf[index++];

		//case: turn streaming on
		if(startStop && isLegalStreamCmd(cmdToStream) && isStreaming < MAX_STREAMS)
		{
			streamCmds[isStreaming] = cmdToStream;
			streamPeriods[isStreaming] = periodInMS;
			streamReceivers[isStreaming] = buf[P_XID];
			streamPortInfos[isStreaming] = *info;
			isStreaming++;

		}
		//case: turn streaming off
		else
		{
			int i;
			//get rid of the appropriate stream (find the index of the appropriate stream)
			for(i=0;i<isStreaming;i++)
			{
				if(streamCmds[i] == cmdToStream)
				{
					isStreaming--;
					break;
				}
			}
			
			//fail safe conditions
			//these conditions will be met if using an older version of plan
			if(i == isStreaming || cmdToStream == ((uint8_t)-1))
			{
				for(i=0;i<MAX_STREAMS;i++)
				{
					streamCmds[i] = -1;
					streamPeriods[i] = 12345;
					streamReceivers[i] = 0;
					streamPortInfos[i] = PORT_NONE;	
				}
				isStreaming = 0;
			}
			else
			{
				//shift other streams down (stream to delete gets overwritten)
				for(i=i; i < MAX_STREAMS-1; i++)
				{
					streamCmds[i] = streamCmds[i+1];
					streamPeriods[i] = streamPeriods[i+1];
					streamReceivers[i] = streamReceivers[i+1];
					streamPortInfos[i] = streamPortInfos[i+1];				
				}
				
				//set last 'stream' to null values
				streamCmds[i] = -1;
				streamPeriods[i] = 12345;
				streamReceivers[i] = 0;
				streamPortInfos[i] = PORT_NONE;		
			}
		}

	#else

		(void)buf;

	#endif	//BOARD_TY
}

void rx_cmd_stream_r(uint8_t *buf, uint8_t *info)
{
	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
//	tx_cmd_stream_w(TX_N_DEFAULT, current stream mode, whether currently streaming)
// 	packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, SEND_TO_MASTER);
	#endif

	(void) buf;
	(void) info;
}

void rx_cmd_stream_rr(uint8_t *buf, uint8_t *info)
{
	(void) buf;
	(void) info;

//	uint16_t index = P_DATA1;
//	uint8_t cmdToStream = buf[index++];
//	uint8_t periodInMS = buf[index++];
//	uint8_t startStop = buf[index++];

	// do stuff ?
}

void init_flexsea_payload_ptr_stream(void)
{
	flexsea_payload_ptr[CMD_STREAM][RX_PTYPE_READ] = &rx_cmd_stream_r;
	flexsea_payload_ptr[CMD_STREAM][RX_PTYPE_WRITE] = &rx_cmd_stream_w;
	flexsea_payload_ptr[CMD_STREAM][RX_PTYPE_REPLY] = &rx_cmd_stream_rr;
}


#ifdef __cplusplus
}
#endif

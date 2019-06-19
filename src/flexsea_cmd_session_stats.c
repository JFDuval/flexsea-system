/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'flexsea-dephy' Exo Controllers
	Copyright (C) 2018 Dephy, Inc. <http://dephy.com/>
*****************************************************************************
	[Lead developer] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors]
*****************************************************************************
	[This file] flexsea_cmd_session_stats: Read session stats from Rigid
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2019-06-12 | kandoni | Initial release
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// Include(s)
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <flexsea.h>
#include <flexsea_system.h>
#include "flexsea_user_structs.h"
#include "flexsea_cmd_user.h"
#include "flexsea_cmd_session_stats.h"

#if((defined BOARD_TYPE_FLEXSEA_EXECUTE) || (defined BOARD_TYPE_FLEXSEA_MANAGE))
	#include "session_tools.h"
#endif

//****************************************************************************
// Variable(s)
//****************************************************************************

struct sessionStats_s sessionStats;

//****************************************************************************
// Function(s)
//****************************************************************************

#ifdef BOARD_TYPE_FLEXSEA_PLAN
#include "flexsea_global_structs.h"
uint16_t getDuration(uint8_t session){return sessionStats.duration[session];}
uint32_t getEnergy(uint8_t session){return sessionStats.energy[session];}
uint8_t getStatus(uint8_t session){return sessionStats.status[session];}
#endif

void initSessionStats(void)
{
    uint8_t i = 0;
    for(i = 0; i < SESSIONS_SAVED; i++)
    {
        sessionStats.duration[i] = 0;
        sessionStats.energy[i] = 0;
        sessionStats.status[i] = 0;
    }
}

//****************************************************************************
// RX/TX Function(s)
//****************************************************************************

void init_flexsea_payload_ptr_session_stats(void)
{
    //Session Statistics
    flexsea_payload_ptr[CMD_SESSION_STATS][RX_PTYPE_READ] = &rx_cmd_session_stats_rw;
    flexsea_payload_ptr[CMD_SESSION_STATS][RX_PTYPE_WRITE] = &rx_cmd_session_stats_w;
    flexsea_payload_ptr[CMD_SESSION_STATS][RX_PTYPE_REPLY] = &rx_cmd_session_stats_rr;

    //Session Statistics - Multipacket
    flexsea_multipayload_ptr[CMD_SESSION_STATS][RX_PTYPE_READ] = &rx_multi_cmd_session_stats_rw;
    flexsea_multipayload_ptr[CMD_SESSION_STATS][RX_PTYPE_WRITE] = &rx_multi_cmd_session_stats_w;
    flexsea_multipayload_ptr[CMD_SESSION_STATS][RX_PTYPE_REPLY] = &rx_multi_cmd_session_stats_rr;
}

void tx_cmd_session_stats_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
					uint16_t *len, uint8_t sessionRequest)
{
	uint16_t index = 0;

	//Formatting:
	(*cmd) = CMD_SESSION_STATS;
	(*cmdType) = CMD_READ;

	//Data:
	shBuf[index++] = sessionRequest;

	//Payload length:
	(*len) = index;
    //(void)shBuf;
}

void tx_cmd_session_stats_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
					uint16_t *len, uint8_t sessionRequest, struct sessionStats_s *sStats)
{
	uint16_t index = 0;

	//Formatting:
	(*cmd) = CMD_SESSION_STATS;
	(*cmdType) = CMD_WRITE;

	//Data:
	shBuf[index++] = sessionRequest;

	//Arguments:
	if(sessionRequest & SESSION_STATS_REQUEST)
	{
	    uint8_t i = 0;
	    for(i = 0; i < SESSIONS_SAVED; i++)
        {
	        SPLIT_16(sStats->duration[i], shBuf, &index);
	        SPLIT_32(sStats->energy[i], shBuf, &index);
            shBuf[index++] = sStats->status[i];
        }

		//((2+4+1)*8 bytes)
	}

	//Payload length:
	(*len) = index;
}

void rx_cmd_session_stats_rw(uint8_t *buf, uint8_t *info)
{
	MultiPacketInfo mInfo;
	fillMultiInfoFromBuf(&mInfo, buf, info);
	mInfo.portOut = info[1];
	rx_multi_cmd_session_stats_rw( buf + P_DATA1, &mInfo, tmpPayload, &cmdLen );
	packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, SEND_TO_MASTER);
}

void rx_multi_cmd_session_stats_rw(uint8_t *msgBuf, MultiPacketInfo *mInfo, uint8_t *responseBuf, uint16_t* responseLen)
{
	uint16_t index = 0;
	(void)mInfo;

	//Temporary variables
	uint8_t sessionRequest = 0;

	//Decode data received:
	index = 0;
	sessionRequest = msgBuf[index++];

	#ifdef BOARD_TYPE_FLEXSEA_MANAGE

	    sessionFlags = sessionRequest;

    #else

		(void)index;
		//(void)offset;

	#endif //BOARD_TYPE_FLEXSEA_MANAGE

	//Reply:
	tx_cmd_session_stats_w(responseBuf, &cmdCode, &cmdType, responseLen, sessionRequest, &sessionStats);
}

void rx_cmd_session_stats_rr(uint8_t *buf, uint8_t *info)
{
	MultiPacketInfo mInfo;
	fillMultiInfoFromBuf(&mInfo, buf, info);
	mInfo.portOut = info[1];
	rx_multi_cmd_session_stats_rr( buf + P_DATA1, &mInfo, tmpPayload, &cmdLen );
}

#ifdef BOARD_TYPE_FLEXSEA_PLAN
#include <stdio.h>
#endif

void rx_multi_cmd_session_stats_rr(uint8_t *msgBuf, MultiPacketInfo *mInfo, uint8_t *responseBuf, uint16_t* responseLen)
{
	uint16_t index = 0;
	(void)mInfo;

	#ifdef BOARD_TYPE_FLEXSEA_PLAN

		struct sessionStats_s *sStats = &sessionStats;
		if( msgBuf[index++] & SESSION_STATS_REQUEST)
		{
		    uint8_t i = 0;
            for(i = 0; i < SESSIONS_SAVED; i++)
            {
                sStats->duration[i] = REBUILD_UINT16(msgBuf, &index);
                sStats->energy[i] = REBUILD_UINT32(msgBuf, &index);
                sStats->status[i] = msgBuf[index++];
            }

			//((2+4+1)*8 bytes)
		}

	#else

		(void)index;

	#endif //BOARD_TYPE_FLEXSEA_PLAN
}

void rx_cmd_session_stats_w(uint8_t *buf, uint8_t *info)
{
	MultiPacketInfo mInfo;
	fillMultiInfoFromBuf(&mInfo, buf, info);
	mInfo.portOut = info[1];
	rx_multi_cmd_session_stats_w( buf + P_DATA1, &mInfo, tmpPayload, &cmdLen );
}

void rx_multi_cmd_session_stats_w(uint8_t *msgBuf, MultiPacketInfo *mInfo, uint8_t *responseBuf, uint16_t* responseLen)
{
	uint16_t index = 0;
	(void)mInfo;

	#ifdef BOARD_TYPE_FLEXSEA_MANAGE

		sessionFlags |= msgBuf[index++];

		struct sessionStats_s *sStats = &sessionStats;
		if(sessionFlags & SESSION_STATS_REQUEST)
		{
		    uint8_t i = 0;
            for(i = 0; i < SESSIONS_SAVED; i++)
            {
                sStats->duration[i] = REBUILD_UINT16(msgBuf, &index);
                sStats->energy[i] = REBUILD_UINT32(msgBuf, &index);
                sStats->status[i] = msgBuf[index++];
            }

			//((2+4+1)*8 bytes)
		}

	#else

		(void)index;

	#endif //BOARD_TYPE_FLEXSEA_MANAGE
}

#ifdef __cplusplus
}
#endif




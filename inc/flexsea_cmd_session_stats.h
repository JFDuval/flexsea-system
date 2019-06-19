#ifndef INC_FLEXSEA_CMD_SESSION_STATS_H
#define INC_FLEXSEA_CMD_SESSION_STATS_H

#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// Include(s)
//****************************************************************************

#include <stdint.h>
#include "flexsea.h"

//****************************************************************************
// Definition(s):
//****************************************************************************
#define SESSIONS_SAVED  8

//****************************************************************************
// Structure(s)
//****************************************************************************

struct sessionStats_s
{
	uint16_t duration[SESSIONS_SAVED];
	uint32_t energy[SESSIONS_SAVED];
	uint8_t status[SESSIONS_SAVED];
};

struct _MultiPacketInfo_s;
typedef struct _MultiPacketInfo_s MultiPacketInfo;

//****************************************************************************
// Prototype(s):
//****************************************************************************

void initSessionStats(void);

#ifdef BOARD_TYPE_FLEXSEA_PLAN
uint16_t getDuration(uint8_t session);
uint32_t getEnergy(uint8_t session);
uint8_t getStatus(uint8_t session);
#endif

//****************************************************************************
// RX/TX Prototype(s):
//****************************************************************************

void init_flexsea_payload_ptr_session_stats(void);

void rx_cmd_session_stats_rw(uint8_t *buf, uint8_t *info);
void rx_cmd_session_stats_rr(uint8_t *buf, uint8_t *info);
void rx_cmd_session_stats_w(uint8_t *buf, uint8_t *info);

void rx_multi_cmd_session_stats_rw(uint8_t *msgBuf, MultiPacketInfo *mInfo, uint8_t *responseBuf, uint16_t* responseLen);
void rx_multi_cmd_session_stats_w(uint8_t *msgBuf, MultiPacketInfo *mInfo, uint8_t *responseBuf, uint16_t* responseLen);
void rx_multi_cmd_session_stats_rr(uint8_t *msgBuf, MultiPacketInfo *mInfo, uint8_t *responseBuf, uint16_t* responseLen);

void tx_cmd_session_stats_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
					uint16_t *len, uint8_t sessionRequest);
void tx_cmd_session_stats_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
                                        uint16_t *len, uint8_t sessionRequest, struct sessionStats_s *sStats);

//****************************************************************************
// Shared variable(s)
//****************************************************************************

extern struct sessionStats_s sessionStats;

#ifdef __cplusplus
}
#endif

#endif	//INC_FLEXSEA_CMD_SESSION_STATS_H

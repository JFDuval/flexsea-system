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
	[This file] flexsea_cmd_sysdata: commands for general transmission of system data to/from GUI
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2018-10-04 | dweisdorf | Initial GPL-3.0 release
	*
****************************************************************************/

#include "flexsea_cmd_sysdata.h"
#include "flexsea_system.h"
#include <flexsea.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initializes part of the array of function pointers which determines which
	function to call upon receiving a message
*/
void init_flexsea_payload_ptr_sysdata(void) {

	flexsea_multipayload_ptr[CMD_SYSDATA][RX_PTYPE_READ] = &rx_cmd_sysdata_r;
	flexsea_multipayload_ptr[CMD_SYSDATA][RX_PTYPE_WRITE] = &rx_cmd_sysdata_w;
	flexsea_multipayload_ptr[CMD_SYSDATA][RX_PTYPE_REPLY] = &rx_cmd_sysdata_rr;

}

// Flexsea General System Data Passing:

/* Called by master to send a message to the slave, attempting to initiate a read, using a bitmap to specify which fields to read.
 * Current implementation allows for 3 * 32 = 96 readable fields, using 96 / 8 = 12 bytes.
 * Slave will respond with the appropriate data.
*/

#define MAX_BYTES_OF_FLAGS 3
void tx_cmd_sysdata_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t *flags, uint8_t lenFlags) {

	uint8_t index = 0;
	(*cmd) = CMD_SYSDATA;
	(*cmdType) = CMD_READ;

	if(lenFlags > MAX_BYTES_OF_FLAGS)
		lenFlags=3;

	shBuf[index++] = lenFlags;

	uint8_t i=0;
	while((i) < lenFlags)
	{
		shBuf[index++] = flags[i++];
	}

	(*len) = index;
}

/* Slave calls this function automatically after receiving a read from master.
	It determines what to do with the information passed to it,
	And it replies indicating the resulting decision
*/
void rx_cmd_sysdata_r(uint8_t *msgBuf, uint8_t *info, uint8_t *responseBuf, uint16_t* responseLen) {

	uint8_t lenFlags;
	uint8_t flags[MAX_BYTES_OF_FLAGS];

	uint8_t index=0, i=0;
	lenFlags = msgBuf[index++];

	if(lenFlags > MAX_BYTES_OF_FLAGS)
		lenFlags=3;

	while(i < lenFlags)
	{
		msgBuf[index++] = flags[i++];
	}

	tx_cmd_sysdata_rr(responseBuf, responseLen, flags, lenFlags);

	(void)info;
}

/* Called by slave to send a read response to the master. Master will not respond
	TODO: implement
*/
void tx_cmd_sysdata_rr(uint8_t *responseBuf, uint16_t* responseLen, uint8_t *flags, uint8_t lenFlags) {

	// For now we're going to try sending more than 48 bytes
	// And it'll just be garbage data until I implement actual exhaustive list system data read :D
	// TODO: implement exhaustive list system data read

	uint8_t l = 0;
	// comm str length is 100
	uint8_t i;
	for(i=0;i<100;i++)
	{
		responseBuf[l++] = 'a' + (i%26);
	}

	*responseLen = l;

	(void) flags;
	(void) lenFlags;
}

/* Master calls this function automatically after receiving a response from slave
*/

void rx_cmd_sysdata_rr(uint8_t *msgBuf, uint8_t *info, uint8_t *responseBuf, uint16_t* responseLen) {
	(void) msgBuf;
	(void) info;
	(void) responseBuf;
	(void) responseLen;
}


/* Called by master to send a message to the slave, attempting to initiate a
	Slave will not respond.
	TODO: I don't think this is needed even??
	TODO: rename this to 'write'
*/
void tx_cmd_sysdata_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t *flags, uint8_t lenFlags) {

}


/* Slave calls this function automatically after receiving a write from master.
	It determines what to do with the information passed to it,
	And it does not reply.
*/
void rx_cmd_sysdata_w(uint8_t *msgBuf, uint8_t *info, uint8_t *responseBuf, uint16_t* responseLen) {

}

#ifdef __cplusplus
}
#endif

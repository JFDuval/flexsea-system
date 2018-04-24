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

#ifndef FLEXSEA_SYSTEM_INC_FLEXSEA_CMD_SYSDATA_H_
#define FLEXSEA_SYSTEM_INC_FLEXSEA_CMD_SYSDATA_H_

#include <stdint.h>
#include "flexsea_dataformats.h"


/* Flexsea Device C Interface defined here - FlexseaDeviceSpec */
typedef struct {
	char* name;
	uint8_t numFields;

	const char** fieldLabels;

	const uint8_t* fieldTypes;
	uint8_t** fieldPointers;
} FlexseaDeviceSpec;

/* Specs exist for the following devices */
#define NUM_DEVICE_TYPES 2
#define FX_NONE_SPEC 0
#define FX_RIGID_SPEC 1

/* this array contains all the device specs */
extern FlexseaDeviceSpec deviceSpecs[NUM_DEVICE_TYPES];

/* defines and externs needed by plan only */
#ifdef BOARD_TYPE_FLEXSEA_PLAN
	#define MAX_CONNECTED_DEVICES 10
	extern FlexseaDeviceSpec connectedDeviceSpecs[MAX_CONNECTED_DEVICES];
	extern uint8_t* deviceData[MAX_CONNECTED_DEVICES];
	extern uint8_t fx_spec_numConnectedDevices;

	// convenience define for accessing deviceData
	// only works on little endian systems
	// TODO: actually implement a big_endian/little_endian define so this isn't a bug waiting to happen
#ifdef BIG_ENDIAN
#else
	#define DEVICESPEC_TYPE_BY_IDX(i) ( *(deviceData[i]) )
	#define DEVICESPEC_UUID_BY_IDX(i) ( *((uint16_t*)(deviceData[i] + 1)) )
#endif
#endif //BOARD_TYPE_FLEXSEA_PLAN

/** FX_RIGID */
#define _rigid_numFields 8
extern const char* _rigid_fieldlabels[_rigid_numFields];
extern const uint8_t _rigid_field_formats[_rigid_numFields];

/* Related to max number of fields, should probably call it max num fields.. */
#define MAX_BYTES_OF_FLAGS 3

/* Functions for message passing declared below
 * */

/* Initializes part of the array of function pointers which determines which
	function to call upon receiving a message
*/
void init_flexsea_payload_ptr_sysdata(void);

// Flexsea General System Data Passing:

/* Called by master to send a message to the slave, attempting to initiate a read, using a bitmap to specify which fields to read.
 * Current implementation allows for 3 * 32 = 96 readable fields, using 96 / 8 = 12 bytes.
 * Slave will respond with the appropriate data.
*/

void tx_cmd_sysdata_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint32_t *flags, uint8_t lenFlags);

/* Called by master to send a message to the slave, attempting to inititiate a
	calibration procedure specified by 'calibrationMode'. Slave will not respond.
	TODO: rename this to 'write'
*/
void tx_cmd_sysdata_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t *flags, uint8_t lenFlags);

/* Master calls this function automatically after receiving a response from slave
*/
void rx_cmd_sysdata_rr(uint8_t *msgBuf, uint8_t *info, uint8_t *responseBuf, uint16_t* responseLen);

/* Slave calls this function automatically after receiving a read from master.
	It determines what to do with the information passed to it,
	And it replies indicating the resulting decision
*/
void rx_cmd_sysdata_r(uint8_t *msgBuf, uint8_t *info, uint8_t *responseBuf, uint16_t* responseLen);

/* Slave calls this function after receiving a read request from master. This sends the response to master.
*/
void tx_cmd_sysdata_rr(uint8_t *responseBuf, uint16_t* responseLen, uint32_t *flags, uint8_t lenFlags);

/* Slave calls this function automatically after receiving a write from master.
	It determines what to do with the information passed to it,
	And it does not reply.
*/
void rx_cmd_sysdata_w(uint8_t *msgBuf, uint8_t *info, uint8_t *responseBuf, uint16_t* responseLen);

#endif /* FLEXSEA_SYSTEM_INC_FLEXSEA_CMD_SYSDATA_H_ */

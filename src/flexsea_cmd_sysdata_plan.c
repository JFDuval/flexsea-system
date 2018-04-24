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
	* 2018-23-04 | dweisdorf | Initial GPL-3.0 release
	*
****************************************************************************/

#include "flexsea_cmd_sysdata.h"
#include "flexsea_system.h"
#include <flexsea.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IS_FIELD_HIGH(i, map) ( (map)[(i)/32] & (1 << ((i)%32)) )
#define SET_MAP_HIGH(i, map) ( (map)[(i)/32] |= (1 << ((i)%32)) )
#define SET_MAP_LOW(i, map) ( (map)[(i)/32] &= (~(1 << ((i)%32))) )

FlexseaDeviceSpec connectedDeviceSpecs[MAX_CONNECTED_DEVICES];
uint8_t numConnectedDevices = 0;
uint8_t* deviceData[MAX_CONNECTED_DEVICES];
uint8_t newDeviceConnecetedFlag;

void addConnectedDevice(uint8_t devType, uint16_t devId)
{
	if(numConnectedDevices == MAX_CONNECTED_DEVICES) return;

	FlexseaDeviceSpec *ds = &connectedDeviceSpecs[numConnectedDevices];
	if(devType == FX_RIGID_SPEC)
	{
		ds->numFields = _rigid_numFields;
		ds->fieldLabels = _rigid_fieldlabels;
		ds->fieldTypes = _rigid_field_formats;
	}

	uint16_t i, totalSpaceNeeded = 0;
	for(i=0; i<ds->numFields;i++)
		totalSpaceNeeded += FORMAT_SIZE_MAP[ds->fieldTypes[i]];

	if(deviceData[numConnectedDevices])
		realloc(deviceData[numConnectedDevices], totalSpaceNeeded);
	else
		deviceData[numConnectedDevices] = malloc(totalSpaceNeeded);

	memset(deviceData[numConnectedDevices], 0, totalSpaceNeeded);

	numConnectedDevices++;
	newDeviceConnecetedFlag=1;
}

void rx_cmd_sysdata_rr(uint8_t *msgBuf, uint8_t *info, uint8_t *responseBuf, uint16_t* responseLen) {

	uint16_t index=0;
	uint8_t lenFlags;
	uint32_t flags[MAX_BYTES_OF_FLAGS];

	lenFlags = msgBuf[index++];

	//read in our fields
	int i, j, fieldOffset;
	for(i=0;i<lenFlags;i++)
		flags[i]=REBUILD_UINT32(msgBuf, &index);

	// first two fields are always device type and id
	// if these fields are not sent then we can't do much with the response
	if(!IS_FIELD_HIGH(0, flags) || !IS_FIELD_HIGH(1, flags)) return;

	uint8_t devType = msgBuf[index++];
	uint16_t devId = REBUILD_UINT16(msgBuf, &index);

	// match this message to a connected device
	for(i = 0; i < numConnectedDevices; i++)
	{
		if(	devType == (deviceData[i][0]) &&
			devId 	== *(uint16_t*)(deviceData[i]+1))
					break;
	}

	//in the event we found no match and we have no more space to allocate we are screwed
	if(i >= MAX_CONNECTED_DEVICES)
		return;

	// in the event we found no connected device we need to add a new connected device
	if(i == numConnectedDevices)
		addConnectedDevice(devType, devId);

	// read into the appropriate device
	FlexseaDeviceSpec *ds = &connectedDeviceSpecs[i];
	fieldOffset = 3; //3 bytes are taken between devType and devId
	for(i = 0; i < ds->numFields; i++)
	{
		uint8_t fw = FORMAT_SIZE_MAP[ds->fieldTypes[i]];
		if(IS_FIELD_HIGH(i, flags))
		{
			for(j = 0; j < fw; j++)
				deviceData[i][fieldOffset + j] = msgBuf[index++];
		}

		fieldOffset += fw;
	}
}

#ifdef __cplusplus
}
#endif

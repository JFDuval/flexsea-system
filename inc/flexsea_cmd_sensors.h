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
	[This file] flexsea_cmd_sensors: commands specific sensors
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-09 | jfduval | Initial GPL-3.0 release
	*
****************************************************************************/

#ifndef INC_FLEXSEA_CMD_SENSORS_H
#define INC_FLEXSEA_CMD_SENSORS_H

//****************************************************************************
// Include(s)
//****************************************************************************

#include "flexsea_system.h"

//****************************************************************************
// Prototype(s):
//****************************************************************************

void rx_cmd_switch(uint8_t *buf);
void rx_cmd_encoder(uint8_t *buf);
void rx_cmd_strain(uint8_t *buf);

uint32_t tx_cmd_switch(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len);
uint32_t tx_cmd_encoder(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, int32_t enc);
uint32_t tx_cmd_strain(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len);

//****************************************************************************
// Definition(s):
//****************************************************************************

//****************************************************************************
// Structure(s):
//****************************************************************************


//****************************************************************************
// Shared variable(s)
//****************************************************************************


#endif	//INC_FLEXSEA_CMD_SENSORS_H

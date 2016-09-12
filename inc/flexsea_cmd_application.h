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
	[This file] flexsea_cmd_application: Application/User specific commands
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-09 | jfduval | Initial GPL-3.0 release
	*
****************************************************************************/

#ifndef INC_FLEXSEA_CMD_APPL_H
#define INC_FLEXSEA_CMD_APPL_H

//****************************************************************************
// Include(s)
//****************************************************************************

#include "flexsea_system.h"

//****************************************************************************
// Prototype(s):
//****************************************************************************

//TX functions are prototyped in flexsea_system

//Redefine weak functions from flexsea_payload:
void rx_cmd_special_1(uint8_t *buf);	//payload_30
void rx_cmd_special_2(uint8_t *buf);
void rx_cmd_special_3(uint8_t *buf);
void rx_cmd_special_4(uint8_t *buf);
void rx_cmd_special_5(uint8_t *buf);

//****************************************************************************
// Definition(s):
//****************************************************************************

//****************************************************************************
// Structure(s):
//****************************************************************************

struct spc4_s
{
	uint8_t ctrl_w;
	uint8_t ctrl;
	uint8_t encoder_w;
	int32_t encoder;
	int16_t current;
	int16_t open_spd;
};

//****************************************************************************
// Shared variable(s)
//****************************************************************************

extern struct spc4_s spc4_ex1, spc4_ex2;

#endif	//INC_FLEXSEA_CMD_APPL_H

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

void rx_cmd_special_1(uint8_t *buf);
void rx_cmd_special_2(uint8_t *buf);
void rx_cmd_special_3(uint8_t *buf);
void rx_cmd_special_4(uint8_t *buf);
void rx_cmd_special_5(uint8_t *buf);
void rx_cmd_ricnu(uint8_t *buf);

void rx_cmd_ricnu_rw(uint8_t *buf, uint8_t *info);
void rx_cmd_ricnu_w(uint8_t *buf, uint8_t *info);
void rx_cmd_ricnu_rr(uint8_t *buf, uint8_t *info);

uint32_t tx_cmd_ctrl_special_1(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
	uint8_t controller_w, uint8_t controller, uint8_t encoder_w, int32_t encoder, \
								int16_t current, int16_t open_spd);
uint32_t tx_cmd_ctrl_special_2(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
	int16_t z_k, int16_t z_b, int16_t z_i, uint8_t rgb, uint8_t clutch, \
								uint8_t trapeze, int32_t posi, int32_t posf, int32_t spdm, int32_t acc);
uint32_t tx_cmd_ctrl_special_3(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
	int16_t i_kp, int16_t i_ki, int16_t i_kd, int16_t w_curr);
uint32_t tx_cmd_ctrl_special_4(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
	uint8_t controller_w1, uint8_t controller1, uint8_t encoder_w1, int32_t encoder1, \
	int16_t current1, int16_t open_spd1, \
	uint8_t controller_w2, uint8_t controller2, uint8_t encoder_w2, int32_t encoder2, \
	int16_t current2, int16_t open_spd2);
uint32_t tx_cmd_ctrl_special_5(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
	uint8_t slave, uint8_t controller, int16_t ctrl_i, int16_t ctrl_o);


uint32_t tx_cmd_ricnu(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
						uint8_t offset, uint8_t controller, int32_t setpoint,
						uint8_t setGains, int16_t g0, int16_t g1, int16_t g2,
						int16_t g3);

void tx_cmd_ricnu_rw(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
					uint16_t *len, uint8_t offset, uint8_t controller, \
					int32_t setpoint, uint8_t setGains, int16_t g0, int16_t g1,\
					int16_t g2, int16_t g3);
void tx_cmd_ricnu_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
					uint16_t *len, uint8_t offset);
void tx_cmd_ricnu_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
					uint16_t *len, uint8_t offset);

//Decoding:
void rx_cmd_ricnu_Action1(uint8_t controller, int32_t setpoint, uint8_t setGains,
						int16_t g0,	int16_t g1,	int16_t g2, int16_t g3);

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

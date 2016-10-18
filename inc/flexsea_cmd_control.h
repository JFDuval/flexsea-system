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
	[This file] flexsea_cmd_control: commands specific to the motor & control
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-09 | jfduval | Initial GPL-3.0 release
	*
****************************************************************************/

#ifndef INC_FLEXSEA_CMD_CONTROL_H
#define INC_FLEXSEA_CMD_CONTROL_H

//****************************************************************************
// Include(s)
//****************************************************************************

#include "flexsea_system.h"

//****************************************************************************
// Prototype(s):
//****************************************************************************

void init_flexsea_payload_ptr_control(void);
	
//void rx_cmd_ctrl_mode(uint8_t *buf);
void rx_cmd_ctrl_mode_rr(uint8_t *buf, uint8_t *info);
void rx_cmd_ctrl_mode_rw(uint8_t *buf, uint8_t *info);
void rx_cmd_ctrl_mode_w(uint8_t *buf, uint8_t *info);
	
void rx_cmd_ctrl_i(uint8_t *buf);
void rx_cmd_ctrl_o(uint8_t *buf);
void rx_cmd_ctrl_p(uint8_t *buf);
void rx_cmd_ctrl_i_g(uint8_t *buf);
void rx_cmd_ctrl_p_g(uint8_t *buf);
void rx_cmd_ctrl_z_g(uint8_t *buf);
void rx_cmd_in_control(uint8_t *buf);

//uint32_t tx_cmd_ctrl_mode(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, int16_t ctrl);
void tx_cmd_ctrl_mode_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len);
void tx_cmd_ctrl_mode_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t ctrlMode);

uint32_t tx_cmd_ctrl_i(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, int16_t wanted, int16_t measured);
uint32_t tx_cmd_ctrl_o(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, int16_t open_spd);
uint32_t tx_cmd_ctrl_p(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
						int32_t pos, int32_t posi, int32_t posf, int32_t spdm, int32_t acc);
uint32_t tx_cmd_ctrl_i_g(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
						int16_t kp, int16_t ki, int16_t kd);
uint32_t tx_cmd_ctrl_p_g(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
						int16_t kp, int16_t ki, int16_t kd);
uint32_t tx_cmd_ctrl_z_g(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
						int16_t zk, int16_t zb, int16_t zi);
uint32_t tx_cmd_in_control(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, uint8_t select_w);

//****************************************************************************
// Definition(s):
//****************************************************************************

//List of controllers:
#define CTRL_NONE						0		//No PID gains, no PWM (no motor)
#define CTRL_OPEN						1		//Open loop controller. Use with CMD_CTRL_O_WRITE
#define CTRL_POSITION					2		//Position controller. Use with CMD_MOVE_TRAP_ABSOLUTE
#define CTRL_CURRENT					3		//Current controller. Use with CMD_CTRL_I_WRITE
#define CTRL_IMPEDANCE					4		//Impedance controller. Use with CMD_MOVE_TRAP_ABSOLUTE
#define CTRL_CUSTOM						5		//
//  (set gains with CMD_SET_Z_GAINS & CMD_CTRL_I_GAINS_WRITE)

//Nickname for the controller gains:
#define I_KP	g0
#define I_KI	g1
#define I_KD	g2
#define P_KP	g0
#define P_KI	g1
#define P_KD	g2
#define Z_K		g0
#define Z_B		g1
#define Z_I		g2

//In Control combined fields:
#define IN_CONTROL_CONTROLLER(x)	((x & 0xE000) >> 13)
#define IN_CONTROL_MOT_DIR(x)		((x & 0x1000) >> 12)
#define IN_CONTROL_PWM(x)			((x & 0x0FFF))

//****************************************************************************
// Structure(s):
//****************************************************************************

//Gains
struct gains_s
{
	 uint16_t g0, g1, g2, g3, g4, g5;
};

//Generic controller
struct gen_ctrl_s
{
	//Gains:
	struct gains_s gain;

	//Value wanted and setpoint value:
	int32_t actual_val;
	int32_t setpoint_val;
	int32_t actual_vel;

	//Errors:
	int32_t error;						//Current error
	int32_t error_prev;					//Past error
	int32_t error_sum;					//Integral
	int32_t error_dif;					//Differential

	//trapezoidal time
	int64_t trap_t;
};

//Position controller
struct pos_ctrl_s
{
	//Gains:
	struct gains_s gain;

	//Value wanted and setpoint value:
	int32_t pos;
	int32_t setp;
	int32_t posi;
	int32_t posf;
	int32_t spdm;
	int32_t acc;

	//Errors:
	int32_t error;						//Current error
	int32_t error_prev;					//Past error
	int32_t error_sum;					//Integral
	int32_t error_dif;					//Differential

	//trapezoidal time
	int64_t trap_t;
};

//Main data structure for all the controllers:
struct ctrl_s
{
	uint8_t active_ctrl;
	uint8_t pwm;						//ToDo needs to be more than 8bits!
	struct gen_ctrl_s generic;
	struct gen_ctrl_s current;
	struct pos_ctrl_s position;
	struct gen_ctrl_s impedance;
};

//Encoder:
struct enc_s
{
	int32_t count;
	int32_t count_last;
	int32_t count_dif;
	uint32_t config;
	int32_t vel;
};

//****************************************************************************
// Shared variable(s)
//****************************************************************************


#endif	//INC_FLEXSEA_CMD_CONTROL_H

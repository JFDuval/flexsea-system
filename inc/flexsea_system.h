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
	[This file] flexsea_system: configuration and functions for this
	particular system
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-09 | jfduval | Initial GPL-3.0 release
	*
****************************************************************************/

#ifndef INC_FLEXSEA_SYSTEM_H
#define INC_FLEXSEA_SYSTEM_H

//****************************************************************************
// Include(s)
//****************************************************************************

#include "flexsea_board.h"
#include "../../flexsea-comm/inc/flexsea.h"
//Include all your own files:
#include "flexsea_cmd_control.h"
#include "flexsea_cmd_sensors.h"
#include "flexsea_cmd_external.h"
#include "flexsea_cmd_application.h"
#include "flexsea_cmd_data.h"
//...

//****************************************************************************
// Prototype(s):
//****************************************************************************

void init_flexsea_payload_ptr(void);
uint16_t tx_cmd(uint8_t *payloadData, uint8_t cmdCode, uint8_t cmd_type, \
				uint32_t len, uint8_t receiver, uint8_t *buf);
__attribute__((weak)) void init_flexsea_payload_ptr_user(void);

//****************************************************************************
// Definition(s):
//****************************************************************************

//Nicknames for commands:
//=======================
//Important: command codes have to be from 0 to 127 (7bits, unsigned)

//System commands:

#define CMD_NULL						0
#define CMD_TEST						1
#define CMD_PING						10
#define CMD_STATUS						11
#define CMD_RESET						12
#define CMD_ACK							13

//Data commands:

#define CMD_MEM							20
#define CMD_ACQUI						21
#define CMD_READ_ALL					22
#define CMD_USER_DATA					23

//Sensor commands:

#define CMD_TEMP						40
#define CMD_SWITCH						41
#define CMD_IMU							42
#define CMD_ENCODER						43
#define CMD_STRAIN						44
#define CMD_STRAIN_CONFIG				45
#define CMD_VOLT						46
#define CMD_BATT						47

//Expansion/external commands:

#define CMD_POWER_OUT					60	//ToDo can we berge both?
#define CMD_PWRO						61
#define CMD_ADV_ANA_CONFIG				62
#define CMD_ANALOG						63
#define CMD_DIGITAL						64
#define CMD_DIGITAL_CONFIG				65
#define CMD_EXP_PERIPH_CONFIG			66

//Motor & Control commands:

#define CMD_CTRL_MODE					80
#define CMD_CTRL_X_G					81
#define CMD_CTRL_I_G					82
#define CMD_CTRL_P_G					83
#define CMD_CTRL_Z_G					84
#define CMD_CTRL_O						85
#define CMD_CTRL_I						86
#define CMD_CTRL_P						87
#define SHORTED_LEADS					88
#define CMD_IN_CONTROL					90

//Special commands:

#define CMD_SPC1						100     //ShuoBot Exo
#define CMD_SPC2						101		//CSEA Knee
#define CMD_SPC3						102		//Current controller tuning
#define CMD_READ_ALL_RICNU				105		//RIC/NU Knee, Read All function
#define CMD_SPC4						120		//Dual ShuoBot
#define CMD_SPC5						121		//Ankle 2-Dof Plan <> Manage
#define CMD_RICNU						122		//RIC/NU R/W

//===================

//Board types:
#define FLEXSEA_PLAN					1
#define FLEXSEA_MANAGE					2
#define FLEXSEA_EXECUTE					3
#define FLEXSEA_BATTERY					4
#define FLEXSEA_STRAIN					5
#define FLEXSEA_GOSSIP					6

//Board addresses:
#define FLEXSEA_DEFAULT					0
#define FLEXSEA_PLAN_BASE			 	10						//Plan: from 10 to 19
#define FLEXSEA_PLAN_1					(FLEXSEA_PLAN_BASE + 0)
#define FLEXSEA_MANAGE_BASE				20						//Manage: from 20 to 39
#define FLEXSEA_MANAGE_1				(FLEXSEA_MANAGE_BASE + 0)
#define FLEXSEA_MANAGE_2				(FLEXSEA_MANAGE_BASE + 1)
#define FLEXSEA_EXECUTE_BASE			40						//Execute: from 40 to 59
#define FLEXSEA_EXECUTE_1				(FLEXSEA_EXECUTE_BASE + 0)
#define FLEXSEA_EXECUTE_2				(FLEXSEA_EXECUTE_BASE + 1)
#define FLEXSEA_EXECUTE_3				(FLEXSEA_EXECUTE_BASE + 2)
#define FLEXSEA_EXECUTE_4				(FLEXSEA_EXECUTE_BASE + 3)
#define FLEXSEA_BATTERY_BASE			60						//Battery: from 60 to 69
#define FLEXSEA_BATTERY_1				(FLEXSEA_BATTERY_BASE + 0)
#define FLEXSEA_STRAIN_BASE				70						//Strain: from 70 to 79
#define FLEXSEA_STRAIN_1				(FLEXSEA_STRAIN_BASE + 0)
#define FLEXSEA_GOSSIP_BASE				80						//Gossip: from 80 to 89
#define FLEXSEA_GOSSIP_1				(FLEXSEA_GOSSIP_BASE + 0)
#define FLEXSEA_GOSSIP_2				(FLEXSEA_GOSSIP_BASE + 1)
#define FLEXSEA_VIRTUAL_BASE			100						//Virtual boards from 100 to N
#define FLEXSEA_VIRTUAL_1				(FLEXSEA_VIRTUAL_BASE + 0)
#define FLEXSEA_VIRTUAL_2				(FLEXSEA_VIRTUAL_BASE + 1)

//Software error (SE) codes. Values will be ORed
#define SE_DEFAULT						0
#define SE_INVALID_SPI					2
#define SE_INIT_SPI						4
#define SE_INVALID_DIO_AF				8
#define SE_INVALID_USART				16
#define SE_INIT_USART					32
#define SE_SEND_SERIAL_MASTER			64
#define SE_INVALID_SLAVE				128
#define SE_RECEIVE_FROM_MASTER			256
#define SE_CMD_NOT_PROGRAMMED			512
#define SE_INVALID_READ_TYPE			1024
#define SE_WRONG_PARSING				2048

//Motor type:
#define MOTOR_BRUSHED					0
#define MOTOR_BRUSHLESS					1

//Generic:
#define DISABLED						0
#define ENABLED							1

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
#define IN_CONTROL_CONTROLLER(x)		((x & 0xE000) >> 13)
#define IN_CONTROL_MOT_DIR(x)			((x & 0x1000) >> 12)
#define IN_CONTROL_PWM(x)				((x & 0x0FFF))

//****************************************************************************
// Structure(s):
//****************************************************************************

//****************************************************************************
// Shared variable(s)
//****************************************************************************

//Structures and shared variables are now in:
#include "flexsea_global_structs.h"

#endif	//INC_FLEXSEA_SYSTEM_H

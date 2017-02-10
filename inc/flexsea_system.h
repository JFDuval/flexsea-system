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

#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// Include(s)
//****************************************************************************

#include "flexsea_board.h"
#include "../flexsea-comm/inc/flexsea.h"
//Include the core flexsea-system files:
#include "flexsea_global_structs.h"
#include "flexsea_cmd_control.h"
#include "flexsea_cmd_sensors.h"
#include "flexsea_cmd_external.h"
#include "flexsea_cmd_data.h"
#include "flexsea_cmd_tools.h"
//#include <fm_block_allocator.h>
//Include the user files:
#include "../flexsea-user/inc/flexsea_cmd_user.h"

//****************************************************************************
// Prototype(s):
//****************************************************************************

void init_flexsea_payload_ptr(void);
uint16_t tx_cmd(uint8_t *payloadData, uint8_t cmdCode, uint8_t cmd_type, \
				uint32_t len, uint8_t receiver, uint8_t *buf);
void pack(uint8_t *shBuf, uint8_t cmd, uint8_t cmdType, uint16_t len, \
			uint8_t rid, uint8_t *info, uint16_t *numBytes, uint8_t *commStr);
void packAndSend(uint8_t *shBuf, uint8_t cmd, uint8_t cmdType, uint16_t len, \
				 uint8_t rid, uint8_t *info, uint8_t ms);
void executePtrXid(struct execute_s **myPtr, uint8_t p_xid);
void managePtrXid(struct manage_s **myPtr, uint8_t p_xid);
void gossipPtrXid(struct gossip_s **myPtr, uint8_t p_xid);
void strainPtrXid(struct strain_s **myPtr, uint8_t p_xid);
void flexsea_payload_catchall(uint8_t *buf, uint8_t *info);
//__attribute__((weak)) void init_flexsea_payload_ptr_user(void);

//****************************************************************************
// Definition(s):
//****************************************************************************

//Nicknames for commands:
//=======================
//Important: command codes have to be from 0 to 127 (7bits, unsigned)

//System commands:

#define CMD_NULL				0
#define CMD_TEST				1
#define CMD_COMM_TEST			2
#define CMD_PING				10
#define CMD_STATUS				11
#define CMD_RESET				12
#define CMD_ACK					13

//Data commands:

#define CMD_MEM					20
#define CMD_ACQUI				21
#define CMD_READ_ALL			22
#define CMD_USER_DATA			23

//Sensor commands:

#define CMD_TEMP				40
#define CMD_SWITCH				41
#define CMD_IMU					42
#define CMD_ENCODER				43
#define CMD_STRAIN				44
#define CMD_STRAIN_CONFIG		45
#define CMD_VOLT				46

//Expansion/external commands:

#define CMD_PWRO				61
#define CMD_ADV_ANA_CONFIG		62
#define CMD_ANALOG				63
#define CMD_DIGITAL				64
#define CMD_DIGITAL_CONFIG		65
#define CMD_EXP_PERIPH_CONFIG	66
#define CMD_BATT				67

//Motor & Control commands:

#define CMD_CTRL_MODE			80
#define CMD_CTRL_X_G			81
#define CMD_CTRL_I_G			82
#define CMD_CTRL_P_G			83
#define CMD_CTRL_Z_G			84
#define CMD_CTRL_O				85
#define CMD_CTRL_I				86
#define CMD_CTRL_P				87
#define SHORTED_LEADS			88
#define CMD_IN_CONTROL			90
#define CMD_CALIBRATION_MODE	91

//User commands (100-127):

#define CMD_USER1				100
#define CMD_USER2				101
#define CMD_USER3				102
#define CMD_USER4				103
#define CMD_USER5				104
//(feel free to expand and/or redefine in flexsea-user, as long as it stays in
// the 100-127 range)

//===================

//Board types:
#define FLEXSEA_PLAN			1
#define FLEXSEA_MANAGE			2
#define FLEXSEA_EXECUTE			3
#define FLEXSEA_BATTERY			4
#define FLEXSEA_STRAIN			5
#define FLEXSEA_GOSSIP			6

//Board addresses:
#define FLEXSEA_DEFAULT			0
#define FLEXSEA_PLAN_BASE		10						//Plan: from 10 to 19
#define FLEXSEA_PLAN_1			(FLEXSEA_PLAN_BASE + 0)
#define FLEXSEA_MANAGE_BASE		20						//Manage: from 20 to 39
#define FLEXSEA_MANAGE_1		(FLEXSEA_MANAGE_BASE + 0)
#define FLEXSEA_MANAGE_2		(FLEXSEA_MANAGE_BASE + 1)
#define FLEXSEA_EXECUTE_BASE	40						//Execute: from 40 to 59
#define FLEXSEA_EXECUTE_1		(FLEXSEA_EXECUTE_BASE + 0)
#define FLEXSEA_EXECUTE_2		(FLEXSEA_EXECUTE_BASE + 1)
#define FLEXSEA_EXECUTE_3		(FLEXSEA_EXECUTE_BASE + 2)
#define FLEXSEA_EXECUTE_4		(FLEXSEA_EXECUTE_BASE + 3)
#define FLEXSEA_BATTERY_BASE	60						//Battery: from 60 to 69
#define FLEXSEA_BATTERY_1		(FLEXSEA_BATTERY_BASE + 0)
#define FLEXSEA_STRAIN_BASE		70						//Strain: from 70 to 79
#define FLEXSEA_STRAIN_1		(FLEXSEA_STRAIN_BASE + 0)
#define FLEXSEA_GOSSIP_BASE		80						//Gossip: from 80 to 89
#define FLEXSEA_GOSSIP_1		(FLEXSEA_GOSSIP_BASE + 0)
#define FLEXSEA_GOSSIP_2		(FLEXSEA_GOSSIP_BASE + 1)
#define FLEXSEA_VIRTUAL_BASE	100						//Virtual boards from 100 to N
#define FLEXSEA_VIRTUAL_1		(FLEXSEA_VIRTUAL_BASE + 0)
#define FLEXSEA_VIRTUAL_2		(FLEXSEA_VIRTUAL_BASE + 1)
//ToDo: The "Virtual" board idea was ill-conceived. Is it used? Can we get rid of it?

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
#define CTRL_MEASRES                    6       //send a constant current through a winding to measure resistance

//List of calibration procedures
#define CALIBRATION_FIND_POLES 				0x01
#define CALIBRATION_FIND_CURRENT_ZEROES 	0x02

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

//Pack and Send:
#define SEND_TO_SLAVE					0
#define SEND_TO_MASTER					1

//****************************************************************************
// Macro(s):
//****************************************************************************

//To simplify user's life, here are two macros:
#define TX_N_DEFAULT		tmpPayload,&cmdCode,&cmdType,&cmdLen
#define P_AND_S_DEFAULT		tmpPayload,cmdCode,cmdType,cmdLen

//****************************************************************************
// Shared variable(s)
//****************************************************************************

//We use this buffer to exchange information between tx_N() and tx_cmd():
extern uint8_t tmpPayload[PAYLOAD_BUF_LEN];	//tx_N() => tx_cmd()
//Similarly, we exchange command code, type and length:
extern uint8_t cmdCode, cmdType;
extern uint16_t cmdLen;
//extern MsgQueue packet_queue;

//Structures and shared variables are now in:
#include "flexsea_global_structs.h"

#ifdef __cplusplus
}
#endif

#endif	//INC_FLEXSEA_SYSTEM_H

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
	[This file] flexsea_cmd_control_1: commands specific to the motor &
	control loops. Code split in 2 files, see flexsea_cmd_control_2.c
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-09 | jfduval | Initial GPL-3.0 release
	* 2016-11-07 | jfduval | Renamed *_1, updated to new convention
****************************************************************************/

/*Note: flexsea_cmd_control.c was getting too long so it was split in two
files. *_1 has all the setpoints, plus the In Control command. *_2 has the
Set Gains functions.
*/

#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// Include(s)
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include "../inc/flexsea_system.h"
#include "../inc/flexsea_cmd_control.h"

//Execute boards only:
#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
#include "main.h"
#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

//****************************************************************************
// Variable(s)
//****************************************************************************

//Will change this, but for now the payloads will be stored in: (ToDo eliminate soon)
uint8_t tmp_payload_xmit[PAYLOAD_BUF_LEN];
struct execute_s *exPtr1 = &exec1;

//****************************************************************************
// Function(s)
//****************************************************************************

//This gets called by flexsea_system's init_flexsea_payload_ptr(). Map all
//functions from this file to the array here. Failure to do so will send all
//commands to flexsea_payload_catchall().
void init_flexsea_payload_ptr_control_1(void)
{
	//Control mode:
	flexsea_payload_ptr[CMD_CTRL_MODE][RX_PTYPE_READ] = &rx_cmd_ctrl_mode_rw;
	flexsea_payload_ptr[CMD_CTRL_MODE][RX_PTYPE_WRITE] = &rx_cmd_ctrl_mode_w;
	flexsea_payload_ptr[CMD_CTRL_MODE][RX_PTYPE_REPLY] = &rx_cmd_ctrl_mode_rr;

	//Open - Setpoint:
	flexsea_payload_ptr[CMD_CTRL_O][RX_PTYPE_READ] = &rx_cmd_ctrl_o_rw;
	flexsea_payload_ptr[CMD_CTRL_O][RX_PTYPE_WRITE] = &rx_cmd_ctrl_o_w;
	flexsea_payload_ptr[CMD_CTRL_O][RX_PTYPE_REPLY] = &rx_cmd_ctrl_o_rr;

	//Position - Setpoint:
	flexsea_payload_ptr[CMD_CTRL_P][RX_PTYPE_READ] = &rx_cmd_ctrl_p_rw;
	flexsea_payload_ptr[CMD_CTRL_P][RX_PTYPE_WRITE] = &rx_cmd_ctrl_p_w;
	flexsea_payload_ptr[CMD_CTRL_P][RX_PTYPE_REPLY] = &rx_cmd_ctrl_p_rr;

	//Current - Setpoint:
	flexsea_payload_ptr[CMD_CTRL_I][RX_PTYPE_READ] = &rx_cmd_ctrl_i_rw;
	flexsea_payload_ptr[CMD_CTRL_I][RX_PTYPE_WRITE] = &rx_cmd_ctrl_i_w;
	flexsea_payload_ptr[CMD_CTRL_I][RX_PTYPE_REPLY] = &rx_cmd_ctrl_i_rr;

	//Misc:
//	flexsea_payload_ptr[CMD_IN_CONTROL][RX_PTYPE_READ] = &rx_cmd_in_control;
}

//Transmit Control Mode:
//======================

//Test code? Yes
void tx_cmd_ctrl_mode_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t ctrlMode)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CTRL_MODE;
	(*cmdType) = CMD_WRITE;

	//Data:
	shBuf[index++] = ctrlMode;

	//Payload length:
	(*len) = index;
}

//Test code? Yes
void tx_cmd_ctrl_mode_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CTRL_MODE;
	(*cmdType) = CMD_READ;

	//Data:
	(void)shBuf; //(none)

	//Payload length:
	(*len) = index;
}

//Receive Control Mode:
//======================

//Test code? No
void rx_cmd_ctrl_mode_w(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		control_strategy(buf[P_DATA1]);

	#else
		(void)buf;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_ctrl_mode_rw(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		tx_cmd_ctrl_mode_w(TX_N_DEFAULT, ctrl.active_ctrl);
		packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, 0);

	#else
		(void)buf;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_ctrl_mode_rr(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

		//Decode data:
		uint8_t controller = buf[P_DATA1];

		//Store value:
		executePtrXid(&exPtr1, buf[P_XID]);
		exPtr1->ctrl.active_ctrl = controller;

	#else
		(void)buf;
	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))
}

//Transmit Control Current Setpoint:
//==================================

//Test code? Yes
void tx_cmd_ctrl_i_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, int16_t currentSetpoint)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CTRL_I;
	(*cmdType) = CMD_WRITE;

	//Data:
	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
		//Execute: reply only
		(void)currentSetpoint;
		SPLIT_16((uint16_t)ctrl.current.actual_val, shBuf, &index);
		SPLIT_16((uint16_t)ctrl.current.setpoint_val, shBuf, &index);
	#else
		//Other boards can write a new setpoint
		SPLIT_16((uint16_t)0, shBuf, &index);
		SPLIT_16((uint16_t)currentSetpoint, shBuf, &index);
	#endif

	//Payload length:
	(*len) = index;
}

//Test code? Yes
void tx_cmd_ctrl_i_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CTRL_I;
	(*cmdType) = CMD_READ;

	//Data:
	(void)shBuf; //(none)

	//Payload length:
	(*len) = index;
}

//Receive Control Current Setpoint:
//=================================

//Test code? No
void rx_cmd_ctrl_i_w(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	int16_t tmp_wanted_current = 0, tmp_measured_current = 0;

	tmp_measured_current = (int16_t) REBUILD_UINT16(buf, &index);
	tmp_wanted_current = (int16_t) REBUILD_UINT16(buf, &index);

	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Only change the setpoint if we are in current control mode:
		if(ctrl.active_ctrl == CTRL_CURRENT)
		{
			ctrl.current.setpoint_val = tmp_wanted_current;
		}

	#else

		(void)buf;

	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_ctrl_i_rw(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		tx_cmd_ctrl_i_w(TX_N_DEFAULT, exec1.ctrl.current.setpoint_val);
		packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, 0);

	#else
		(void)buf;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_ctrl_i_rr(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	int16_t tmp_wanted_current = 0, tmp_measured_current = 0;

	tmp_measured_current = (int16_t) REBUILD_UINT16(buf, &index);
	tmp_wanted_current = (int16_t) REBUILD_UINT16(buf, &index);

	(void)info;

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	//Store value:
	executePtrXid(&exPtr1, buf[P_XID]);
	exPtr1->current = tmp_measured_current;

	#else
		(void)buf;
	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))
}

//Transmit Control Open Setpoint:
//===============================

//Test code? Yes
void tx_cmd_ctrl_o_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, int16_t setpoint)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CTRL_O;
	(*cmdType) = CMD_WRITE;

	//Data:
	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
		//Execute: reply only
		(void)setpoint;
		SPLIT_16((uint16_t)exec1.ctrl.pwm, shBuf, &index);
		//ToDo: not sure that this field is updated
	#else
		//Other boards can write a new setpoint
		SPLIT_16((uint16_t)setpoint, shBuf, &index);
	#endif

	//Payload length:
	(*len) = index;
}

//Test code? Yes
void tx_cmd_ctrl_o_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CTRL_O;
	(*cmdType) = CMD_READ;

	//Data:
	(void)shBuf; //(none)

	//Payload length:
	(*len) = index;
}

//Receive Control Open Setpoint:
//==============================

//Test code? No
void rx_cmd_ctrl_o_w(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	int16_t tmp = 0;

	tmp = (int16_t) REBUILD_UINT16(buf, &index);

	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Only change the setpoint if we are in open control mode:
		if(ctrl.active_ctrl == CTRL_OPEN)
		{
			motor_open_speed_1(tmp);
		}

	#else

		(void)buf;

	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_ctrl_o_rw(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		tx_cmd_ctrl_o_w(TX_N_DEFAULT, exec1.ctrl.pwm);
		packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, 0);

	#else
		(void)buf;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_ctrl_o_rr(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	int16_t tmp = 0;

	tmp = (int16_t) REBUILD_UINT16(buf, &index);

	(void)info;

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

		//Store value:
		executePtrXid(&exPtr1, buf[P_XID]);
		exPtr1->ctrl.pwm = tmp;
		//ToDo shouldn't be exec1!

	#else
		(void)buf;
	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))
}

//Transmit Control Position Setpoint:
//===================================

//Test code? Yes
void tx_cmd_ctrl_p_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, int32_t pos, int32_t posi, int32_t posf,\
						int32_t spdm, int32_t acc)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CTRL_P;
	(*cmdType) = CMD_WRITE;

	//Data:
	SPLIT_32((uint32_t)pos, shBuf, &index);
	SPLIT_32((uint32_t)posi, shBuf, &index);
	SPLIT_32((uint32_t)posf, shBuf, &index);
	SPLIT_32((uint32_t)spdm, shBuf, &index);
	SPLIT_32((uint32_t)acc, shBuf, &index);

	//Payload length:
	(*len) = index;
}

//Test code? Yes
void tx_cmd_ctrl_p_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_CTRL_P;
	(*cmdType) = CMD_READ;

	//Data:
	(void)shBuf; //(none)

	//Payload length:
	(*len) = index;
}

//Receive Control Position Setpoint:
//==================================

//Test code? No
void rx_cmd_ctrl_p_w(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	int32_t tmp_pos = 0, tmp_posi = 0, tmp_posf = 0, tmp_spdm = 0, tmp_acc = 0;

	(void)info;	//Unused for now

	//Decode data:
	tmp_pos = (int32_t) REBUILD_UINT32(buf, &index);
	tmp_posi = (int32_t) REBUILD_UINT32(buf, &index);
	tmp_posf = (int32_t) REBUILD_UINT32(buf, &index);
	tmp_spdm = (int32_t) REBUILD_UINT32(buf, &index);
	tmp_acc = (int32_t) REBUILD_UINT32(buf, &index);

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		int16_t tmp_z_k = 0, tmp_z_b = 0, tmp_z_i = 0;

		ctrl.position.posf = tmp_posf;
		ctrl.position.spdm = tmp_spdm;
		ctrl.position.acc = tmp_acc;

		if(ctrl.active_ctrl == CTRL_POSITION)
		{
			ctrl.position.posi = ctrl.position.setp;
			steps = trapez_gen_motion_1(ctrl.position.posi, ctrl.position.posf,\
										ctrl.position.spdm, \
										ctrl.position.acc = tmp_acc);
		}
		else if(ctrl.active_ctrl == CTRL_IMPEDANCE)
		{
			//Backup gains
			tmp_z_k = ctrl.impedance.gain.Z_K;
			tmp_z_b = ctrl.impedance.gain.Z_B;
			tmp_z_i = ctrl.impedance.gain.Z_I;

			//Zero them
			ctrl.impedance.gain.Z_K = 0;
			ctrl.impedance.gain.Z_B = 0;
			ctrl.impedance.gain.Z_I = 0;

			//New trajectory
			ctrl.position.posi = ctrl.impedance.setpoint_val;
			steps = trapez_gen_motion_1(ctrl.position.posi, ctrl.position.posf,\
										ctrl.position.spdm, \
										ctrl.position.acc = tmp_acc);

			//Restore gains
			ctrl.impedance.gain.Z_K = tmp_z_k;
			ctrl.impedance.gain.Z_B = tmp_z_b;
			ctrl.impedance.gain.Z_I = tmp_z_i;
		}

	#else

		(void)buf;

	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_ctrl_p_rw(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		tx_cmd_ctrl_p_w(TX_N_DEFAULT, ctrl.position.pos, ctrl.position.posi, \
						ctrl.position.posf, ctrl.position.spdm, \
						ctrl.position.acc);
		packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, 0);

	#else
		(void)buf;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_ctrl_p_rr(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	int32_t tmp_pos = 0, tmp_posi = 0, tmp_posf = 0, tmp_spdm = 0, tmp_acc = 0;

	(void)info;	//Unused for now

	//Decode data:
	tmp_pos = (int32_t) REBUILD_UINT32(buf, &index);
	tmp_posi = (int32_t) REBUILD_UINT32(buf, &index);
	tmp_posf = (int32_t) REBUILD_UINT32(buf, &index);
	tmp_spdm = (int32_t) REBUILD_UINT32(buf, &index);
	tmp_acc = (int32_t) REBUILD_UINT32(buf, &index);

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

		//Store value:
		executePtrXid(&exPtr1, buf[P_XID]);
		exPtr1->ctrl.position.pos = tmp_pos;
		exPtr1->ctrl.position.posi = tmp_posi;
		exPtr1->ctrl.position.posf = tmp_posf;
		exPtr1->ctrl.position.spdm = tmp_spdm;
		exPtr1->ctrl.position.acc = tmp_acc;
		//ToDo: shouldn't be exec1!

	#else
		(void)buf;
	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))
}

//Transmit In Control Command:
//============================

void tx_cmd_in_control_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t select_w)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_IN_CONTROL;
	(*cmdType) = CMD_WRITE;

	//Data:
	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		shBuf[index++] = select_w;	//Parameter written
		SPLIT_32((uint32_t)in_control.w[select_w], shBuf, &index);
		SPLIT_32((uint32_t)in_control.setp, shBuf, &index);
		SPLIT_32((uint32_t)in_control.actual_val, shBuf, &index);
		SPLIT_32((uint32_t)in_control.error, shBuf, &index);
		SPLIT_16((uint16_t)in_control.output, shBuf, &index);

		//Combine 3 fields in 1 uint16:
		in_control_get_pwm_dir();
		in_control_combine();
		SPLIT_16((uint16_t)in_control.combined, shBuf, &index);

		SPLIT_16((uint16_t)ctrl.current.actual_val, shBuf, &index);

		//User fields:
		SPLIT_32((uint32_t)in_control.r[0], shBuf, &index);
		SPLIT_32((uint32_t)in_control.r[1], shBuf, &index);

	#endif //BOARD_TYPE_FLEXSEA_EXECUTE

	#ifdef BOARD_TYPE_FLEXSEA_PLAN

		shBuf[index++] = select_w;	//Parameter written
		SPLIT_32((uint32_t)in_control_1.w[select_w], shBuf, &index);

	#endif  //BOARD_TYPE_FLEXSEA_PLAN

	//Payload length:
	(*len) = index;
}

void tx_cmd_in_control_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_IN_CONTROL;
	(*cmdType) = CMD_READ;

	//Data:
	(void)shBuf; //(none)

	//Payload length:
	(*len) = index;
}

//Receive In Control Command:
//===========================

//Test code? No
void rx_cmd_in_control_w(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	int32_t w_val = 0;
	uint8_t w_select = 0;

	//Decode data:
	w_select =  buf[index++];
	w_val = (int32_t) REBUILD_UINT32(buf, &index);

	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		in_control.w[w_select] = w_val;

	#else

		(void)buf;

	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_in_control_rw(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		tx_cmd_in_control_w(TX_N_DEFAULT, 0);
		packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, 0);

	#else
		(void)buf;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_in_control_rr(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	int32_t w_val = 0;
	uint8_t w_select = 0;

	//Decode data:
	w_select =  buf[index++];
	w_val = (int32_t) REBUILD_UINT32(buf, &index);

	(void)info;

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	//ToDo: change in_control to be part of execPtr
		//Store value:

		in_control_1.setp = (int32_t) REBUILD_UINT32(buf, &index);
		in_control_1.actual_val = (int32_t) REBUILD_UINT32(buf, &index);

		in_control_1.error = (int32_t) REBUILD_UINT32(buf, &index);
		in_control_1.output = (int16_t) REBUILD_UINT16(buf, &index);

		in_control_1.combined = REBUILD_UINT16(buf, &index);
		in_control_1.current = (int16_t) REBUILD_UINT16(buf, &index);

		in_control_1.r[0] = (int32_t) REBUILD_UINT32(buf, &index);
		in_control_1.r[1] = (int32_t) REBUILD_UINT32(buf, &index);

		in_control_1.controller = IN_CONTROL_CONTROLLER(in_control_1.combined);
		in_control_1.mot_dir = IN_CONTROL_MOT_DIR(in_control_1.combined);
		in_control_1.pwm = IN_CONTROL_PWM(in_control_1.combined);

	#else
		(void)buf;
	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))
}

#ifdef __cplusplus
}
#endif

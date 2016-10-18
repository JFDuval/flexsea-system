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

#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// Include(s)
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include "../inc/flexsea_system.h"
#include "../inc/flexsea_cmd_application.h"
#include "main.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

//Will change this, but for now the payloads will be stored in:
unsigned char tmp_payload_xmit[PAYLOAD_BUF_LEN];

//Special code for Manage:
struct spc4_s spc4_ex1, spc4_ex2;
//ToDo useful?

//****************************************************************************
// Function(s)
//****************************************************************************

//Transmission of a CTRL_SPECIAL_1 command: ShuoBot Exo
//Arguments are only for data that the user will change at runtime.
//controller_w (Write New Controller): KEEP/CHANGE
//controller (New controller): ignored if ctrl_w == KEEP
//encoder_w (Write New Encoder value): KEEP/CHANGE
//encoder_cnt (New encoder count): ignored if encoder_w == KEEP
//current: current controller setpoint
uint32_t tx_cmd_ctrl_special_1(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								uint8_t controller_w, uint8_t controller, uint8_t encoder_w, int32_t encoder, \
								int16_t current, int16_t open_spd)
{
	uint8_t tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
	uint32_t bytes = 0;

	//Fresh payload string:
	prepare_empty_payload(board_id, receiver, buf, len);

	//Command:
	buf[P_CMDS] = 1;                     //1 command in string

	if(cmd_type == CMD_READ)
	{
		//In that case Read also includes a bunch of writing. We keep the Read keyword as
		//it will get us a reply.

		buf[P_CMD1] = CMD_R(CMD_SPC1);

		//Arguments:
		buf[P_DATA1] = controller_w;
		buf[P_DATA1 + 1] = controller;
		uint16_to_bytes((uint16_t)current, &tmp0, &tmp1);
		buf[P_DATA1 + 2] = tmp0;
		buf[P_DATA1 + 3] = tmp1;
		buf[P_DATA1 + 4] = encoder_w;
		uint32_to_bytes((uint32_t)encoder, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 5] = tmp0;
		buf[P_DATA1 + 6] = tmp1;
		buf[P_DATA1 + 7] = tmp2;
		buf[P_DATA1 + 8] = tmp3;
		uint16_to_bytes((uint16_t)open_spd, &tmp0, &tmp1);
		buf[P_DATA1 + 9] = tmp0;
		buf[P_DATA1 + 10] = tmp1;

		bytes = P_DATA1 + 11;     //Bytes is always last+1
	}
	else if(cmd_type == CMD_WRITE)
	{
		//In that case Write is only used for the Reply

		buf[P_CMD1] = CMD_W(CMD_SPC1);

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Arguments:
		uint16_to_bytes((uint16_t)imu.gyro.x, &tmp0, &tmp1);
		buf[P_DATA1] = tmp0;
		buf[P_DATA1 + 1] = tmp1;
		uint16_to_bytes((uint16_t)imu.gyro.y, &tmp0, &tmp1);
		buf[P_DATA1 + 2] = tmp0;
		buf[P_DATA1 + 3] = tmp1;
		uint16_to_bytes((uint16_t)imu.gyro.z, &tmp0, &tmp1);
		buf[P_DATA1 + 4] = tmp0;
		buf[P_DATA1 + 5] = tmp1;

		uint16_to_bytes(strain_read(), &tmp0, &tmp1);
		buf[P_DATA1 + 6] = tmp0;
		buf[P_DATA1 + 7] = tmp1;

		uint16_to_bytes(read_analog(0), &tmp0, &tmp1);
		buf[P_DATA1 + 8] = tmp0;
		buf[P_DATA1 + 9] = tmp1;

		//uint32_to_bytes((uint32_t)encoder_read(), &tmp0, &tmp1, &tmp2, &tmp3);
		uint32_to_bytes((uint32_t)refresh_enc_control(), &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 10] = tmp0;
		buf[P_DATA1 + 11] = tmp1;
		buf[P_DATA1 + 12] = tmp2;
		buf[P_DATA1 + 13] = tmp3;

		uint16_to_bytes((uint16_t)ctrl.current.actual_val, &tmp0, &tmp1);
		buf[P_DATA1 + 14] = tmp0;
		buf[P_DATA1 + 15] = tmp1;

		buf[P_DATA1 + 16] = safety_cop.status1;
		buf[P_DATA1 + 17] = safety_cop.status2;

		bytes = P_DATA1 + 18;     //Bytes is always last+1

		#else

		bytes = 0;

		#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
	}
	else
	{
		//Invalid
		flexsea_error(SE_INVALID_READ_TYPE);
		bytes = 0;
	}

	return bytes;
}

//Reception of a CMD_SPECIAL_1 command
void rx_cmd_special_1(uint8_t *buf)
{
	uint32_t numb = 0;
	int16_t tmp_wanted_current = 0, tmp_open_spd  = 0;
	int32_t tmp_enc  = 0;

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	//Structure pointer. Points to exec1 by default.
	struct execute_s *exec_s_ptr;

	//Point to the appropriate structure:
	if(buf[P_XID] == FLEXSEA_EXECUTE_1)
	{
		exec_s_ptr = &exec1;
	}
	else if(buf[P_XID] == FLEXSEA_EXECUTE_2)
	{
		exec_s_ptr = &exec2;
	}
	else if(buf[P_XID] == FLEXSEA_EXECUTE_3)
	{
		exec_s_ptr = &exec3;
	}
	else if(buf[P_XID] == FLEXSEA_EXECUTE_4)
	{
		exec_s_ptr = &exec4;
	}

	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	if(IS_CMD_RW(buf[P_CMD1]) == READ)
	{
		//Received a Read command from our master.

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Decode its data:
		//===============

		//Controller:
		if(buf[P_DATA1] == CHANGE)
		{
			//User wants to change the type of controller:
			//control_strategy(buf[P_DATA1]);	//Disabled, change it manually
		}

		//Only change the setpoint if we are in current control mode:
		if(ctrl.active_ctrl == CTRL_CURRENT)
		{
			tmp_wanted_current = BYTES_TO_UINT16(buf[P_DATA1 + 2], buf[P_DATA1 + 3]);
			ctrl.current.setpoint_val = tmp_wanted_current;
		}
		else if(ctrl.active_ctrl == CTRL_OPEN)
		{
			tmp_open_spd = BYTES_TO_UINT16(buf[P_DATA1 + 9], buf[P_DATA1 + 10]);
			motor_open_speed_1(tmp_open_spd);
		}

		//Encoder:
		if(buf[P_DATA1 + 4] == CHANGE)
		{
			//User wants to overwrite the encoder:
			int32 tmp_enc = (int32)BYTES_TO_UINT32(buf[P_DATA1 + 5], buf[P_DATA1 + 6], \
													buf[P_DATA1 + 7], buf[P_DATA1 + 8]);
			qei_write(tmp_enc);
		}

		//Generate the reply:
		//===================

		numb = tx_cmd_ctrl_special_1(buf[P_XID], CMD_WRITE, tmp_payload_xmit, \
									PAYLOAD_BUF_LEN, KEEP, 0, KEEP, 0, 0, 0);
		numb = comm_gen_str(tmp_payload_xmit, comm_str_485_1, numb);
		numb = COMM_STR_BUF_LEN;	//Fixed length for now to accomodate the DMA


		//(for now, send it)
		//rs485_puts(comm_str_485_1, (numb));
		rs485_reply_ready(comm_str_485_1, (numb));	//Delayed response

		#ifdef USE_USB
		usb_puts(comm_str_485_1, (numb));
		#endif

		#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE
		//No code (yet), you shouldn't be here...
		flexsea_error(SE_CMD_NOT_PROGRAMMED);
		#endif	//BOARD_TYPE_FLEXSEA_MANAGE

		#ifdef BOARD_TYPE_FLEXSEA_PLAN
		//No code (yet), you shouldn't be here...
		flexsea_error(SE_CMD_NOT_PROGRAMMED);
		#endif	//BOARD_TYPE_FLEXSEA_PLAN
	}
	else if(IS_CMD_RW(buf[P_CMD1]) == WRITE)
	{
		//Two options: from Master of from slave (a read reply)

		if(sent_from_a_slave(buf))
		{
			//We received a reply to our read request

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

			#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

			//Store values:

			exec_s_ptr->gyro.x = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+0], buf[P_DATA1+1]));
			exec_s_ptr->gyro.y = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+2], buf[P_DATA1+3]));
			exec_s_ptr->gyro.z = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+4], buf[P_DATA1+5]));

			exec_s_ptr->strain = (BYTES_TO_UINT16(buf[P_DATA1+6], buf[P_DATA1+7]));
			exec_s_ptr->analog[0] = (BYTES_TO_UINT16(buf[P_DATA1+8], buf[P_DATA1+9]));

			exec_s_ptr->enc_display = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1+10], buf[P_DATA1+11], \
										buf[P_DATA1+12], buf[P_DATA1+13]));

			exec_s_ptr->current = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+14], buf[P_DATA1+15]));

			exec_s_ptr->status1 = buf[P_DATA1+16];
			exec_s_ptr->status2 = buf[P_DATA1+17];

			#ifdef MULTIPLE_COMMANDS
			//To interface with Python:
			printf("[%i,%i,%i,%i,%i,%i,%i]\n", exec1.encoder, exec1.current, exec1.gyro.x, exec1.gyro.y, exec1.gyro.z, \
					exec1.strain, exec1.analog[0]);
			#endif

			#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

			#ifdef BOARD_TYPE_FLEXSEA_PLAN

			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
		else
		{
			//Master is writing a value to this board

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

			//ToDo call relevant functions ****

			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

			#ifdef BOARD_TYPE_FLEXSEA_MANAGE
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_MANAGE

			#ifdef BOARD_TYPE_FLEXSEA_PLAN
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
	}
}

//Transmission of a CTRL_SPECIAL_2 command: CSEA Knee
//Arguments are only for data that the user will change at runtime.
//'trapeze' can be KEEP/CHANGE.
uint32_t tx_cmd_ctrl_special_2(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								int16_t z_k, int16_t z_b, int16_t z_i, uint8_t rgb, uint8_t clutch, \
								uint8_t trapeze, int32_t posi, int32_t posf, int32_t spdm, int32_t acc)
{
	uint8_t tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
	uint32_t bytes = 0;

	//Fresh payload string:
	prepare_empty_payload(board_id, receiver, buf, len);

	//Command:
	buf[P_CMDS] = 1;                     //1 command in string

	if(cmd_type == CMD_READ)
	{
		//In that case Read also includes a bunch of writing. We keep the Read keyword as
		//it will get us a reply.

		buf[P_CMD1] = CMD_R(CMD_SPC2);

		//Arguments:
		uint16_to_bytes((uint16_t)z_k, &tmp0, &tmp1);
		buf[P_DATA1 + 0] = tmp0;
		buf[P_DATA1 + 1] = tmp1;
		uint16_to_bytes((uint16_t)z_b, &tmp0, &tmp1);
		buf[P_DATA1 + 2] = tmp0;
		buf[P_DATA1 + 3] = tmp1;
		uint16_to_bytes((uint16_t)z_i, &tmp0, &tmp1);
		buf[P_DATA1 + 4] = tmp0;
		buf[P_DATA1 + 5] = tmp1;
		buf[P_DATA1 + 6] = rgb;
		buf[P_DATA1 + 7] = clutch;
		uint32_to_bytes((uint32_t)posi, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 8] = tmp0;
		buf[P_DATA1 + 9] = tmp1;
		buf[P_DATA1 + 10] = tmp2;
		buf[P_DATA1 + 11] = tmp3;
		uint32_to_bytes((uint32_t)posf, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 12] = tmp0;
		buf[P_DATA1 + 13] = tmp1;
		buf[P_DATA1 + 14] = tmp2;
		buf[P_DATA1 + 15] = tmp3;
		uint32_to_bytes((uint32_t)spdm, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 16] = tmp0;
		buf[P_DATA1 + 17] = tmp1;
		buf[P_DATA1 + 18] = tmp2;
		buf[P_DATA1 + 19] = tmp3;
		uint32_to_bytes((uint32_t)acc, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 20] = tmp0;
		buf[P_DATA1 + 21] = tmp1;
		buf[P_DATA1 + 22] = tmp2;
		buf[P_DATA1 + 23] = tmp3;
		buf[P_DATA1 + 24] = trapeze;

		bytes = P_DATA1 + 25;     //Bytes is always last+1
	}
	else if(cmd_type == CMD_WRITE)
	{
		//In that case Write is only used for the Reply

		buf[P_CMD1] = CMD_W(CMD_SPC2);

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Arguments:
		uint16_to_bytes((uint16_t)imu.gyro.x, &tmp0, &tmp1);
		buf[P_DATA1] = tmp0;
		buf[P_DATA1 + 1] = tmp1;
		uint16_to_bytes((uint16_t)imu.gyro.y, &tmp0, &tmp1);
		buf[P_DATA1 + 2] = tmp0;
		buf[P_DATA1 + 3] = tmp1;
		uint16_to_bytes((uint16_t)imu.gyro.z, &tmp0, &tmp1);
		buf[P_DATA1 + 4] = tmp0;
		buf[P_DATA1 + 5] = tmp1;

		uint16_to_bytes((uint16_t)read_analog(0), &tmp0, &tmp1);	//Angle
		buf[P_DATA1 + 6] = tmp0;
		buf[P_DATA1 + 7] = tmp1;

		uint16_to_bytes((uint16_t)read_analog(1), &tmp0, &tmp1);	//Force
		buf[P_DATA1 + 8] = tmp0;
		buf[P_DATA1 + 9] = tmp1;

		uint32_to_bytes((uint32_t)qei_read(), &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 10] = tmp0;
		buf[P_DATA1 + 11] = tmp1;
		buf[P_DATA1 + 12] = tmp2;
		buf[P_DATA1 + 13] = tmp3;

		uint16_to_bytes((uint16_t)ctrl.current.actual_val, &tmp0, &tmp1);
		buf[P_DATA1 + 14] = tmp0;
		buf[P_DATA1 + 15] = tmp1;

		bytes = P_DATA1 + 16;     //Bytes is always last+1

		#else

		bytes = 0;

		#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
	}
	else
	{
		//Invalid
		flexsea_error(SE_INVALID_READ_TYPE);
		bytes = 0;
	}

	return bytes;
}

//Reception of a CMD_SPECIAL_2 command
void rx_cmd_special_2(uint8_t *buf)
{
	uint32_t numb = 0;
	int16_t tmp_z_k = 0, tmp_z_b = 0, tmp_z_i = 0, tmp_current = 0;
	int32_t tmp_posi  = 0, tmp_posf  = 0, tmp_spdm  = 0, tmp_acc  = 0;

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	//Structure pointer. Points to exec1 by default.
	struct execute_s *exec_s_ptr = &exec1;

	//Point to the appropriate structure:
	if(buf[P_XID] == FLEXSEA_EXECUTE_1)
	{
		exec_s_ptr = &exec1;
	}
	else if(buf[P_XID] == FLEXSEA_EXECUTE_2)
	{
		exec_s_ptr = &exec2;
	}

	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	if(IS_CMD_RW(buf[P_CMD1]) == READ)
	{
		//Received a Read command from our master.

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Decode its data:
		//===============

		//Impedance gains:
		if(ctrl.active_ctrl == CTRL_IMPEDANCE)
		{
			tmp_z_k = BYTES_TO_UINT16(buf[P_DATA1 + 0], buf[P_DATA1 + 1]);
			ctrl.impedance.gain.Z_K = tmp_z_k;
			tmp_z_b = BYTES_TO_UINT16(buf[P_DATA1 + 2], buf[P_DATA1 + 3]);
			ctrl.impedance.gain.Z_B = tmp_z_b;
			tmp_z_i = BYTES_TO_UINT16(buf[P_DATA1 + 4], buf[P_DATA1 + 5]);
			ctrl.impedance.gain.Z_I = tmp_z_i;
		}

		//Clutch:
		pwro_output(buf[P_DATA1 + 7]);

		//MinM RGB:
		minm_rgb_color = buf[P_DATA1 + 6]; //ToDo set value that will be used in the next cycle

		//Trapeze:
		if(buf[P_DATA1 + 24] == CHANGE)
		{
			//Time to update the trajectory

			tmp_posi = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1 + 8], buf[P_DATA1 + 9], buf[P_DATA1 + 10], buf[P_DATA1 + 11]));
			tmp_posf = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1 + 12], buf[P_DATA1 + 13], buf[P_DATA1 + 14], buf[P_DATA1 + 15]));
			tmp_spdm = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1 + 16], buf[P_DATA1 + 17], buf[P_DATA1 + 18], buf[P_DATA1 + 19]));
			tmp_acc = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1 + 20], buf[P_DATA1 + 21], buf[P_DATA1 + 22], buf[P_DATA1 + 23]));

			if(ctrl.active_ctrl == CTRL_IMPEDANCE)
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
				steps = trapez_gen_motion_1(tmp_posi, tmp_posf, tmp_spdm, tmp_acc);

				//Restore gains
				ctrl.impedance.gain.Z_K = tmp_z_k;
				ctrl.impedance.gain.Z_B = tmp_z_b;
				ctrl.impedance.gain.Z_I = tmp_z_i;
			}
		}

		//Generate the reply:
		//===================

		numb = tx_cmd_ctrl_special_2(buf[P_XID], CMD_WRITE, tmp_payload_xmit, \
									PAYLOAD_BUF_LEN, 0, 0, 0, 0, 0, \
									KEEP, 0, 0, 0, 0);	//ToDo confirm
		numb = comm_gen_str(tmp_payload_xmit, comm_str_485_1, numb);
		numb = COMM_STR_BUF_LEN;	//Fixed length for now to accomodate the DMA

		//(for now, send it)
		rs485_puts(comm_str_485_1, (numb));

		#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE
		//No code (yet), you shouldn't be here...
		flexsea_error(SE_CMD_NOT_PROGRAMMED);
		#endif	//BOARD_TYPE_FLEXSEA_MANAGE

		#ifdef BOARD_TYPE_FLEXSEA_PLAN
		//No code (yet), you shouldn't be here...
		flexsea_error(SE_CMD_NOT_PROGRAMMED);
		#endif	//BOARD_TYPE_FLEXSEA_PLAN
	}
	else if(IS_CMD_RW(buf[P_CMD1]) == WRITE)
	{
		//Two options: from Master of from slave (a read reply)

		if(sent_from_a_slave(buf))
		{
			//We received a reply to our read request

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

			#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

			//Store values:

			exec_s_ptr->gyro.x = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+0], buf[P_DATA1+1]));
			exec_s_ptr->gyro.y = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+2], buf[P_DATA1+3]));
			exec_s_ptr->gyro.z = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+4], buf[P_DATA1+5]));

			exec_s_ptr->analog[0] = (BYTES_TO_UINT16(buf[P_DATA1+6], buf[P_DATA1+7]));
			//printf("A0: %i.\n", (BYTES_TO_UINT16(buf[P_DATA1+6], buf[P_DATA1+7])));	//ToDo remove (debug only)
			exec_s_ptr->analog[1] = (BYTES_TO_UINT16(buf[P_DATA1+8], buf[P_DATA1+9]));
			//printf("A1: %i.\n", (BYTES_TO_UINT16(buf[P_DATA1+8], buf[P_DATA1+9])));		//ToDo remove (debug only)

			exec_s_ptr->enc_display = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1+10], buf[P_DATA1+11], \
										buf[P_DATA1+12], buf[P_DATA1+13]));

			exec_s_ptr->current = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+14], buf[P_DATA1+15]));

			#endif	//BOARD_TYPE_FLEXSEA_MANAGE

			#ifdef BOARD_TYPE_FLEXSEA_PLAN

			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
		else
		{
			//Master is writing a value to this board

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

			//ToDo call relevant functions ****

			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

			#ifdef BOARD_TYPE_FLEXSEA_MANAGE
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_MANAGE

			#ifdef BOARD_TYPE_FLEXSEA_PLAN
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
	}
}

//Transmission of a CTRL_SPECIAL_3 command: Current controller tuning
//Arguments are only for data that the user will change at runtime.
uint32_t tx_cmd_ctrl_special_3(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								int16_t i_kp, int16_t i_ki, int16_t i_kd, int16_t w_curr)
{
	uint8_t tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
	uint32_t bytes = 0;

	//Fresh payload string:
	prepare_empty_payload(board_id, receiver, buf, len);

	//Command:
	buf[P_CMDS] = 1;                     //1 command in string

	if(cmd_type == CMD_READ)
	{
		//In that case Read also includes a bunch of writing. We keep the Read keyword as
		//it will get us a reply.

		buf[P_CMD1] = CMD_R(CMD_SPC3);

		//Arguments:
		uint16_to_bytes((uint16_t)i_kp, &tmp0, &tmp1);
		buf[P_DATA1 + 0] = tmp0;
		buf[P_DATA1 + 1] = tmp1;
		uint16_to_bytes((uint16_t)i_ki, &tmp0, &tmp1);
		buf[P_DATA1 + 2] = tmp0;
		buf[P_DATA1 + 3] = tmp1;
		uint16_to_bytes((uint16_t)i_kd, &tmp0, &tmp1);
		buf[P_DATA1 + 4] = tmp0;
		buf[P_DATA1 + 5] = tmp1;
		uint16_to_bytes((uint16_t)w_curr, &tmp0, &tmp1);
		buf[P_DATA1 + 6] = tmp0;
		buf[P_DATA1 + 7] = tmp1;


		bytes = P_DATA1 + 8;     //Bytes is always last+1
	}
	else if(cmd_type == CMD_WRITE)
	{
		//In that case Write is only used for the Reply

		buf[P_CMD1] = CMD_W(CMD_SPC3);

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Arguments:
		uint16_to_bytes((uint16_t)ctrl.current.actual_val, &tmp0, &tmp1);
		buf[P_DATA1 + 0] = tmp0;
		buf[P_DATA1 + 1] = tmp1;
		uint32_to_bytes((uint32_t)ctrl.current.error, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 2] = tmp0;
		buf[P_DATA1 + 3] = tmp1;
		buf[P_DATA1 + 4] = tmp2;
		buf[P_DATA1 + 5] = tmp3;
		uint32_to_bytes((uint32_t)ctrl.current.error_sum, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 6] = tmp0;
		buf[P_DATA1 + 7] = tmp1;
		buf[P_DATA1 + 8] = tmp2;
		buf[P_DATA1 + 9] = tmp3;
		uint16_to_bytes((uint16_t)ctrl.pwm, &tmp0, &tmp1);	//ToDo add to structure
		buf[P_DATA1 + 10] = tmp0;
		buf[P_DATA1 + 11] = tmp1;

		bytes = P_DATA1 + 12;     //Bytes is always last+1

		#else

		bytes = 0;

		#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
	}
	else
	{
		//Invalid
		flexsea_error(SE_INVALID_READ_TYPE);
		bytes = 0;
	}

	return bytes;
}

//Reception of a CMD_SPECIAL_3 command
void rx_cmd_special_3(uint8_t *buf)
{
	uint32_t numb = 0;
	int16_t tmp_kp = 0, tmp_ki = 0, tmp_kd = 0, tmp_current = 0;

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	//Structure pointer. Points to exec1 by default.
	struct execute_s *exec_s_ptr = &exec1;

	//Point to the appropriate structure:
	if(buf[P_XID] == FLEXSEA_EXECUTE_1)
	{
		exec_s_ptr = &exec1;
	}
	else if(buf[P_XID] == FLEXSEA_EXECUTE_2)
	{
		exec_s_ptr = &exec2;
	}

	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	if(IS_CMD_RW(buf[P_CMD1]) == READ)
	{
		//Received a Read command from our master.

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Decode its data:
		//===============

		//Current gains:
		if(ctrl.active_ctrl == CTRL_CURRENT)
		{
			//Gains:
			tmp_kp = BYTES_TO_UINT16(buf[P_DATA1 + 0], buf[P_DATA1 + 1]);
			ctrl.current.gain.I_KP = tmp_kp;
			tmp_ki = BYTES_TO_UINT16(buf[P_DATA1 + 2], buf[P_DATA1 + 3]);
			ctrl.current.gain.I_KI = tmp_ki;
			tmp_kd = BYTES_TO_UINT16(buf[P_DATA1 + 4], buf[P_DATA1 + 5]);
			ctrl.current.gain.I_KD = tmp_kd;

			//Setpoint:
			tmp_current = (int16_t)BYTES_TO_UINT16(buf[P_DATA1 + 6], buf[P_DATA1 + 7]);
			ctrl.current.setpoint_val = tmp_current;
		}

		//Generate the reply:
		//===================

		numb = tx_cmd_ctrl_special_3(buf[P_XID], CMD_WRITE, tmp_payload_xmit, \
									PAYLOAD_BUF_LEN, 0, 0, 0, 0);
		numb = comm_gen_str(tmp_payload_xmit, comm_str_485_1, numb);
		numb = COMM_STR_BUF_LEN;	//Fixed length for now to accomodate the DMA

		//(for now, send it)
		rs485_puts(comm_str_485_1, numb);

		#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE
		//No code (yet), you shouldn't be here...
		flexsea_error(SE_CMD_NOT_PROGRAMMED);
		#endif	//BOARD_TYPE_FLEXSEA_MANAGE

		#ifdef BOARD_TYPE_FLEXSEA_PLAN
		//No code (yet), you shouldn't be here...
		flexsea_error(SE_CMD_NOT_PROGRAMMED);
		#endif	//BOARD_TYPE_FLEXSEA_PLAN
	}
	else if(IS_CMD_RW(buf[P_CMD1]) == WRITE)
	{
		//Two options: from Master of from slave (a read reply)

		if(sent_from_a_slave(buf))
		{
			//We received a reply to our read request

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

			#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

			//Store values:

			exec_s_ptr->current = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+0], buf[P_DATA1+1]));

			//ToDo decode and store errors

			#endif	//BOARD_TYPE_FLEXSEA_MANAGE

			#ifdef BOARD_TYPE_FLEXSEA_PLAN

			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
		else
		{
			//Master is writing a value to this board

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

			//ToDo call relevant functions ****

			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

			#ifdef BOARD_TYPE_FLEXSEA_MANAGE
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_MANAGE

			#ifdef BOARD_TYPE_FLEXSEA_PLAN
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
	}
}

//=============

//Transmission of a CTRL_SPECIAL_4 command: Plan <> Manage, Dual ShuoBot Exo
//Special4 works in pair with Special1
//Arguments are only for data that the user will change at runtime.
//controller_w (Write New Controller): KEEP/CHANGE
//controller (New controller): ignored if ctrl_w == KEEP
//encoder_w (Write New Encoder value): KEEP/CHANGE
//encoder_cnt (New encoder count): ignored if encoder_w == KEEP
//current: current controller setpoint
uint32_t tx_cmd_ctrl_special_4(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								uint8_t controller_w1, uint8_t controller1, uint8_t encoder_w1, int32_t encoder1, \
								int16_t current1, int16_t open_spd1, \
								uint8_t controller_w2, uint8_t controller2, uint8_t encoder_w2, int32_t encoder2, \
								int16_t current2, int16_t open_spd2)
{
	uint8_t tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
	uint32_t bytes = 0;

	#if(defined BOARD_TYPE_FLEXSEA_MANAGE)

	//Structure pointer. Points to exec1 by default.
	struct execute_s *exec_s_ptr = &exec1;

	#endif	//(defined BOARD_TYPE_FLEXSEA_MANAGE)

	//Fresh payload string:
	prepare_empty_payload(board_id, receiver, buf, len);

	//Command:
	buf[P_CMDS] = 1;		//1 command in string

	if(cmd_type == CMD_READ)
	{
		//In that case Read also includes a bunch of writing. We keep the Read keyword as
		//it will get us a reply.

		buf[P_CMD1] = CMD_R(CMD_SPC4);

		//Arguments:
		buf[P_DATA1 + 0] = controller_w1;
		buf[P_DATA1 + 1] = controller1;
		uint16_to_bytes((uint16_t)current1, &tmp0, &tmp1);
		buf[P_DATA1 + 2] = tmp0;
		buf[P_DATA1 + 3] = tmp1;
		buf[P_DATA1 + 4] = encoder_w1;
		uint32_to_bytes((uint32_t)encoder1, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 5] = tmp0;
		buf[P_DATA1 + 6] = tmp1;
		buf[P_DATA1 + 7] = tmp2;
		buf[P_DATA1 + 8] = tmp3;
		uint16_to_bytes((uint16_t)open_spd1, &tmp0, &tmp1);
		buf[P_DATA1 + 9] = tmp0;
		buf[P_DATA1 + 10] = tmp1;

		buf[P_DATA1 + 11] = controller_w2;
		buf[P_DATA1 + 12] = controller2;
		uint16_to_bytes((uint16_t)current2, &tmp0, &tmp1);
		buf[P_DATA1 + 13] = tmp0;
		buf[P_DATA1 + 14] = tmp1;
		buf[P_DATA1 + 15] = encoder_w2;
		uint32_to_bytes((uint32_t)encoder2, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 16] = tmp0;
		buf[P_DATA1 + 17] = tmp1;
		buf[P_DATA1 + 18] = tmp2;
		buf[P_DATA1 + 19] = tmp3;
		uint16_to_bytes((uint16_t)open_spd2, &tmp0, &tmp1);
		buf[P_DATA1 + 20] = tmp0;
		buf[P_DATA1 + 21] = tmp1;

		bytes = P_DATA1 + 22;     //Bytes is always last+1
	}
	else if(cmd_type == CMD_WRITE)
	{
		//In that case Write is only used for the Reply

		buf[P_CMD1] = CMD_W(CMD_SPC4);

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE

		//Arguments - Execute #1:
		exec_s_ptr = &exec1;

		uint16_to_bytes((uint16_t)exec_s_ptr->gyro.x, &tmp0, &tmp1);
		buf[P_DATA1] = tmp0;
		buf[P_DATA1 + 1] = tmp1;
		uint16_to_bytes((uint16_t)exec_s_ptr->gyro.y, &tmp0, &tmp1);
		buf[P_DATA1 + 2] = tmp0;
		buf[P_DATA1 + 3] = tmp1;
		uint16_to_bytes((uint16_t)exec_s_ptr->gyro.z, &tmp0, &tmp1);
		buf[P_DATA1 + 4] = tmp0;
		buf[P_DATA1 + 5] = tmp1;

		uint16_to_bytes(exec_s_ptr->strain, &tmp0, &tmp1);
		buf[P_DATA1 + 6] = tmp0;
		buf[P_DATA1 + 7] = tmp1;

		uint16_to_bytes(exec_s_ptr->analog[0], &tmp0, &tmp1);
		buf[P_DATA1 + 8] = tmp0;
		buf[P_DATA1 + 9] = tmp1;

		uint32_to_bytes((uint32_t)exec_s_ptr->enc_control, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 10] = tmp0;
		buf[P_DATA1 + 11] = tmp1;
		buf[P_DATA1 + 12] = tmp2;
		buf[P_DATA1 + 13] = tmp3;

		uint16_to_bytes((uint16_t)exec_s_ptr->current, &tmp0, &tmp1);
		buf[P_DATA1 + 14] = tmp0;
		buf[P_DATA1 + 15] = tmp1;

		//Arguments - Execute #2:
		exec_s_ptr = &exec2;

		uint16_to_bytes((uint16_t)exec_s_ptr->gyro.x, &tmp0, &tmp1);
		buf[P_DATA1 + 16] = tmp0;
		buf[P_DATA1 + 17] = tmp1;
		uint16_to_bytes((uint16_t)exec_s_ptr->gyro.y, &tmp0, &tmp1);
		buf[P_DATA1 + 18] = tmp0;
		buf[P_DATA1 + 19] = tmp1;
		uint16_to_bytes((uint16_t)exec_s_ptr->gyro.z, &tmp0, &tmp1);
		buf[P_DATA1 + 20] = tmp0;
		buf[P_DATA1 + 21] = tmp1;

		uint16_to_bytes(exec_s_ptr->strain, &tmp0, &tmp1);
		buf[P_DATA1 + 22] = tmp0;
		buf[P_DATA1 + 23] = tmp1;

		uint16_to_bytes(exec_s_ptr->analog[0], &tmp0, &tmp1);
		buf[P_DATA1 + 24] = tmp0;
		buf[P_DATA1 + 25] = tmp1;

		uint32_to_bytes((uint32_t)exec_s_ptr->enc_control, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 26] = tmp0;
		buf[P_DATA1 + 27] = tmp1;
		buf[P_DATA1 + 28] = tmp2;
		buf[P_DATA1 + 29] = tmp3;

		uint16_to_bytes((uint16_t)exec_s_ptr->current, &tmp0, &tmp1);
		buf[P_DATA1 + 30] = tmp0;
		buf[P_DATA1 + 31] = tmp1;

		bytes = P_DATA1 + 32;     //Bytes is always last+1

		#else

		bytes = 0;

		#endif	//BOARD_TYPE_FLEXSEA_MANAGE
	}
	else
	{
		//Invalid
		flexsea_error(SE_INVALID_READ_TYPE);
		bytes = 0;
	}

	return bytes;
}

//Reception of a CMD_SPECIAL_4 command
void rx_cmd_special_4(uint8_t *buf)
{
	uint32_t numb = 0;

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	//Structure pointer.
	struct execute_s *exec_s_ptr;
	struct spc4_s *spc4_s_ptr;

	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	if(IS_CMD_RW(buf[P_CMD1]) == READ)
	{
		//Received a Read command from our master.

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
		//No code (yet), you shouldn't be here...
		flexsea_error(SE_CMD_NOT_PROGRAMMED);
		#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE

		//Decode its data:
		//===============

		spc4_s_ptr = &spc4_ex1;
		spc4_s_ptr->ctrl_w = buf[P_DATA1 + 0];
		spc4_s_ptr->ctrl = buf[P_DATA1 + 1];
		spc4_s_ptr->current = BYTES_TO_UINT16(buf[P_DATA1 + 2], buf[P_DATA1 + 3]);
		spc4_s_ptr->encoder_w = buf[P_DATA1 + 4];
		spc4_s_ptr->encoder = (int32_t)BYTES_TO_UINT32(buf[P_DATA1 + 5], buf[P_DATA1 + 6], \
				buf[P_DATA1 + 7], buf[P_DATA1 + 8]);
		spc4_s_ptr->open_spd = BYTES_TO_UINT16(buf[P_DATA1 + 9], buf[P_DATA1 + 10]);

		spc4_s_ptr = &spc4_ex2;
		spc4_s_ptr->ctrl_w = buf[P_DATA1 + 11];
		spc4_s_ptr->ctrl = buf[P_DATA1 + 12];
		spc4_s_ptr->current = BYTES_TO_UINT16(buf[P_DATA1 + 13], buf[P_DATA1 + 14]);
		spc4_s_ptr->encoder_w = buf[P_DATA1 + 15];
		spc4_s_ptr->encoder = (int32_t)BYTES_TO_UINT32(buf[P_DATA1 + 16], buf[P_DATA1 + 17], \
				buf[P_DATA1 + 18], buf[P_DATA1 + 19]);
		spc4_s_ptr->open_spd = BYTES_TO_UINT16(buf[P_DATA1 + 20], buf[P_DATA1 + 21]);

		//Generate the reply:
		//===================

		numb = tx_cmd_ctrl_special_4(buf[P_XID], CMD_WRITE, tmp_payload_xmit, PAYLOAD_BUF_LEN, \
									KEEP, 0, KEEP, 0, 0, 0, \
									KEEP, 0, KEEP, 0, 0, 0);
		numb = comm_gen_str(tmp_payload_xmit, comm_str_spi, numb);
		numb = COMM_STR_BUF_LEN;	//Fixed length for now to accomodate the DMA
		//(for now, send it)

		#endif	//BOARD_TYPE_FLEXSEA_MANAGE

		#ifdef BOARD_TYPE_FLEXSEA_PLAN
		//No code (yet), you shouldn't be here...
		flexsea_error(SE_CMD_NOT_PROGRAMMED);
		#endif	//BOARD_TYPE_FLEXSEA_PLAN
	}
	else if(IS_CMD_RW(buf[P_CMD1]) == WRITE)
	{
		//Two options: from Master of from slave (a read reply)

		if(sent_from_a_slave(buf))
		{
			//We received a reply to our read request

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

			#if(defined BOARD_TYPE_FLEXSEA_PLAN)

			//Store values:

			exec_s_ptr = &exec1;
			exec_s_ptr->gyro.x = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+0], buf[P_DATA1+1]));
			exec_s_ptr->gyro.y = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+2], buf[P_DATA1+3]));
			exec_s_ptr->gyro.z = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+4], buf[P_DATA1+5]));

			exec_s_ptr->strain = (BYTES_TO_UINT16(buf[P_DATA1+6], buf[P_DATA1+7]));
			exec_s_ptr->analog[0] = (BYTES_TO_UINT16(buf[P_DATA1+8], buf[P_DATA1+9]));

			exec_s_ptr->enc_display = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1+10], buf[P_DATA1+11], \
										buf[P_DATA1+12], buf[P_DATA1+13]));

			exec_s_ptr->current = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+14], buf[P_DATA1+15]));

			exec_s_ptr = &exec2;
			exec_s_ptr->gyro.x = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+16], buf[P_DATA1+17]));
			exec_s_ptr->gyro.y = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+18], buf[P_DATA1+19]));
			exec_s_ptr->gyro.z = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+20], buf[P_DATA1+21]));

			exec_s_ptr->strain = (BYTES_TO_UINT16(buf[P_DATA1+22], buf[P_DATA1+23]));
			exec_s_ptr->analog[0] = (BYTES_TO_UINT16(buf[P_DATA1+24], buf[P_DATA1+25]));

			exec_s_ptr->enc_display = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1+26], buf[P_DATA1+27], \
										buf[P_DATA1+28], buf[P_DATA1+29]));

			exec_s_ptr->current = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+30], buf[P_DATA1+31]));

			#ifdef MULTIPLE_COMMANDS
			//ToDo interface with Python

			#endif

			#endif	//BOARD_TYPE_FLEXSEA_MANAGE
		}
		else
		{
			//Master is writing a value to this board

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

			//ToDo call relevant functions ****

			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

			#ifdef BOARD_TYPE_FLEXSEA_MANAGE
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_MANAGE

			#ifdef BOARD_TYPE_FLEXSEA_PLAN
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
	}
}

//=============

//Transmission of a CTRL_SPECIAL_5 command: Ankle 2DOF
uint32_t tx_cmd_ctrl_special_5(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								uint8_t slave, uint8_t controller, int16_t ctrl_i, int16_t ctrl_o)
{
	uint8_t tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
	uint32_t bytes = 0;

	#if(defined BOARD_TYPE_FLEXSEA_MANAGE)

	//Structure pointer. Points to exec1 by default.
	struct execute_s *exec_s_ptr = &exec1;

	#endif	//(defined BOARD_TYPE_FLEXSEA_MANAGE)

	//Fresh payload string:
	prepare_empty_payload(board_id, receiver, buf, len);

	//Command:
	buf[P_CMDS] = 1;                     //1 command in string

	if(cmd_type == CMD_READ)
	{
		buf[P_CMD1] = CMD_R(CMD_SPC5);

		//Arguments:
		buf[P_DATA1] = slave;
		buf[P_DATA1 + 1] = controller;
		uint16_to_bytes((uint16_t)ctrl_i, &tmp0, &tmp1);
		buf[P_DATA1 + 2] = tmp0;
		buf[P_DATA1 + 3] = tmp1;
		uint16_to_bytes((uint16_t)ctrl_o, &tmp0, &tmp1);
		buf[P_DATA1 + 4] = tmp0;
		buf[P_DATA1 + 5] = tmp1;

		bytes = P_DATA1 + 6;		//Bytes is always last+1
	}
	else if(cmd_type == CMD_WRITE)
	{
		//In that case Write is only used for the Reply

		buf[P_CMD1] = CMD_W(CMD_SPC5);
		buf[P_DATA1] = slave;

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE

		//Assign data structure based on slave:
		if(slave == 0)
		{
			exec_s_ptr = &exec1;
		}
		else
		{
			exec_s_ptr = &exec2;
		}

		//Arguments
		uint16_to_bytes((uint16_t)exec_s_ptr->gyro.x, &tmp0, &tmp1);
		buf[P_DATA1 + 1] = tmp0;
		buf[P_DATA1 + 2] = tmp1;
		uint16_to_bytes((uint16_t)exec_s_ptr->gyro.y, &tmp0, &tmp1);
		buf[P_DATA1 + 3] = tmp0;
		buf[P_DATA1 + 4] = tmp1;
		uint16_to_bytes((uint16_t)exec_s_ptr->gyro.z, &tmp0, &tmp1);
		buf[P_DATA1 + 5] = tmp0;
		buf[P_DATA1 + 6] = tmp1;

		uint16_to_bytes((uint16_t)exec_s_ptr->accel.x, &tmp0, &tmp1);
		buf[P_DATA1 + 7] = tmp0;
		buf[P_DATA1 + 8] = tmp1;
		uint16_to_bytes((uint16_t)exec_s_ptr->accel.y, &tmp0, &tmp1);
		buf[P_DATA1 + 9] = tmp0;
		buf[P_DATA1 + 10] = tmp1;
		uint16_to_bytes((uint16_t)exec_s_ptr->accel.z, &tmp0, &tmp1);
		buf[P_DATA1 + 11] = tmp0;
		buf[P_DATA1 + 12] = tmp1;

		uint16_to_bytes(exec_s_ptr->strain, &tmp0, &tmp1);
		buf[P_DATA1 + 13] = tmp0;
		buf[P_DATA1 + 14] = tmp1;

		uint16_to_bytes(exec_s_ptr->analog[0], &tmp0, &tmp1);
		buf[P_DATA1 + 15] = tmp0;
		buf[P_DATA1 + 16] = tmp1;

		uint16_to_bytes(exec_s_ptr->analog[1], &tmp0, &tmp1);
		buf[P_DATA1 + 17] = tmp0;
		buf[P_DATA1 + 18] = tmp1;

		uint32_to_bytes((uint32_t)exec_s_ptr->enc_display, &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 19] = tmp0;
		buf[P_DATA1 + 20] = tmp1;
		buf[P_DATA1 + 21] = tmp2;
		buf[P_DATA1 + 22] = tmp3;

		uint16_to_bytes((uint16_t)exec_s_ptr->current, &tmp0, &tmp1);
		buf[P_DATA1 + 23] = tmp0;
		buf[P_DATA1 + 24] = tmp1;

		buf[P_DATA1 + 25] = exec_s_ptr->volt_batt;
		buf[P_DATA1 + 26] = exec_s_ptr->volt_int;
		buf[P_DATA1 + 27] = exec_s_ptr->temp;
		buf[P_DATA1 + 28] = exec_s_ptr->status1;
		buf[P_DATA1 + 29] = exec_s_ptr->status2;

		bytes = P_DATA1 + 30;     //Bytes is always last+1
		#endif	//BOARD_TYPE_FLEXSEA_MANAGE

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Arguments:
		uint16_to_bytes((uint16_t)imu.gyro.x, &tmp0, &tmp1);
		buf[P_DATA1 + 1] = tmp0;
		buf[P_DATA1 + 2] = tmp1;
		uint16_to_bytes((uint16_t)imu.gyro.y, &tmp0, &tmp1);
		buf[P_DATA1 + 3] = tmp0;
		buf[P_DATA1 + 4] = tmp1;
		uint16_to_bytes((uint16_t)imu.gyro.z, &tmp0, &tmp1);
		buf[P_DATA1 + 5] = tmp0;
		buf[P_DATA1 + 6] = tmp1;

		uint16_to_bytes((uint16_t)imu.accel.x, &tmp0, &tmp1);
		buf[P_DATA1 + 7] = tmp0;
		buf[P_DATA1 + 8] = tmp1;
		uint16_to_bytes((uint16_t)imu.accel.y, &tmp0, &tmp1);
		buf[P_DATA1 + 9] = tmp0;
		buf[P_DATA1 + 10] = tmp1;
		uint16_to_bytes((uint16_t)imu.accel.z, &tmp0, &tmp1);
		buf[P_DATA1 + 11] = tmp0;
		buf[P_DATA1 + 12] = tmp1;

		uint16_to_bytes(strain_read(), &tmp0, &tmp1);
		buf[P_DATA1 + 13] = tmp0;
		buf[P_DATA1 + 14] = tmp1;

		uint16_to_bytes(read_analog(0), &tmp0, &tmp1);
		buf[P_DATA1 + 15] = tmp0;
		buf[P_DATA1 + 16] = tmp1;

		uint16_to_bytes(read_analog(1), &tmp0, &tmp1);
		buf[P_DATA1 + 17] = tmp0;
		buf[P_DATA1 + 18] = tmp1;

		uint32_to_bytes((uint32_t)refresh_enc_display(), &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 19] = tmp0;
		buf[P_DATA1 + 20] = tmp1;
		buf[P_DATA1 + 21] = tmp2;
		buf[P_DATA1 + 22] = tmp3;

		uint16_to_bytes((uint16_t)ctrl.current.actual_val, &tmp0, &tmp1);
		buf[P_DATA1 + 23] = tmp0;
		buf[P_DATA1 + 24] = tmp1;

		buf[P_DATA1 + 25] = safety_cop.v_vb;
		buf[P_DATA1 + 26] = safety_cop.v_vg;
		buf[P_DATA1 + 27] = safety_cop.temperature;
		buf[P_DATA1 + 28] = safety_cop.status1;
		buf[P_DATA1 + 29] = safety_cop.status2;

		bytes = P_DATA1 + 30;     //Bytes is always last+1

		#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
	}
	else
	{
		//Invalid
		flexsea_error(SE_INVALID_READ_TYPE);
		bytes = 0;
	}

	return bytes;
}

//Reception of a CMD_SPECIAL_5 command
void rx_cmd_special_5(uint8_t *buf)
{
	uint32_t numb = 0;
	uint8_t slave = 0;
	int16_t tmp_wanted_current = 0, tmp_open_spd = 0;

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	//Structure pointer.
	struct execute_s *exec_s_ptr = &exec1;
	//struct spc5_s *spc5_s_ptr;

	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	if(IS_CMD_RW(buf[P_CMD1]) == READ)
	{
		//Received a Read command from our master.

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		slave = buf[P_DATA1];

		//Update controller:
		control_strategy(buf[P_DATA1 + 1]);

		//Only change the setpoint if we are in current control mode:
		if(ctrl.active_ctrl == CTRL_CURRENT)
		{
			tmp_wanted_current = BYTES_TO_UINT16(buf[P_DATA1 + 2], buf[P_DATA1 + 3]);
			ctrl.current.setpoint_val = tmp_wanted_current;
		}
		else if(ctrl.active_ctrl == CTRL_OPEN)
		{
			tmp_open_spd = (int16_t) BYTES_TO_UINT16(buf[P_DATA1 + 4], buf[P_DATA1 + 5]);
			motor_open_speed_1(tmp_open_spd);
		}

		//Generate the reply:
		numb = tx_cmd_ctrl_special_5(buf[P_XID], CMD_WRITE, tmp_payload_xmit, PAYLOAD_BUF_LEN, slave, 0, 0, 0);
		numb = comm_gen_str(tmp_payload_xmit, comm_str_485_1, numb);
		numb = COMM_STR_BUF_LEN;	//Fixed length for now to accomodate the DMA

		//Delayed response:
		rs485_reply_ready(comm_str_485_1, (numb));

		#ifdef USE_USB
		usb_puts(comm_str_485_1, (numb));
		#endif

		#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE

		//Decode its data:
		//===============
		slave = buf[P_DATA1];
		//...

		//Generate the reply:
		//===================

		numb = tx_cmd_ctrl_special_5(buf[P_XID], CMD_WRITE, tmp_payload_xmit, PAYLOAD_BUF_LEN, \
									slave, 0, 0, 0);
		numb = comm_gen_str(tmp_payload_xmit, comm_str_spi, numb);
		numb = COMM_STR_BUF_LEN;	//Fixed length for now to accomodate the DMA
		flexsea_send_serial_master(PORT_USB, comm_str_spi, numb);	//Same comment here - ToDo fix
		//(the SPI driver will grab comm_str_spi directly)

		#endif	//BOARD_TYPE_FLEXSEA_MANAGE

		#ifdef BOARD_TYPE_FLEXSEA_PLAN
		//No code (yet), you shouldn't be here...
		flexsea_error(SE_CMD_NOT_PROGRAMMED);
		#endif	//BOARD_TYPE_FLEXSEA_PLAN
	}
	else if(IS_CMD_RW(buf[P_CMD1]) == WRITE)
	{
		//Two options: from Master of from slave (a read reply)

		if(sent_from_a_slave(buf))
		{
			//We received a reply to our read request

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

			#if((defined BOARD_TYPE_FLEXSEA_MANAGE))
			slave = buf[P_XID];
			//Assign data structure based on slave:
			if(slave == FLEXSEA_EXECUTE_1)
			{
				exec_s_ptr = &exec1;
			}
			else
			{
				exec_s_ptr = &exec2;
			}
			#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE))

			#if((defined BOARD_TYPE_FLEXSEA_PLAN))
			slave = buf[P_DATA1];

			//Assign data structure based on slave:
			if(slave == 0)
			{
				exec_s_ptr = &exec1;
			}
			else
			{
				exec_s_ptr = &exec2;
			}
			#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE))

			#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

			//Decode its data:
			//===============

			//Store values:

			exec_s_ptr->gyro.x = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+1], buf[P_DATA1+2]));
			exec_s_ptr->gyro.y = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+3], buf[P_DATA1+4]));
			exec_s_ptr->gyro.z = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+5], buf[P_DATA1+6]));

			exec_s_ptr->accel.x = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+7], buf[P_DATA1+8]));
			exec_s_ptr->accel.y = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+9], buf[P_DATA1+10]));
			exec_s_ptr->accel.z = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+11], buf[P_DATA1+12]));

			exec_s_ptr->strain = (BYTES_TO_UINT16(buf[P_DATA1+13], buf[P_DATA1+14]));
			exec_s_ptr->analog[0] = (BYTES_TO_UINT16(buf[P_DATA1+15], buf[P_DATA1+16]));
			exec_s_ptr->analog[1] = (BYTES_TO_UINT16(buf[P_DATA1+17], buf[P_DATA1+18]));

			exec_s_ptr->enc_display = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1+19], buf[P_DATA1+20], \
										buf[P_DATA1+21], buf[P_DATA1+22]));

			exec_s_ptr->current = (int16_t) (BYTES_TO_UINT16(buf[P_DATA1+23], buf[P_DATA1+24]));

			exec_s_ptr->volt_batt = buf[P_DATA1+25];
			exec_s_ptr->volt_int = buf[P_DATA1+26];
			exec_s_ptr->temp = buf[P_DATA1+27];
			exec_s_ptr->status1 = buf[P_DATA1+28];
			exec_s_ptr->status2 = buf[P_DATA1+29];

			#endif	//BOARD_TYPE_FLEXSEA_MANAGE
		}
		else
		{
			//Master is writing a value to this board

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

			//ToDo call relevant functions ****

			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

			#ifdef BOARD_TYPE_FLEXSEA_MANAGE
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_MANAGE

			#ifdef BOARD_TYPE_FLEXSEA_PLAN
			//No code (yet), you shouldn't be here...
			flexsea_error(SE_CMD_NOT_PROGRAMMED);
			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
	}
}

//Command: CMD_RICNU. Type: R/W.
//setGains: KEEP/CHANGE
void tx_cmd_ricnu_rw(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType,uint16_t *len,\
					uint8_t offset, uint8_t controller, \
					int32_t setpoint, uint8_t setGains, int16_t g0, int16_t g1,\
					int16_t g2, int16_t g3)
{
	uint16_t index = 0;

	//Formatting:
	(*cmd) = CMD_RICNU;
	(*cmdType) = READ;

	//Data:
	shBuf[index++] = offset;
	shBuf[index++] = controller;
	SPLIT_32(setpoint, shBuf, &index);
	shBuf[index++] = setGains;
	SPLIT_16(g0, shBuf, &index);
	SPLIT_16(g1, shBuf, &index);
	SPLIT_16(g2, shBuf, &index);
	SPLIT_16(g3, shBuf, &index);

	//Payload length:
	(*len) = index;
}

//Command: CMD_RICNU. Type: R.
//setGains: KEEP/CHANGE
void tx_cmd_ricnu_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
					uint16_t *len, uint8_t offset)
{
	uint16_t index = 0;

	//Formatting:
	(*cmd) = CMD_RICNU;
	(*cmdType) = READ;

	//Data:
	shBuf[index++] = offset;

	//Payload length:
	(*len) = index;
}

//Command: CMD_RICNU. Type: W.
//setGains: KEEP/CHANGE
void tx_cmd_ricnu_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
					uint16_t *len, uint8_t offset)
{
	uint16_t index = 0;

	//Formatting:
	(*cmd) = CMD_RICNU;
	(*cmdType) = WRITE;

	//Data:
	shBuf[index++] = offset;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Arguments:
		if(offset == 0)
		{
			SPLIT_16((uint16_t)imu.gyro.x, shBuf, &index);
			SPLIT_16((uint16_t)imu.gyro.y, shBuf, &index);
			SPLIT_16((uint16_t)imu.gyro.z, shBuf, &index);
			SPLIT_16((uint16_t)imu.accel.x, shBuf, &index);
			SPLIT_16((uint16_t)imu.accel.y, shBuf, &index);
			SPLIT_16((uint16_t)imu.accel.z, shBuf, &index);
			SPLIT_32((uint32_t)exec1.enc_motor, shBuf, &index);
			SPLIT_32((uint32_t)exec1.enc_joint, shBuf, &index);
			SPLIT_16((uint16_t)ctrl.current.actual_val, shBuf, &index);
		}
		else if(offset == 1)
		{
			//Compressed Strain:

			shBuf[index++] = strain1.compressedBytes[0];
			shBuf[index++] = strain1.compressedBytes[1];
			shBuf[index++] = strain1.compressedBytes[2];
			shBuf[index++] = strain1.compressedBytes[3];
			shBuf[index++] = strain1.compressedBytes[4];
			shBuf[index++] = strain1.compressedBytes[5];
			shBuf[index++] = strain1.compressedBytes[6];
			shBuf[index++] = strain1.compressedBytes[7];
			shBuf[index++] = strain1.compressedBytes[8];
			//Include other variables here (ToDo)
		}
		else
		{
			//Deal with other offsets here...
		}

	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

	//Payload length:
	(*len) = index;
}

//Gets called when our Master sends us a Read request
void rx_cmd_ricnu_rw(uint8_t *buf, uint8_t *info)
{
	uint16_t index = 0;

	//Temporary variables
	uint8_t offset = 0;
	uint8_t tmpController = 0, tmpSetGains = 0;
	int32_t tmpSetpoint = 0;
	int16_t tmpGain[4] = {0,0,0,0};

	//Decode data received:
	index = P_DATA1;
	offset = buf[index++];
	tmpController = buf[index++];
	tmpSetpoint = (int32_t)REBUILD_UINT32(buf, &index);
	tmpSetGains = buf[index++];
	tmpGain[0] = (int16_t)REBUILD_UINT16(buf, &index);
	tmpGain[1] = (int16_t)REBUILD_UINT16(buf, &index);
	tmpGain[2] = (int16_t)REBUILD_UINT16(buf, &index);
	tmpGain[3] = (int16_t)REBUILD_UINT16(buf, &index);

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

	//Act on the decoded data:
	rx_cmd_ricnu_Action1(tmpController, tmpSetpoint, tmpSetGains, tmpGain[0],
							tmpGain[1], tmpGain[2], tmpGain[3]);
	#endif

	//Generate the reply:
//	numb = tx_cmd_ricnu_w(TX_CMD_DEFAULT, offset, 0, 0, 0, 0, 0, 0);	//ToDo
//	COMM_GEN_STR_DEFAULT
//	flexsea_send_serial_master(myPort, myData, myLen);	//ToDo
}

//Gets called when our Slave sends us a Reply to our Read Request
void rx_cmd_ricnu_rr(uint8_t *buf, uint8_t *info)
{
	uint16_t index = 0;
	uint8_t offset = 0;
	
	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
		(void)buf;
		flexsea_error(SE_CMD_NOT_PROGRAMMED);
		return;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

		struct execute_s *ex = &exec1;
		struct ricnu_s *rn = &ricnu_1;

		index = P_DATA1;
		offset = buf[index++];

		if(offset == 0)
		{
			rn->ex.gyro.x = (int16_t) REBUILD_UINT16(buf, &index);
			rn->ex.gyro.y = (int16_t) REBUILD_UINT16(buf, &index);
			rn->ex.gyro.z = (int16_t) REBUILD_UINT16(buf, &index);
			rn->ex.accel.x = (int16_t) REBUILD_UINT16(buf, &index);
			rn->ex.accel.y = (int16_t) REBUILD_UINT16(buf, &index);
			rn->ex.accel.z = (int16_t) REBUILD_UINT16(buf, &index);
			rn->ex.enc_motor = (int32_t) REBUILD_UINT32(buf, &index);
			rn->ex.enc_joint = (int32_t) REBUILD_UINT32(buf, &index);
			rn->ex.current = (int16_t) REBUILD_UINT16(buf, &index);
		}
		else if(offset == 1)
		{
			rn->st.compressedBytes[0] = buf[index++];
			rn->st.compressedBytes[1] = buf[index++];
			rn->st.compressedBytes[2] = buf[index++];
			rn->st.compressedBytes[3] = buf[index++];
			rn->st.compressedBytes[4] = buf[index++];
			rn->st.compressedBytes[5] = buf[index++];
			rn->st.compressedBytes[6] = buf[index++];
			rn->st.compressedBytes[7] = buf[index++];
			rn->st.compressedBytes[8] = buf[index++];

			//Include other variables here (ToDo)
		}
		else
		{
			//...
		}

		//Copy RICNU to Exec:
		*ex = rn->ex;

	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))
}

//Gets called when our Master Writes to us
void rx_cmd_ricnu_w(uint8_t *buf, uint8_t *info)
{
	//Master Write isn't implemented for this command.

	(void)buf;
	(void)info;
	flexsea_error(SE_CMD_NOT_PROGRAMMED);
}

//Command = rx_cmd_ricnu, section = READ
void rx_cmd_ricnu_Action1(uint8_t controller, int32_t setpoint, uint8_t setGains,
						int16_t g0,	int16_t g1,	int16_t g2, int16_t g3)
{
	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

	//Update controller (if needed):
	control_strategy(controller);

	//Only change the setpoint if we are in current control mode:
	if(ctrl.active_ctrl == CTRL_CURRENT)
	{
		ctrl.current.setpoint_val = setpoint;
		if (setGains == CHANGE)
		{
			ctrl.current.gain.g0 = g0;
			ctrl.current.gain.g1 = g1;
		}
	}
	else if(ctrl.active_ctrl == CTRL_OPEN)
	{
		motor_open_speed_1(setpoint);
	}
	else if(ctrl.active_ctrl == CTRL_POSITION)
	{
		ctrl.position.setp = setpoint;
		if (setGains == CHANGE)
		{
			ctrl.position.gain.g0 = g0;
			ctrl.position.gain.g1 = g1;
		}
	}
	else if (ctrl.active_ctrl == CTRL_IMPEDANCE)
	{
		ctrl.impedance.setpoint_val = setpoint;
		if (setGains == CHANGE)
		{
			ctrl.impedance.gain.g0 = g0;
			ctrl.impedance.gain.g1 = g1;
			ctrl.current.gain.g0 = g2;
			ctrl.current.gain.g1 = g3;
		}
	}

	#else

		//Unused - deal with variables to avoid warnings
		(void) controller;
		(void)setpoint;
		(void)setGains;
		(void)g0;
		(void)g1;
		(void)g2;
		(void)g3;

	#endif
}

#ifdef __cplusplus
}
#endif

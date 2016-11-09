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
	[This file] flexsea_cmd_data: Data Commands
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
#include "../inc/flexsea_cmd_data.h"

//Manage boards only:
#ifdef BOARD_TYPE_FLEXSEA_MANAGE
#include "user-mn.h"
#endif	//BOARD_TYPE_FLEXSEA_MANAGE

#if (defined BOARD_TYPE_FLEXSEA_EXECUTE || defined BOARD_TYPE_FLEXSEA_GOSSIP \
	|| defined BOARD_TYPE_FLEXSEA_STRAIN_AMP)
#include "main.h"
#endif

//****************************************************************************
// Variable(s)
//****************************************************************************

//Will change this, but for now the payloads will be stored in:
uint8_t tmp_payload_xmit[PAYLOAD_BUF_LEN];

//****************************************************************************
// Function(s)
//****************************************************************************

//This gets called by flexsea_system's init_flexsea_payload_ptr(). Map all
//functions from this file to the array here. Failure to do so will send all
//commands to flexsea_payload_catchall().
void init_flexsea_payload_ptr_data(void)
{
	flexsea_payload_ptr[CMD_READ_ALL][RX_PTYPE_READ] = &rx_cmd_data_read_all_rw;
	flexsea_payload_ptr[CMD_READ_ALL][RX_PTYPE_REPLY] = &rx_cmd_data_read_all_rr;

	//flexsea_payload_ptr[CMD_USER_DATA][RX_PTYPE_READ] = &rx_cmd_data_user;
}

void tx_cmd_data_read_all_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
					uint16_t *len)
{
	uint16_t index = 0;

	//Formatting:
	(*cmd) = CMD_READ_ALL;
	(*cmdType) = CMD_READ;

	//Data:
	//(none)
	(void)shBuf;

	//Payload length:
	(*len) = index;
}

void tx_cmd_data_read_all_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
					uint16_t *len)
{
	uint16_t index = 0;

	//Formatting:
	(*cmd) = CMD_READ_ALL;
	(*cmdType) = CMD_WRITE;

	//Data:
	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

	SPLIT_16((uint16_t)imu.gyro.x, shBuf, &index);
	SPLIT_16((uint16_t)imu.gyro.y, shBuf, &index);
	SPLIT_16((uint16_t)imu.gyro.z, shBuf, &index);
	SPLIT_16((uint16_t)imu.accel.x, shBuf, &index);
	SPLIT_16((uint16_t)imu.accel.y, shBuf, &index);
	SPLIT_16((uint16_t)imu.accel.z, shBuf, &index);

	SPLIT_16(strain_read(), shBuf, &index);
	SPLIT_16(read_analog(0), shBuf, &index);
	SPLIT_16(read_analog(1), shBuf, &index);

	SPLIT_32((uint32_t)(uint32_t)refresh_enc_display(), shBuf, &index);
	SPLIT_16((uint16_t)ctrl.current.actual_val, shBuf, &index);

	shBuf[index++] = safety_cop.v_vb;
	shBuf[index++] = safety_cop.v_vg;
	shBuf[index++] = safety_cop.temperature;
	shBuf[index++] = safety_cop.status1;
	shBuf[index++] = safety_cop.status2;

	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

	#ifdef BOARD_TYPE_FLEXSEA_MANAGE

	SPLIT_16((uint16_t)imu.gyro.x, shBuf, &index);
	SPLIT_16((uint16_t)imu.gyro.y, shBuf, &index);
	SPLIT_16((uint16_t)imu.gyro.z, shBuf, &index);
	SPLIT_16((uint16_t)imu.accel.x, shBuf, &index);
	SPLIT_16((uint16_t)imu.accel.y, shBuf, &index);
	SPLIT_16((uint16_t)imu.accel.z, shBuf, &index);

	//...

	#endif  //BOARD_TYPE_FLEXSEA_MANAGE

	#ifdef BOARD_TYPE_FLEXSEA_GOSSIP

	SPLIT_16((uint16_t)imu.gyro.x, shBuf, &index);
	SPLIT_16((uint16_t)imu.gyro.y, shBuf, &index);
	SPLIT_16((uint16_t)imu.gyro.z, shBuf, &index);
	SPLIT_16((uint16_t)imu.accel.x, shBuf, &index);
	SPLIT_16((uint16_t)imu.accel.y, shBuf, &index);
	SPLIT_16((uint16_t)imu.accel.z, shBuf, &index);
	SPLIT_16((uint16_t)imu.magneto.x, shBuf, &index);
	SPLIT_16((uint16_t)imu.magneto.y, shBuf, &index);
	SPLIT_16((uint16_t)imu.magneto.z, shBuf, &index);

	#endif  //BOARD_TYPE_FLEXSEA_GOSSIP

	#ifdef BOARD_TYPE_FLEXSEA_STRAIN_AMP

	//Arguments:

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

	#endif 	//BOARD_TYPE_FLEXSEA_STRAIN_AMP

	//Payload length:
	(*len) = index;
}

void rx_cmd_data_read_all_rw(uint8_t *buf, uint8_t *info)
{
	/*
	uint8_t cmdCode = 0, cmdType = 0;
	uint16_t len = 0, numb = 0;
	uint8_t transferBuf[COMM_STR_BUF_LEN];
	*/

	//Note: for Read All, there is no RW per say, it's always a pure read,
	//so we send a tx_ command right away:

	/*
	//ToDo: we need to use something more generic than 'comm_str_usb'
	tx_cmd_data_read_all_w(tmp_payload_xmit, &cmdCode, &cmdType, &len);
	numb = tx_cmd(tmp_payload_xmit, cmdCode, cmdType, len, buf[P_XID], transferBuf);
	numb = comm_gen_str(transferBuf, comm_str_usb, numb);
	numb = COMM_STR_BUF_LEN;	//Fixed length for now to accomodate the DMA
	flexsea_send_serial_master(info[0], comm_str_usb, numb);
	*/

	tx_cmd_data_read_all_w(TX_N_DEFAULT);
	packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, 0);
}

void rx_cmd_data_read_all_rr(uint8_t *buf, uint8_t *info)
{
	uint16_t index = P_DATA1;
	uint8_t tmp = 0, baseAddr = 0;

	(void)info;	//Unused for now

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

	//Structure pointers:
	struct execute_s *exec_s_ptr = &exec1;
	struct manage_s *mn_s_ptr = &manag1;
	struct gossip_s *go_s_ptr = &gossip1;
	struct strain_s *st_s_ptr = &strain1;
	executePtrXid(&exec_s_ptr, buf[P_XID]);
	managePtrXid(&mn_s_ptr, buf[P_XID]);
	gossipPtrXid(&go_s_ptr, buf[P_XID]);
	strainPtrXid(&st_s_ptr, buf[P_XID]);

	//Extract base address:
	tmp = buf[P_XID]/10;
	baseAddr = 10*tmp;

	switch(baseAddr)
	{
		case FLEXSEA_EXECUTE_BASE:

			exec_s_ptr->gyro.x = (int16_t) REBUILD_UINT16(buf, &index);
			exec_s_ptr->gyro.y = (int16_t) REBUILD_UINT16(buf, &index);
			exec_s_ptr->gyro.z = (int16_t) REBUILD_UINT16(buf, &index);
			exec_s_ptr->accel.x = (int16_t) REBUILD_UINT16(buf, &index);
			exec_s_ptr->accel.y = (int16_t) REBUILD_UINT16(buf, &index);
			exec_s_ptr->accel.z = (int16_t) REBUILD_UINT16(buf, &index);

			exec_s_ptr->strain = (int16_t) REBUILD_UINT16(buf, &index);
			exec_s_ptr->analog[0] = (int16_t) REBUILD_UINT16(buf, &index);
			exec_s_ptr->analog[1] = (int16_t) REBUILD_UINT16(buf, &index);
			exec_s_ptr->enc_display = (int32_t) REBUILD_UINT32(buf, &index);
			exec_s_ptr->current = (int16_t) REBUILD_UINT16(buf, &index);

			exec_s_ptr->volt_batt = buf[index++];
			exec_s_ptr->volt_int = buf[index++];
			exec_s_ptr->temp = buf[index++];
			exec_s_ptr->status1 = buf[index++];
			exec_s_ptr->status2 = buf[index++];
			break;

		case FLEXSEA_MANAGE_BASE:

			mn_s_ptr->gyro.x = (int16_t) REBUILD_UINT16(buf, &index);
			mn_s_ptr->gyro.y = (int16_t) REBUILD_UINT16(buf, &index);
			mn_s_ptr->gyro.z = (int16_t) REBUILD_UINT16(buf, &index);
			mn_s_ptr->accel.x = (int16_t) REBUILD_UINT16(buf, &index);
			mn_s_ptr->accel.y = (int16_t) REBUILD_UINT16(buf, &index);
			mn_s_ptr->accel.z = (int16_t) REBUILD_UINT16(buf, &index);

			//...
			break;

		case FLEXSEA_GOSSIP_BASE:

			go_s_ptr->gyro.x = (int16_t) REBUILD_UINT16(buf, &index);
			go_s_ptr->gyro.y = (int16_t) REBUILD_UINT16(buf, &index);
			go_s_ptr->gyro.z = (int16_t) REBUILD_UINT16(buf, &index);
			go_s_ptr->accel.x = (int16_t) REBUILD_UINT16(buf, &index);
			go_s_ptr->accel.y = (int16_t) REBUILD_UINT16(buf, &index);
			go_s_ptr->accel.z = (int16_t) REBUILD_UINT16(buf, &index);
			go_s_ptr->magneto.x = (int16_t) REBUILD_UINT16(buf, &index);
			go_s_ptr->magneto.y = (int16_t) REBUILD_UINT16(buf, &index);
			go_s_ptr->magneto.z = (int16_t) REBUILD_UINT16(buf, &index);
			break;

		case FLEXSEA_STRAIN_BASE:

			st_s_ptr->compressedBytes[0] = buf[index++];
			st_s_ptr->compressedBytes[1] = buf[index++];
			st_s_ptr->compressedBytes[2] = buf[index++];
			st_s_ptr->compressedBytes[3] = buf[index++];
			st_s_ptr->compressedBytes[4] = buf[index++];
			st_s_ptr->compressedBytes[5] = buf[index++];
			st_s_ptr->compressedBytes[6] = buf[index++];
			st_s_ptr->compressedBytes[7] = buf[index++];
			st_s_ptr->compressedBytes[8] = buf[index++];
			break;
	}

	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))
}

//****************************************************************************
// Antiquated but valid function(s) - rework & integrate:
//****************************************************************************

/*
//Transmission of a USER_DATA command
//Note: we can only write one parameter at the time (that's what we need for typical use cases).
//'select_w' will determine what variable is written.
uint32_t tx_cmd_data_user(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, uint8_t select_w )
{
	uint8_t tmp0  = 0, tmp1  = 0, tmp2  = 0, tmp3  = 0;
	uint32_t bytes = 0;

	//Fresh payload string:
	prepare_empty_payload(board_id, receiver, buf, len);

	//Command:
	buf[P_CMDS] = 1;	//1 command in string

	if(cmd_type == CMD_READ)
	{
		buf[P_CMD1] = CMD_R(CMD_USER_DATA);

		bytes = P_CMD1 + 1;     //Bytes is always last+1
	}
	else if(cmd_type == CMD_WRITE)
	{
		buf[P_CMD1] = CMD_W(CMD_USER_DATA);

		//Arguments:

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE

		//Manage: Send all the R variables.

		uint32_to_bytes((uint32_t)user_data.r[0], &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 0] = tmp0;
		buf[P_DATA1 + 1] = tmp1;
		buf[P_DATA1 + 2] = tmp2;
		buf[P_DATA1 + 3] = tmp3;

		uint32_to_bytes((uint32_t)user_data.r[1], &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 4] = tmp0;
		buf[P_DATA1 + 5] = tmp1;
		buf[P_DATA1 + 6] = tmp2;
		buf[P_DATA1 + 7] = tmp3;

		uint32_to_bytes((uint32_t)user_data.r[2], &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 8] = tmp0;
		buf[P_DATA1 + 9] = tmp1;
		buf[P_DATA1 + 10] = tmp2;
		buf[P_DATA1 + 11] = tmp3;

		uint32_to_bytes((uint32_t)user_data.r[3], &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 12] = tmp0;
		buf[P_DATA1 + 13] = tmp1;
		buf[P_DATA1 + 14] = tmp2;
		buf[P_DATA1 + 15] = tmp3;

		bytes = P_DATA1 + 31;     //Bytes is always last+1

		#endif //BOARD_TYPE_FLEXSEA_EXECUTE

		#ifdef BOARD_TYPE_FLEXSEA_PLAN

		buf[P_DATA1 + 0] = select_w; //Parameter written
		uint32_to_bytes((uint32_t)user_data_1.w[select_w], &tmp0, &tmp1, &tmp2, &tmp3);
		buf[P_DATA1 + 1] = tmp0;
		buf[P_DATA1 + 2] = tmp1;
		buf[P_DATA1 + 3] = tmp2;
		buf[P_DATA1 + 4] = tmp3;

		bytes = P_DATA1 + 5;     //Bytes is always last+1

		#endif  //BOARD_TYPE_FLEXSEA_PLAN
	}
	else
	{
		//Invalid
		flexsea_error(SE_INVALID_READ_TYPE);
		bytes = 0;
	}

	return bytes;
}

//Reception of a USER_DATA command
void rx_cmd_data_user(uint8_t *buf)
{
	uint8_t numb = 0, w_select = 0;
	int32_t w_val = 0;

	if(IS_CMD_RW(buf[P_CMD1]) == READ)
	{
		//Received a Read command from our master, prepare a reply:

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE

		//Generate the reply:
		numb = tx_cmd_data_user(buf[P_XID], CMD_WRITE, tmp_payload_xmit, PAYLOAD_BUF_LEN, \
									0);
		numb = comm_gen_str(tmp_payload_xmit, comm_str_spi, numb);
		numb = COMM_STR_BUF_LEN;	//Fixed length for now to accomodate the DMA
		flexsea_send_serial_master(PORT_USB, comm_str_spi, numb);	//Same comment here - ToDo fix
		//(the SPI driver will grab comm_str_spi directly)

		#endif	//BOARD_TYPE_FLEXSEA_MANAGE
	}
	else if(IS_CMD_RW(buf[P_CMD1]) == WRITE)
	{
		//Two options: from Master of from slave (a read reply)

		//Decode data:
		w_select = buf[P_DATA1];
		w_val = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1 + 1], buf[P_DATA1 + 2], buf[P_DATA1 + 3], buf[P_DATA1 + 4]));

		if(sent_from_a_slave(buf))
		{
			//We received a reply to our read request

			#if((defined BOARD_TYPE_FLEXSEA_PLAN))

			//Store value:
			user_data_1.r[0] = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1 + 0], buf[P_DATA1 + 1], buf[P_DATA1 + 2], buf[P_DATA1 + 3]));
			user_data_1.r[1] = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1 + 4], buf[P_DATA1 + 5], buf[P_DATA1 + 6], buf[P_DATA1 + 7]));
			user_data_1.r[2] = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1 + 8], buf[P_DATA1 + 9], buf[P_DATA1 + 10], buf[P_DATA1 + 11]));
			user_data_1.r[3] = (int32_t) (BYTES_TO_UINT32(buf[P_DATA1 + 12], buf[P_DATA1 + 13], buf[P_DATA1 + 14], buf[P_DATA1 + 15]));

			#endif	//((defined BOARD_TYPE_FLEXSEA_PLAN))
		}
		else
		{
			//Master is writing a value to this board

			#ifdef BOARD_TYPE_FLEXSEA_MANAGE

			user_data.w[w_select] = w_val;

			#endif	//BOARD_TYPE_FLEXSEA_MANAGE
		}
	}
}
*/

#ifdef __cplusplus
}
#endif

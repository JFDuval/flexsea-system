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
	[This file] flexsea_cmd_external: External/Expansion connector
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

#include "main.h"
#include "../inc/flexsea_system.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

//Will change this, but for now the payloads will be stored in:
uint8_t tmp_payload_xmit[PAYLOAD_BUF_LEN];
struct execute_s *exPtrE = &exec1;

//****************************************************************************
// Function(s)
//****************************************************************************

//This gets called by flexsea_system's init_flexsea_payload_ptr(). Map all
//functions from this file to the array here. Failure to do so will send all
//commands to flexsea_payload_catchall().
void init_flexsea_payload_ptr_external(void)
{
	flexsea_payload_ptr[CMD_PWRO][RX_PTYPE_READ] = &rx_cmd_exp_pwro_rw;
	flexsea_payload_ptr[CMD_PWRO][RX_PTYPE_WRITE] = &rx_cmd_exp_pwro_w;
	flexsea_payload_ptr[CMD_PWRO][RX_PTYPE_REPLY] = &rx_cmd_exp_pwro_rr;
}

//Transmit Power Output:
//======================

//Test code? No
void tx_cmd_exp_pwro_w(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len, uint8_t pwroPwm)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_PWRO;
	(*cmdType) = CMD_WRITE;

	//Data:
	shBuf[index++] = pwroPwm;

	//Payload length:
	(*len) = index;
}

//Test code? No
void tx_cmd_exp_pwro_r(uint8_t *shBuf, uint8_t *cmd, uint8_t *cmdType, \
						uint16_t *len)
{
	//Variable(s) & command:
	uint16_t index = 0;
	(*cmd) = CMD_PWRO;
	(*cmdType) = CMD_READ;

	//Data:
	(void)shBuf; //(none)

	//Payload length:
	(*len) = index;
}

//Receive Power Output:
//=====================

//Test code? No
void rx_cmd_exp_pwro_w(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		pwro_output(buf[P_DATA1]);

	#else
		(void)buf;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_exp_pwro_rw(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		tx_cmd_exp_pwro_w(TX_N_DEFAULT, read_pwro());
		packAndSend(P_AND_S_DEFAULT, buf[P_XID], info, 0);

	#else
		(void)buf;
	#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
}

//Test code? No
void rx_cmd_exp_pwro_rr(uint8_t *buf, uint8_t *info)
{
	(void)info;

	#if((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))

		//Decode data:
		uint8_t pwro = buf[P_DATA1];

		//Store value:
		executePtrXid(&exPtrE, buf[P_XID]);
		exPtrE->pwro = pwro;

	#else
		(void)buf;
	#endif	//((defined BOARD_TYPE_FLEXSEA_MANAGE) || (defined BOARD_TYPE_FLEXSEA_PLAN))
}

#ifdef __cplusplus
}
#endif

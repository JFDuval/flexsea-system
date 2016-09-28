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

#ifdef __cplusplus
extern "C" {
#endif

//ToDo: the ENCODER command is too "QEI" centric. Generalize.

//****************************************************************************
// Include(s)
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include "../inc/flexsea_system.h"

//Plan boards only:
#ifdef BOARD_TYPE_FLEXSEA_PLAN
//...
#endif	//BOARD_TYPE_FLEXSEA_PLAN

//Manage boards only:
#ifdef BOARD_TYPE_FLEXSEA_MANAGE
//...
#endif	//BOARD_TYPE_FLEXSEA_MANAGE

//Execute boards only:
#ifdef BOARD_TYPE_FLEXSEA_EXECUTE
#include "main.h"
#endif	//BOARD_TYPE_FLEXSEA_EXECUTE

//Strain Amplifier boards only:
#ifdef BOARD_TYPE_FLEXSEA_STRAIN_AMP
#include "main.h"
#endif	//BOARD_TYPE_FLEXSEA_STRAIN_AMP

//****************************************************************************
// Variable(s)
//****************************************************************************

//Will change this, but for now the payloads will be stored in:
unsigned char tmp_payload_xmit[PAYLOAD_BUF_LEN];

//****************************************************************************
// Function(s)
//****************************************************************************

//Transmission of a SWITCH command: Read Manage's onboard switch
uint32_t tx_cmd_switch(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len)
{
	uint32_t bytes = 0;

	//Fresh payload string:
	prepare_empty_payload(board_id, receiver, buf, len);

	//Command:
	buf[P_CMDS] = 1;                     //1 command in string

	if(cmd_type == CMD_READ)
	{
		buf[P_CMD1] = CMD_R(CMD_SWITCH);

		//Arguments:
		//(none)

		bytes = P_CMD1 + 1;     //Bytes is always last+1
	}
	else if(cmd_type == CMD_WRITE)
	{
		//In that case Write is only used for the Reply

		buf[P_CMD1] = CMD_W(CMD_SWITCH);

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE

		//Arguments:
		buf[P_DATA1 + 0] = read_sw1();

		bytes = P_DATA1 + 1;     //Bytes is always last+1

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

//Reception of a CMD_SWITCH command
void rx_cmd_switch(uint8_t *buf)
{
	uint32_t numb = 0;
	uint8_t tmp_sw = 0;

	if(IS_CMD_RW(buf[P_CMD1]) == READ)
	{
		//Received a Read command from our master.

		#ifdef BOARD_TYPE_FLEXSEA_MANAGE

		//Generate the reply:
		//===================

		numb = tx_cmd_switch(buf[P_XID], CMD_WRITE, tmp_payload_xmit, PAYLOAD_BUF_LEN);
		numb = comm_gen_str(tmp_payload_xmit, comm_str_spi, numb);
		numb = COMM_STR_BUF_LEN;    //Fixed length for now
		//(the SPI driver will grab comm_str_spi directly)
		//flexsea_send_serial_master(0, comm_str, numb);

		#endif	//BOARD_TYPE_FLEXSEA_MANAGE

	}
	else if(IS_CMD_RW(buf[P_CMD1]) == WRITE)
	{
		//Two options: from Master of from slave (a read reply)

		if(sent_from_a_slave(buf))
		{
			//We received a reply to our read request

			#ifdef BOARD_TYPE_FLEXSEA_PLAN

			printf("Received data\n");//ToDo remove
			manag1.sw1 = buf[P_DATA1];

			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
		else
		{
			//Master is writing a value to this board

		}
	}
}

//Transmission of an ENCODER command.
uint32_t tx_cmd_encoder(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, int32_t enc)
{
	uint32_t bytes = 0;
	uint8_t tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;

	//Fresh payload string:
	prepare_empty_payload(board_id, receiver, buf, len);

	//Command:
	buf[P_CMDS] = 1;                     //1 command in string

	if(cmd_type == CMD_READ)
	{
		buf[P_CMD1] = CMD_R(CMD_ENCODER);

		//Arguments:
		//(none)

		bytes = P_CMD1 + 1;     //Bytes is always last+1
	}
	else if(cmd_type == CMD_WRITE)
	{
		buf[P_CMD1] = CMD_W(CMD_ENCODER);

		uint32_to_bytes((uint32_t)enc, &tmp0, &tmp1, &tmp2, &tmp3);

		//Arguments:
		buf[P_DATA1 + 0] = tmp0;
		buf[P_DATA1 + 1] = tmp1;
		buf[P_DATA1 + 2] = tmp2;
		buf[P_DATA1 + 3] = tmp3;

		bytes = P_DATA1 + 4;     //Bytes is always last+1
	}
	else
	{
		//Invalid
		flexsea_error(SE_INVALID_READ_TYPE);
		bytes = 0;
	}

	return bytes;
}

//Reception of an ENCODER command
void rx_cmd_encoder(uint8_t *buf)
{
	uint32_t numb = 0;
	int32_t tmp = 0;

	if(IS_CMD_RW(buf[P_CMD1]) == READ)
	{
		//Received a Read command from our master.

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Generate the reply:
		//===================

		numb = tx_cmd_encoder(buf[P_XID], CMD_WRITE, tmp_payload_xmit, PAYLOAD_BUF_LEN, qei_read());
		numb = comm_gen_str(tmp_payload_xmit, comm_str_485_1, numb);
		numb = COMM_STR_BUF_LEN;    //Fixed length for now
		rs485_puts(comm_str_485_1, numb);

		#endif	//BOARD_TYPE_FLEXSEA_MANAGE

	}
	else if(IS_CMD_RW(buf[P_CMD1]) == WRITE)
	{
		//Two options: from Master of from slave (a read reply)
		tmp = (int32_t)BYTES_TO_UINT32(buf[P_DATA1], buf[P_DATA1+1], buf[P_DATA1+2], buf[P_DATA1+3]);

		if(sent_from_a_slave(buf))
		{
			//We received a reply to our read request

			#ifdef BOARD_TYPE_FLEXSEA_PLAN

			_USE_PRINTF("Encoder = %i.\n", tmp);

			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
		else
		{
			//Master is writing a value to this board

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

			qei_write(tmp);

			#endif

		}
	}
}

//Transmission of a STRAIN command.
//TODO: add support for gains & offsets
//Note: the Reading part can be (and should be) done via Read All
uint32_t tx_cmd_strain(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len)
{
	uint32_t bytes = 0;
	uint8_t tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;

	//Fresh payload string:
	prepare_empty_payload(board_id, receiver, buf, len);

	//Command:
	buf[P_CMDS] = 1;                     //1 command in string

	if(cmd_type == CMD_READ)
	{
		buf[P_CMD1] = CMD_R(CMD_STRAIN);

		//Arguments:
		//(none)

		bytes = P_CMD1 + 1;     //Bytes is always last+1
	}
	else if(cmd_type == CMD_WRITE)
	{
		buf[P_CMD1] = CMD_W(CMD_STRAIN);

		//ToDo gains & offsets
		
		#ifdef BOARD_TYPE_FLEXSEA_STRAIN_AMP
			
		//Arguments:	
		uint16_to_bytes((uint16_t)strain1.ch[0].strain_filtered, &tmp0, &tmp1);		
		buf[P_DATA1 + 0] = tmp0;
		buf[P_DATA1 + 1] = tmp1;
		uint16_to_bytes((uint16_t)strain1.ch[1].strain_filtered, &tmp0, &tmp1);
		buf[P_DATA1 + 2] = tmp0;
		buf[P_DATA1 + 3] = tmp1;
		uint16_to_bytes((uint16_t)strain1.ch[2].strain_filtered, &tmp0, &tmp1);
		buf[P_DATA1 + 4] = tmp0;
		buf[P_DATA1 + 5] = tmp1;
		uint16_to_bytes((uint16_t)strain1.ch[3].strain_filtered, &tmp0, &tmp1);
		buf[P_DATA1 + 6] = tmp0;
		buf[P_DATA1 + 7] = tmp1;
		uint16_to_bytes((uint16_t)strain1.ch[4].strain_filtered, &tmp0, &tmp1);
		buf[P_DATA1 + 8] = tmp0;
		buf[P_DATA1 + 9] = tmp1;
		uint16_to_bytes((uint16_t)strain1.ch[5].strain_filtered, &tmp0, &tmp1);
		buf[P_DATA1 + 10] = tmp0;
		buf[P_DATA1 + 11] = tmp1;
		
		#endif 	//BOARD_TYPE_FLEXSEA_STRAIN_AMP

		bytes = P_DATA1 + 12;     //Bytes is always last+1
	}
	else
	{
		//Invalid
		flexsea_error(SE_INVALID_READ_TYPE);
		bytes = 0;
	}

	return bytes;
}

//Reception of a STRAIN command
void rx_cmd_strain(uint8_t *buf)
{
	uint32_t numb = 0;
	int32_t tmp = 0;
    struct strain_s *strainPtr = &strain1;

	if(IS_CMD_RW(buf[P_CMD1]) == READ)
	{
		//Received a Read command from our master.

		#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

		//Generate the reply:
		//===================

		//ToDo...

		#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
		
		#ifdef BOARD_TYPE_FLEXSEA_STRAIN_AMP
			
		//Generate the reply:
		//===================

		numb = tx_cmd_strain(buf[P_XID], CMD_WRITE, tmp_payload_xmit, PAYLOAD_BUF_LEN);
		numb = comm_gen_str(tmp_payload_xmit, comm_str_485_1, numb);
		numb = COMM_STR_BUF_LEN;    //Fixed length for now
		//rs485_puts(comm_str_485_1, numb);	
		
		#ifdef USE_USB
		usb_puts(comm_str_485_1, (numb));	
		#endif
			
		#endif 	//BOARD_TYPE_FLEXSEA_STRAIN_AMP
	}
	else if(IS_CMD_RW(buf[P_CMD1]) == WRITE)
	{
		//Two options: from Master of from slave (a read reply)

		if(sent_from_a_slave(buf))
		{
			//We received a reply to our read request

			#ifdef BOARD_TYPE_FLEXSEA_PLAN
				
            strainPtr->ch[0].strain_filtered = (uint16_t)BYTES_TO_UINT16(buf[P_DATA1], buf[P_DATA1+1]);
            strainPtr->ch[1].strain_filtered = (uint16_t)BYTES_TO_UINT16(buf[P_DATA1+2], buf[P_DATA1+3]);
            strainPtr->ch[2].strain_filtered = (uint16_t)BYTES_TO_UINT16(buf[P_DATA1+4], buf[P_DATA1+5]);
            strainPtr->ch[3].strain_filtered = (uint16_t)BYTES_TO_UINT16(buf[P_DATA1+6], buf[P_DATA1+7]);
            strainPtr->ch[4].strain_filtered = (uint16_t)BYTES_TO_UINT16(buf[P_DATA1+8], buf[P_DATA1+9]);
            strainPtr->ch[5].strain_filtered = (uint16_t)BYTES_TO_UINT16(buf[P_DATA1+10], buf[P_DATA1+11]);

			_USE_PRINTF("Strain[0] = %i.\n", strain[0].strain_filtered);

			#endif	//BOARD_TYPE_FLEXSEA_PLAN
		}
		else
		{
			//Master is writing a value to this board

			#ifdef BOARD_TYPE_FLEXSEA_EXECUTE

			qei_write(tmp);

			#endif	//BOARD_TYPE_FLEXSEA_EXECUTE
			
			#ifdef BOARD_TYPE_FLEXSEA_STRAIN_AMP

			//ToDo

			#endif	//BOARD_TYPE_FLEXSEA_STRAIN_AMP

		}
	}
}

#ifdef __cplusplus
}
#endif

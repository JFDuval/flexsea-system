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

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"
#include "../inc/flexsea_system.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

//Slave data:
unsigned char execute_1_data[SLAVE_READ_BUFFER_LEN];
unsigned char manage_1_data[SLAVE_READ_BUFFER_LEN];

#if defined(BOARD_TYPE_FLEXSEA_MANAGE) || defined(BOARD_TYPE_FLEXSEA_PLAN)

struct execute_s exec1, exec2, exec3, exec4;
struct ricnu_s ricnu_1;
struct manage_s manag1, manag2;
struct strain_s strain1;
struct in_control_s in_control_1;
struct gossip_s gossip1, gossip2;
struct battery_s batt1;

#endif	//defined(BOARD_TYPE_FLEXSEA_MANAGE) || defined(BOARD_TYPE_FLEXSEA_PLAN)

#if defined(BOARD_TYPE_FLEXSEA_PLAN)
struct user_data_s user_data_1;
#endif  //defined(BOARD_TYPE_FLEXSEA_PLAN)

#if defined(BOARD_TYPE_FLEXSEA_MANAGE)
struct user_data_s user_data;
#endif  //defined(BOARD_TYPE_FLEXSEA_MANAGE)

//****************************************************************************
// Function(s)
//****************************************************************************

//Initialize function pointer array
void init_flexsea_payload_ptr(void)
{
    int i = 0;

    //By default, they all point to 'flexsea_payload_catchall()'
    for(i = 0; i < 128; i++)
    {
        flexsea_payload_ptr[i] = &flexsea_payload_catchall;
    }

    //Associate pointers to your project-specific functions:
    //(index = command code)
    //======================================================

    //External:
    //flexsea_payload_ptr[CMD_ANALOG] = &rx_cmd_analog_in;
    //flexsea_payload_ptr[CMD_DIGITAL] = &rx_cmd_digital_in;
    flexsea_payload_ptr[CMD_PWRO] = &rx_cmd_exp_pwro;

    //Control:
    flexsea_payload_ptr[CMD_CTRL_MODE] = &rx_cmd_ctrl_mode;
    //flexsea_payload_ptr[CMD_CTRL_X_G] = &rx_cmd_ctrl_x_g;
    flexsea_payload_ptr[CMD_CTRL_I_G] = &rx_cmd_ctrl_i_g;
    flexsea_payload_ptr[CMD_CTRL_P_G] = &rx_cmd_ctrl_p_g;
    flexsea_payload_ptr[CMD_CTRL_Z_G] = &rx_cmd_ctrl_z_g;
    flexsea_payload_ptr[CMD_CTRL_O] = &rx_cmd_ctrl_o;
    flexsea_payload_ptr[CMD_CTRL_I] = &rx_cmd_ctrl_i;
    flexsea_payload_ptr[CMD_CTRL_P] = &rx_cmd_ctrl_p;
    //flexsea_payload_ptr[SHORTED_LEADS = &;
    flexsea_payload_ptr[CMD_IN_CONTROL] = &rx_cmd_in_control;

    //Data:
    flexsea_payload_ptr[CMD_ACQUI] = &rx_cmd_data_acqui;
    flexsea_payload_ptr[CMD_READ_ALL] = &rx_cmd_data_read_all;
    flexsea_payload_ptr[CMD_USER_DATA] = &rx_cmd_data_user;
    flexsea_payload_ptr[CMD_READ_ALL_RICNU] = &rx_cmd_data_read_all_ricnu;

    //Application:
    flexsea_payload_ptr[CMD_SPC1] = &rx_cmd_special_1;
    flexsea_payload_ptr[CMD_SPC2] = &rx_cmd_special_2;
    flexsea_payload_ptr[CMD_SPC3] = &rx_cmd_special_3;
    flexsea_payload_ptr[CMD_SPC4] = &rx_cmd_special_4;
    flexsea_payload_ptr[CMD_SPC5] = &rx_cmd_special_5;

    //Sensors:
    flexsea_payload_ptr[CMD_ENCODER] = &rx_cmd_encoder;
    flexsea_payload_ptr[CMD_STRAIN] = &rx_cmd_strain;
}

//Catch all function - does nothing
void flexsea_payload_catchall(uint8_t *buf){return;}

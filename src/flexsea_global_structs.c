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
	[This file] flexsea_global_structs: contains all the data structures
	used across the project
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************

#include "main.h"
#include "../inc/flexsea_global_structs.h"
#include "ext_input.h"

//****************************************************************************
// Variable(s)
//****************************************************************************

//Data structures:
struct execute_s exec1, exec2, exec3, exec4;
struct ricnu_s ricnu_1;
struct battery_s batt1;
struct manage_s manag1 = {.battPtr = &batt1}, manag2 = {.battPtr = &batt1};
struct strain_s strain1;
struct in_control_s in_control_1;
struct gossip_s gossip1, gossip2;
struct user_data_s user_data_1;

#if(defined BOARD_TYPE_FLEXSEA_PLAN || defined BOARD_TYPE_FLEXSEA_MANAGE)
int32_t exec1Ang, exec2Ang, exec3Ang, exec4Ang;
int32_t exec1AngVel, exec2AngVel, exec3AngVel, exec4AngVel;
#endif

//****************************************************************************
// Function(s)
//****************************************************************************

void initializeGlobalStructs()
{
#if(defined BOARD_TYPE_FLEXSEA_PLAN || defined BOARD_TYPE_FLEXSEA_MANAGE)
    exec1.enc_ang = &exec1Ang;
    exec2.enc_ang = &exec2Ang;
    exec3.enc_ang = &exec3Ang;
    exec4.enc_ang = &exec4Ang;

    exec1.enc_ang_vel = &exec1AngVel;
    exec2.enc_ang_vel = &exec2AngVel;
    exec3.enc_ang_vel = &exec3AngVel;
    exec4.enc_ang_vel = &exec4AngVel;
#elif(defined BOARD_TYPE_FLEXSEA_EXECUTE)
    exec1.enc_ang = &(as5047.signed_ang);
    exec1.enc_ang_vel = &(as5047.signed_ang_vel);
#endif
}

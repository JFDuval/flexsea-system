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

//TX functions:
//=============

//External:
uint32_t tx_cmd_digital_in(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len);
uint32_t tx_cmd_analog_in(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len);
uint32_t tx_cmd_exp_pwro(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								uint8_t pwro_pwm);
								
//Control:
uint32_t tx_cmd_ctrl_mode(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, int16_t ctrl);
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
						
//Data:
uint32_t tx_cmd_data_acqui(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, int16_t acqui);
uint32_t tx_cmd_data_read_all(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len);
uint32_t tx_cmd_data_user(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, uint8_t select_w);
uint32_t tx_cmd_data_read_all_ricnu(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len);

//Application:
uint32_t tx_cmd_ctrl_special_1(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								uint8_t controller_w, uint8_t controller, uint8_t encoder_w, int32_t encoder, \
								int16_t current, int16_t open_spd);						
uint32_t tx_cmd_ctrl_special_2(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								int16_t z_k, int16_t z_b, int16_t z_i, uint8_t rgb, uint8_t clutch, \
								uint8_t trapeze, int32_t posi, int32_t posf, int32_t spdm, int32_t acc);
uint32_t tx_cmd_ctrl_special_3(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								int16_t i_kp, int16_t i_ki, int16_t i_kd, int16_t w_curr);
uint32_t tx_cmd_ctrl_special_4(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								uint8_t controller_w1, uint8_t controller1, uint8_t encoder_w1, int32_t encoder1, \
								int16_t current1, int16_t open_spd1, \
								uint8_t controller_w2, uint8_t controller2, uint8_t encoder_w2, int32_t encoder2, \
								int16_t current2, int16_t open_spd2);
uint32_t tx_cmd_ctrl_special_5(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, \
								uint8_t slave, uint8_t controller, int16_t ctrl_i, int16_t ctrl_o);

//Sensors:
uint32_t tx_cmd_switch(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len);
uint32_t tx_cmd_encoder(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len, int32_t enc);
uint32_t tx_cmd_strain(uint8_t receiver, uint8_t cmd_type, uint8_t *buf, uint32_t len);

//****************************************************************************
// Definition(s):
//****************************************************************************

//Nicknames for commands:
//=======================
//Important: command codes have to be from 0 to 127 (7bits, unsigned)

//System commands:

#define CMD_NULL              			0
#define CMD_TEST                		1
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

//===================

//Board types:
#define FLEXSEA_PLAN                	1
#define FLEXSEA_MANAGE              	2
#define FLEXSEA_EXECUTE             	3
#define FLEXSEA_BATTERY					4
#define FLEXSEA_STRAIN					5
#define FLEXSEA_GOSSIP					6

//Board addresses:
#define FLEXSEA_DEFAULT             	0
#define FLEXSEA_PLAN_BASE              	10						//Plan: from 10 to 19
#define FLEXSEA_PLAN_1					(FLEXSEA_PLAN_BASE + 0)
#define FLEXSEA_MANAGE_BASE				20						//Manage: from 20 to 39
#define FLEXSEA_MANAGE_1            	(FLEXSEA_MANAGE_BASE + 0)
#define FLEXSEA_MANAGE_2            	(FLEXSEA_MANAGE_BASE + 1)
#define FLEXSEA_EXECUTE_BASE          	40						//Execute: from 40 to 59
#define FLEXSEA_EXECUTE_1				(FLEXSEA_EXECUTE_BASE + 0)
#define FLEXSEA_EXECUTE_2           	(FLEXSEA_EXECUTE_BASE + 1)
#define FLEXSEA_EXECUTE_3           	(FLEXSEA_EXECUTE_BASE + 2)
#define FLEXSEA_EXECUTE_4           	(FLEXSEA_EXECUTE_BASE + 3)
#define FLEXSEA_BATTERY_BASE          	60						//Battery: from 60 to 69
#define FLEXSEA_BATTERY_1				(FLEXSEA_BATTERY_BASE + 0)
#define FLEXSEA_STRAIN_BASE          	70						//Strain: from 70 to 79
#define FLEXSEA_STRAIN_1				(FLEXSEA_STRAIN_BASE + 0)
#define FLEXSEA_GOSSIP_BASE          	80						//Gossip: from 80 to 89
#define FLEXSEA_GOSSIP_1				(FLEXSEA_GOSSIP_BASE + 0)
#define FLEXSEA_GOSSIP_2				(FLEXSEA_GOSSIP_BASE + 1)

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

//Slave Read Buffer Positions - Execute:
#define SRB_EXECUTE_OFFSET				0
#define SRB_EXECUTE_STATUS				1			//Status byte. Includes ?
#define SRB_EXECUTE_ENC1_MSB			2			//Encoder #1, MSB
#define SRB_EXECUTE_ENC1_LSB			3			//Encoder #1, LSB
#define SRB_EXECUTE_AN0_MSB				4			//Analog 0, MSB
#define SRB_EXECUTE_AN0_LSB				5			//Analog 0, LSB
#define SRB_EXECUTE_AN1_MSB				6			//Analog 1, MSB
#define SRB_EXECUTE_AN1_LSB				7			//Analog 1, LSB
#define SRB_EXECUTE_CURRENT_MSB			8			//Motor current, MSB
#define SRB_EXECUTE_CURRENT_LSB			9			//Motor current, LSB
#define SRB_EXECUTE_BUS_VOLTAGE			10			//Bus voltage
#define SRB_EXECUTE_TEMPERATURE			11			//Temperature

//ToDO Redo that whole thing
//Slave Read Buffer Positions - Manage:
#define SRB_MANAGE_OFFSET				0
#define SRB_MANAGE_STATUS				1			//Status byte. Includes the 2 switches
#define SRB_MANAGE_DIGITAL_IO_B1		2			//First Digital IO byte: DIO0-7
#define SRB_MANAGE_DIGITAL_IO_B2		3			//Second Digital IO byte: DIO8-11
#define SRB_MANAGE_AN0_MSB				4			//MSB for Analog 0
#define SRB_MANAGE_AN0_LSB				5			//LSB
#define SRB_MANAGE_AN1_MSB				6			//MSB for Analog 1
#define SRB_MANAGE_AN1_LSB				7			//LSB
#define SRB_MANAGE_AN2_MSB				8			//MSB for Analog 2
#define SRB_MANAGE_AN2_LSB				9			//LSB
#define SRB_MANAGE_AN3_MSB				10			//MSB for Analog 3
#define SRB_MANAGE_AN3_LSB				11			//LSB
#define SRB_MANAGE_AN4_MSB				12			//MSB for Analog 4
#define SRB_MANAGE_AN4_LSB				13			//LSB
#define SRB_MANAGE_AN5_MSB				14			//MSB for Analog 5
#define SRB_MANAGE_AN5_LSB				15			//LSB
#define SRB_MANAGE_AN6_MSB				16			//MSB for Analog 6
#define SRB_MANAGE_AN6_LSB				17			//LSB
#define SRB_MANAGE_AN7_MSB				18			//MSB for Analog 7
#define SRB_MANAGE_AN7_LSB				19			//LSB

//Motor type:
#define MOTOR_BRUSHED					0
#define MOTOR_BRUSHLESS					1

//Generic:
#define DISABLED						0
#define ENABLED							1

//****************************************************************************
// Structure(s):
//****************************************************************************

//Note: most structures will have two versions. The original, aka 'raw'
//version has multiple data types, and is used by the different boards.
//The version with 'D' in the name is Decoded => it includes extra variables
//(often with the same name) holding decoded values (ex.: mV and not ADC counts)
//The 'D' values are always int32

//Inner structure for the gyro and the accelero
struct xyz_s
{
     int16_t x;
     int16_t y;
     int16_t z;
};

struct xyzD_s
{
     int32_t x;
     int32_t y;
     int32_t z;
};

struct execute_s
{
	struct xyz_s gyro;
	struct xyz_s accel;

	uint16_t strain;
	uint16_t analog[8];
	int16_t current;
	int32_t enc_display;
	int32_t enc_control;
	int32_t enc_commut;
	uint8_t volt_batt;	//+VB
	uint8_t volt_int;	//+VG
	uint8_t temp;
	uint8_t pwro;
	uint8_t status1;
	uint8_t status2;

    struct ctrl_s ctrl;
};

struct executeD_s
{
    //Raw values:
    struct execute_s exRaw;

    //Decoded fields:

    struct xyzD_s gyro;     //deg/s
    struct xyzD_s accel;    //mg

    int32_t strain;         //%
    int32_t current;        //mA
    int32_t volt_batt;      //mV
    int32_t volt_int;       //mV
    int32_t temp;           //Celsius x10
    int32_t analog[8];      //mV
};

struct manage_s
{
    struct xyz_s gyro;
    struct xyz_s accel;

    uint16_t analog[8];
    uint16_t digitalIn;

    uint8_t status1;

	uint8_t sw1;
	uint8_t sampling;
};

struct manageD_s
{
    //Raw values:
    struct manage_s mnRaw;

    //Decoded values:

    struct xyzD_s gyro;     //deg/s
    struct xyzD_s accel;    //mg

    int32_t analog[8];      //mV
};

//In Control Tool:
struct in_control_s
{
	uint8_t controller;
	int32_t setp;
	int32_t actual_val;
	int32_t error;
	int32_t output;
	int16_t pwm;
	uint8_t mot_dir;
	int16_t current;
	uint16_t combined;	//[CTRL2:0][MOT_DIR][PWM]
	
	int32_t r[4];
	int32_t w[4];
};

//Strain gauge amplifier:
//(structure supports both single and 6-ch amp)
struct strain_s
{
	//Config:     
    uint8_t offset;
	uint8_t gain;
    uint8_t oref;	
	
    //Raw ADC values:
    uint16_t strain_raw[4];
	uint16_t vo1;
    uint16_t vo2;
	 
	//Filtered value:
	uint16_t strain_filtered;
	uint16_t filtered_strain;
};

struct strainD_s
{
    //Raw values:
    struct strain_s stRaw;

    //Decoded values:

    //ToDo
    //...
};

//Special structure for the RIC/NU Knee. 'execute_s' + extra sensors.
struct ricnu_s
{
	//Execute:
	struct execute_s ex;
	
	//Extra sensors:
    uint16_t ext_strain[6];
};

struct ricnuD_s
{
    //Execute:
    struct executeD_s ex;

    //Extra sensors:
    uint16_t ext_strain[6];
};

struct user_data_s
{
	int32_t r[4];
	int32_t w[4];
};


struct gossip_s
{
    struct xyz_s gyro;
    struct xyz_s accel;
    struct xyz_s magneto;

    uint16_t capsense[4];

    uint16_t io[2];
    uint8_t status;
};

struct gossipD_s
{
    //Raw values:
    struct gossip_s goRaw;

    //Decoded values:

    struct xyzD_s gyro;     //deg/s
    struct xyzD_s accel;    //mg
    struct xyzD_s magneto;  //?
};

struct battery_s
{
    uint16_t voltage;
    int16_t current;
    uint8_t temp;
    uint8_t pushbutton;
    uint8_t status;
};

struct batteryD_s
{
    //Raw values:
    struct battery_s baRaw;

    //Decoded values:

    int32_t voltage;    //mV
    int32_t current;    //mA
    int32_t power;      //mW
    int32_t temp;       //C*10
};

//****************************************************************************
// Shared variable(s)
//****************************************************************************

#if defined(BOARD_TYPE_FLEXSEA_MANAGE) || defined(BOARD_TYPE_FLEXSEA_PLAN)

extern struct execute_s exec1, exec2, exec3, exec4;
extern struct ricnu_s ricnu_1;
extern struct manage_s manag1, manag2;
extern struct strain_s strain[6];
extern struct in_control_s in_control_1;
extern struct gossip_s gossip1, gossip2;
extern struct battery_s batt1;

extern struct executeD_s execD1, execD2, execD3, execD4;
extern struct ricnuD_s ricnuD_1;
extern struct manageD_s managD1, managD2;
extern struct strainD_s strainD[6];
extern struct gossipD_s gossipD1, gossipD2;
extern struct batteryD_s battD1;

#endif	//defined(BOARD_TYPE_FLEXSEA_MANAGE) || defined(BOARD_TYPE_FLEXSEA_PLAN)

#if defined(BOARD_TYPE_FLEXSEA_PLAN)
extern struct user_data_s user_data_1;
#endif  //defined(BOARD_TYPE_FLEXSEA_PLAN)

#if defined(BOARD_TYPE_FLEXSEA_MANAGE)
extern struct user_data_s user_data;
#endif  //defined(BOARD_TYPE_FLEXSEA_MANAGE)

#endif	//INC_FLEXSEA_SYSTEM_H

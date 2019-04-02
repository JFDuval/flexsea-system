#ifdef __cplusplus
extern "C" {
#endif

#if(defined BOARD_TYPE_FLEXSEA_PLAN || defined BOARD_TYPE_FLEXSEA_MANAGE && defined BOARD_SUBTYPE_RIGID)
#include "flexsea_device_spec_defs.h"
#endif

#include "flexsea_device_spec.h"
#include "flexsea_dataformats.h"
#include "flexsea_sys_def.h"
#include "stdlib.h"
#include "string.h"
#include "flexsea_user_structs.h"
#include "flexsea_board.h"

#define IS_FIELD_HIGH(i, map) ( (map)[(i)/32] & (1U << ((i)%32)) )
#define SET_MAP_HIGH(i, map) ( (map)[(i)/32] |= (1U << ((i)%32)) )
#define SET_MAP_LOW(i, map) ( (map)[(i)/32] &= (~(1U << ((i)%32))) )

/** Interface specs */
FlexseaDeviceSpec fx_none_spec = {
		.numFields = 0,
		.fieldLabels = NULL,
		.fieldTypes = NULL
};

// FX_RIGID spec starts here
// -------------------------

#if((defined BOARD_TYPE_FLEXSEA_MANAGE  && defined BOARD_SUBTYPE_RIGID && HW_VER < 10) || defined BOARD_TYPE_FLEXSEA_PLAN)

const char* _rigid_fieldlabels[_rigid_mn_numFields] = 	{"rigid", 			"id",													// METADATA			2 2
														"state_time",																// STATE TIME		1 3
														"accelx", 	"accely", 	"accelz", 	"gyrox", 	"gyroy", 	"gyroz",		// IMU				6 9

														"mot_ang", 		"mot_vel", 	"mot_acc", 										// MOTOR KIN		3 12
														"mot_cur", 		"mot_volt",													// MOTOR ELEC		2 14
														"batt_volt", 	"batt_curr",												// BATTERY			2 16
														"temp", 		"status",													// STATS			2 18
														"genvar_0", "genvar_1", "genvar_2", "genvar_3", "genvar_4", 				// GEN VARS			5 23
														"genvar_5", "genvar_6", "genvar_7", "genvar_8", "genvar_9", 				// GEN VARS			5 28
														"ank_ang", "ank_vel", 														// ANKLE			2 30
#ifdef DEPHY
														"ank_from_mot", "ank_torque",												// ANKLE			4 32
														"cur_stpt",	"step_energy", "walking_state", "gait_state" 					// CONTROLLER		4 36
#endif
};

const uint8_t _rigid_field_formats[_rigid_mn_numFields] =	{FORMAT_8U, 	FORMAT_16U,												// METADATA			2 2
														FORMAT_32U,																	// STATE TIME		1 3
														FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S ,	// IMU				6 9

														FORMAT_32S, FORMAT_32S, FORMAT_32S,											// MOTOR KIN		3 12
														FORMAT_32S, FORMAT_32S,														// MOTOR ELEC		2 14
														FORMAT_16U, FORMAT_16S,														// BATTERY			2 16
														FORMAT_8S, 	FORMAT_16U,														// STATS			2 18
														FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S,					// GEN VARS			5 23
														FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S,					// GEN VARS			5 28
														FORMAT_16S, FORMAT_16S, 													// ANKLE			2 30
#ifdef DEPHY
														FORMAT_16S,	FORMAT_16S, 													// ANKLE			2 32
														FORMAT_32S, FORMAT_16S, FORMAT_8S, FORMAT_8S								// CONTROLLER		4 36
#endif
};

FlexseaDeviceSpec fx_rigid_spec = {
		.numFields = _rigid_mn_numFields,
		.fieldLabels = _rigid_fieldlabels,
		.fieldTypes = _rigid_field_formats
};

#endif

#if((defined BOARD_TYPE_FLEXSEA_MANAGE  && defined BOARD_SUBTYPE_RIGID && HW_VER < 10))

#define PTR2(x) (uint8_t*)&(x)

// only defined on boards not on plan
uint8_t* _rigid_field_pointers[_rigid_mn_numFields] =	{	0,	0,																					// METADATA			2 2
														PTR2(rigid1.ctrl.timestamp),																// STATE TIME		1 3
														(uint8_t*)&rigid1.mn.accel.x, (uint8_t*)&rigid1.mn.accel.y, (uint8_t*)&rigid1.mn.accel.z,	// IMU				3 6
														(uint8_t*)&rigid1.mn.gyro.x, (uint8_t*)&rigid1.mn.gyro.y, (uint8_t*)&rigid1.mn.gyro.z,		// IMU 				3 9
														PTR2(rigid1.ex._enc_ang_), PTR2(rigid1.ex._enc_ang_vel_), PTR2(rigid1.ex.mot_acc), 			// MOTOR KIN		3 12
														PTR2(rigid1.ex.mot_current), PTR2(rigid1.ex.mot_volt),	 						 			// MOTOR ELEC		2 14
														PTR2(rigid1.re.vb), PTR2(rigid1.re.current), 												// BATTERY			2 16
														PTR2(rigid1.re.temp),  PTR2(rigid1.re.status),												// STATS			2 18
														(uint8_t*)(rigid1.mn.genVar),	(uint8_t*)(rigid1.mn.genVar+1),								// GEN VARS
														(uint8_t*)(rigid1.mn.genVar+2), (uint8_t*)(rigid1.mn.genVar+3),								// GEN VARS
														(uint8_t*)(rigid1.mn.genVar+4), (uint8_t*)(rigid1.mn.genVar+5),								// GEN VARS
														(uint8_t*)(rigid1.mn.genVar+6), (uint8_t*)(rigid1.mn.genVar+7),								// GEN VARS
														(uint8_t*)(rigid1.mn.genVar+8), (uint8_t*)(rigid1.mn.genVar+9),								// GEN VARS			10 28
#ifdef DEPHY
														PTR2(rigid1.ctrl._ank_ang_deg_), PTR2(rigid1.ctrl._ank_vel_), 								// ANKLE			2 30
#else
														PTR2(rigid1.ex._joint_ang_), PTR2(rigid1.ex._joint_ang_vel_),
#endif
};

#undef PTR2

uint32_t *fx_dev_timestamp = &rigid1.ctrl.timestamp;
const FlexseaDeviceSpec *fx_this_device_spec = &fx_rigid_spec;
const uint8_t ** _dev_data_pointers = (const uint8_t **) _rigid_field_pointers;
const uint8_t * _dev_field_formats = _rigid_field_formats;

// -------------------------
// FX_RIGID spec ends here

#endif // BOARD_TYPE_FLEXSEA_MANAGE

#if(defined BOARD_TYPE_FLEXSEA_EXECUTE  || defined BOARD_TYPE_FLEXSEA_PLAN)
	
// FX_EXECUTE spec starts here
// -------------------------
#define _execute_numFields 16
#define	_dev_numFields _execute_numFields

															// type
const char* _execute_fieldlabels[_execute_numFields] = 		{"execute", "id", "state_time", \
																"accelx", "accely", "accelz", \
																"accelx", "accely", "accelz", \
																"encoder", "current", \
																"analog0", "analog1", \
																"vb", "vg", "temp"};
const uint8_t _execute_field_formats[_execute_numFields] =	{FORMAT_8U, FORMAT_16U, FORMAT_32U, \
															FORMAT_16S, FORMAT_16S, FORMAT_16S, \
															FORMAT_16S, FORMAT_16S, FORMAT_16S, \
															FORMAT_32S, FORMAT_32S, FORMAT_16U, FORMAT_16U, \
															FORMAT_32S, FORMAT_32S, FORMAT_32S};

#define PTR2(x) (uint8_t*)&(x)

// only defined on boards not on plan
uint8_t* _execute_field_pointers[_execute_numFields] =	{	(uint8_t*)0, (uint8_t*)0, PTR2(rigid1.ctrl.timestamp),
														(uint8_t*)&exec1.accel.x, (uint8_t*)&exec1.accel.y, (uint8_t*)&exec1.accel.z, \
														(uint8_t*)&exec1.gyro.x, (uint8_t*)&exec1.gyro.y, (uint8_t*)&exec1.gyro.z, \
														PTR2(exec1._enc_ang_), (uint8_t*)&exec1.current, \
														(uint8_t*)&exec1.analog[0], (uint8_t*)&exec1.analog[1], \
														(uint8_t*)&exec1.decoded.volt_batt, (uint8_t*)&exec1.decoded.volt_int, (uint8_t*)&exec1.decoded.temp};

#undef PTR2

FlexseaDeviceSpec fx_execute_spec = {
		.numFields = _execute_numFields,
		.fieldLabels = _execute_fieldlabels,
		.fieldTypes = _execute_field_formats
};
// -------------------------
// FX_EXECUTE spec ends here
#endif // BOARD_TYPE_FLEXSEA_EXECUTE

//Prototype boards.
#if(defined BOARD_TYPE_FLEXSEA_PROTOTYPE  || defined BOARD_TYPE_FLEXSEA_PLAN)
#include "user-proto.h"
#if (ACTIVE_PROJECT == PROJECT_SCCD)
	
// FX_PROTOTYPE spec starts here
// -------------------------
#define _proto_numFields 16
#define	_dev_numFields _proto_numFields

															// type
const char* _proto_fieldlabels[_proto_numFields] = 		{"execute", "id", "state_time", \
																"accelx", "accely", "accelz", \
																"accelx", "accely", "accelz", \
																"encoder", "current", \
																"analog0", "analog1", \
																"vb", "vg", "temp"};
const uint8_t _proto_field_formats[_proto_numFields] =	{FORMAT_8U, FORMAT_16U, FORMAT_32U, \
															FORMAT_16S, FORMAT_16S, FORMAT_16S, \
															FORMAT_16S, FORMAT_16S, FORMAT_16S, \
															FORMAT_32S, FORMAT_32S, FORMAT_16U, FORMAT_16U, \
															FORMAT_32S, FORMAT_32S, FORMAT_32S};

#define PTR2(x) (uint8_t*)&(x)

// only defined on boards not on plan
uint8_t* _proto_field_pointers[_proto_numFields] =	{	(uint8_t*)0, (uint8_t*)0, PTR2(rigid1.ctrl.timestamp),
														(uint8_t*)&exec1.accel.x, (uint8_t*)&exec1.accel.y, (uint8_t*)&exec1.accel.z, \
														(uint8_t*)&exec1.gyro.x, (uint8_t*)&exec1.gyro.y, (uint8_t*)&exec1.gyro.z, \
														PTR2(exec1._enc_ang_), (uint8_t*)&exec1.current, \
														(uint8_t*)&exec1.analog[0], (uint8_t*)&exec1.analog[1], \
														(uint8_t*)&exec1.decoded.volt_batt, (uint8_t*)&exec1.decoded.volt_int, (uint8_t*)&exec1.decoded.temp};

#undef PTR2

FlexseaDeviceSpec fx_proto_spec = {
		.numFields = _proto_numFields,
		.fieldLabels = _proto_fieldlabels,
		.fieldTypes = _proto_field_formats};

uint32_t *fx_dev_timestamp = &rigid1.ctrl.timestamp;
const FlexseaDeviceSpec *fx_this_device_spec = &fx_proto_spec;
const uint8_t ** _dev_data_pointers = (const uint8_t **) _proto_field_pointers;
const uint8_t * _dev_field_formats = _proto_field_formats;

// -------------------------
// FX_PROTOTYPE spec ends here

#else
	
	#warning "BOARD_TYPE_FLEXSEA_PROTOTYPE only supports PROJECT_SCCD!"

#endif // (ACTIVE_PROJECT == PROJECT_SCCD)
#endif	//(defined BOARD_TYPE_FLEXSEA_PROTOTYPE  || defined BOARD_TYPE_FLEXSEA_PLAN)

// FX_MANAGE spec starts here
// -------------------------

#if(defined BOARD_TYPE_FLEXSEA_PLAN || (defined BOARD_TYPE_FLEXSEA_MANAGE && !(defined BOARD_SUBTYPE_RIGID)))

#define _manage_numFields 13									// type
const char* _manage_fieldlabels[_manage_numFields] = 		{	"manage", 			"id",													// METADATA			2 2
																"state_time",																// STATE TIME		1 3
																"accelx", 	"accely", 	"accelz", 	"gyrox", 	"gyroy", 	"gyroz",		// IMU
																"analog0", "analog1", "analog2", "analog3"};								// Analog
const uint8_t _manage_field_formats[_manage_numFields] =	{	FORMAT_8U, 	FORMAT_16U,														// METADATA			2 2
																FORMAT_32U,																	// STATE TIME		1 3
																FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S,
																FORMAT_16U, FORMAT_16U, FORMAT_16U, FORMAT_16U};

FlexseaDeviceSpec fx_manage_spec = {
		.numFields = _manage_numFields,
		.fieldLabels = _manage_fieldlabels,
		.fieldTypes = _manage_field_formats
};

#endif // BOARD_TYPE_FLEXSEA_MANAGE

#if(defined BOARD_TYPE_FLEXSEA_MANAGE && !(defined BOARD_SUBTYPE_RIGID))

#define PTR2(x) (uint8_t*)&(x)

// only defined on boards not on plan
uint8_t* _manage_field_pointers[_manage_numFields] =	{	(uint8_t*)0, (uint8_t*)0, \
															PTR2(rigid1.ctrl.timestamp),																// STATE TIME		1 3
															(uint8_t*)&rigid1.mn.accel.x, (uint8_t*)&rigid1.mn.accel.y, (uint8_t*)&rigid1.mn.accel.z,	// IMU				3 6
															(uint8_t*)&rigid1.mn.gyro.x, (uint8_t*)&rigid1.mn.gyro.y, (uint8_t*)&rigid1.mn.gyro.z,		// IMU
															(uint8_t*)&rigid1.mn.analog[0], (uint8_t*)&rigid1.mn.analog[1], 							// Analog 0-1
															(uint8_t*)&rigid1.mn.analog[2], (uint8_t*)&rigid1.mn.analog[3]};							// Analog 2-3
#undef PTR2

uint32_t *fx_dev_timestamp = &rigid1.ctrl.timestamp;
const FlexseaDeviceSpec *fx_this_device_spec = &fx_manage_spec;
const uint8_t ** _dev_data_pointers = (const uint8_t **) _manage_field_pointers;
const uint8_t * _dev_field_formats = _manage_field_formats;

#endif


#if(defined BOARD_TYPE_FLEXSEA_MANAGE)

// definition moved to flexsea_device_spec_m7.c

#include "flexsea_device_spec_defs.h"

#elif(defined BOARD_TYPE_FLEXSEA_PLAN)

const FlexseaDeviceSpec *fx_this_device_spec = &fx_none_spec;
uint32_t fx_empty_timestamp;
uint32_t *fx_dev_timestamp = &fx_empty_timestamp;
const uint8_t** _dev_data_pointers = NULL;
	
#elif(defined BOARD_TYPE_FLEXSEA_EXECUTE)

#define	_dev_numFields _execute_numFields

uint32_t *fx_dev_timestamp = &rigid1.ctrl.timestamp;
const FlexseaDeviceSpec *fx_this_device_spec = &fx_execute_spec;
const uint8_t** _dev_data_pointers = (const uint8_t**)_execute_field_pointers;
const uint8_t * _dev_field_formats = _execute_field_formats;

#endif

// initialization goes in payload_ptr initialization which is a hack :(

FlexseaDeviceSpec deviceSpecs[NUM_DEVICE_TYPES];

#ifndef BOARD_TYPE_FLEXSEA_PLAN
uint32_t fx_active_bitmap[FX_BITMAP_WIDTH_C];
uint16_t fx_num_fields_active = 0;

const uint8_t *_device_active_field_pointers[_dev_numFields];
uint8_t _device_active_field_lengths[_dev_numFields];

const uint16_t const* read_num_fields_active = &fx_num_fields_active;
const uint8_t const* const* read_device_active_field_pointers = (const uint8_t const* const*)_device_active_field_pointers;
const uint8_t const* read_device_active_field_lengths =_device_active_field_lengths;

void setActiveFieldsByMap(uint32_t *map)
{

	const uint8_t * dev_field_formats = fx_this_device_spec->fieldTypes;

	int i, j=0;
	for(i=0;i<_dev_numFields; ++i)
	{
		if(IS_FIELD_HIGH(i, map))
		{
			_device_active_field_pointers[j] = _dev_data_pointers[i];
			_device_active_field_lengths[j] = FORMAT_SIZE_MAP[dev_field_formats[i]];
			++j;
		}
	}

	fx_num_fields_active = j;
}
#else

void initializeDeviceSpecs()
{
	deviceSpecs[FX_NONE] = fx_none_spec;
	deviceSpecs[FX_RIGID] = fx_rigid_spec;
	deviceSpecs[FX_EXECUTE] = fx_execute_spec;
	deviceSpecs[FX_MANAGE] = fx_manage_spec;
	deviceSpecs[FX_PROTOTYPE] = fx_proto_spec;
}

#endif

#ifdef __cplusplus
}
#endif

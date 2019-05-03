#include "flexsea_device_spec.h"
#include "flexsea_device_spec_defs.h"
#include "flexsea_dataformats.h"
#include "flexsea_user_structs.h"
#include "flexsea_board.h"
#include "user-mn-DpEb42.h"

const char* _rigid_m7_fieldlabels[_rigid_m7_numFields] =
														{"rigid", 			"id",													// METADATA			2 2
														"state_time",																// STATE TIME		1 3
														"accelx", 	"accely", 	"accelz", 	"gyrox", 	"gyroy", 	"gyroz",		// IMU				6 9

														"mot_ang", 		"mot_vel", 	"mot_acc", 										// MOTOR KIN		3 12
														"mot_cur", 		"mot_volt",													// MOTOR ELEC		2 14
														"batt_volt", 	"batt_curr",												// BATTERY			2 16
														"temp", 		"status",													// STATS			2 18
														"genvar_0", "genvar_1", "genvar_2", "genvar_3", "genvar_4", 				// GEN VARS			5 23
														"genvar_5", "genvar_6", "genvar_7", "genvar_8", "genvar_9", 				// GEN VARS			5 28
														"ank_ang", "ank_vel", 														// ANKLE			2 30
														"ank_from_mot", "ank_torque",												// ANKLE			4 32
														"cur_stpt",	"step_energy", "walking_state", "gait_state" 					// CONTROLLER		4 36
};

const uint8_t _rigid_m7_field_formats[_rigid_m7_numFields] =
														{FORMAT_8U, 	FORMAT_16U,													// METADATA			2 2
														FORMAT_32U,																	// STATE TIME		1 3
														FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S ,	// IMU				6 9

														FORMAT_32S, FORMAT_32S, FORMAT_32S,											// MOTOR KIN		3 12
														FORMAT_32S, FORMAT_32S,														// MOTOR ELEC		2 14
														FORMAT_16U, FORMAT_16S,														// BATTERY			2 16
														FORMAT_8S, 	FORMAT_16U,														// STATS			2 18
														FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S,					// GEN VARS			5 23
														FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S,					// GEN VARS			5 28
														FORMAT_16S, FORMAT_16S, 													// ANKLE			2 30
														FORMAT_16S,	FORMAT_16S, 													// ANKLE			2 32
														FORMAT_32S, FORMAT_16S, FORMAT_8S, FORMAT_8S								// CONTROLLER		4 36
};

#define PTR2(x) (uint8_t*)&(x)
#define RGD_STRUCT dpRigid

// only defined on boards not on plan
uint8_t* _rigid_m7_field_pointers[_rigid_m7_numFields] =
														{	0,	0,																							// METADATA			2 2
														PTR2(RGD_STRUCT.ctrl.timestamp),																	// STATE TIME		1 3
														(uint8_t*)&RGD_STRUCT.mn.accel.x, (uint8_t*)&RGD_STRUCT.mn.accel.y, (uint8_t*)&RGD_STRUCT.mn.accel.z,	// IMU				3 6
														(uint8_t*)&RGD_STRUCT.mn.gyro.x, (uint8_t*)&RGD_STRUCT.mn.gyro.y, (uint8_t*)&RGD_STRUCT.mn.gyro.z,		// IMU 				3 9
														PTR2(RGD_STRUCT.ex._enc_ang_), PTR2(RGD_STRUCT.ex._enc_ang_vel_), PTR2(RGD_STRUCT.ex.mot_acc), 			// MOTOR KIN		3 12
														PTR2(RGD_STRUCT.ex.mot_current), PTR2(RGD_STRUCT.ex.mot_volt),	 						 			// MOTOR ELEC		2 14
														PTR2(rigid1.re.vb), PTR2(rigid1.re.current), 														// BATTERY			2 16
														PTR2(rigid1.re.temp),  PTR2(rigid1.mn.status),														// STATS			2 18
														(uint8_t*)(RGD_STRUCT.mn.genVar),	(uint8_t*)(RGD_STRUCT.mn.genVar+1),								// GEN VARS
														(uint8_t*)(RGD_STRUCT.mn.genVar+2), (uint8_t*)(RGD_STRUCT.mn.genVar+3),								// GEN VARS
														(uint8_t*)(RGD_STRUCT.mn.genVar+4), (uint8_t*)(RGD_STRUCT.mn.genVar+5),								// GEN VARS
														(uint8_t*)(RGD_STRUCT.mn.genVar+6), (uint8_t*)(RGD_STRUCT.mn.genVar+7),								// GEN VARS
														(uint8_t*)(RGD_STRUCT.mn.genVar+8), (uint8_t*)(RGD_STRUCT.mn.genVar+9),								// GEN VARS			10 28
														PTR2(RGD_STRUCT.ctrl._ank_ang_deg_), PTR2(RGD_STRUCT.ctrl._ank_vel_), 								// ANKLE			2 30
														PTR2(RGD_STRUCT.ctrl._ank_ang_from_mot_),  0,														// ANKLE			2 32
														PTR2(RGD_STRUCT.ex.ctrl.current.setpoint_val), PTR2(RGD_STRUCT.ctrl.step_energy),					// CONTROLLER		2 34
														PTR2(RGD_STRUCT.ctrl.walkingState), PTR2(RGD_STRUCT.ctrl.gaitState),								// CONTROLLER		2 36
};

FlexseaDeviceSpec fx_rigid_m7_spec = {
		.numFields = _rigid_m7_numFields,
		.fieldLabels = _rigid_m7_fieldlabels,
		.fieldTypes = _rigid_m7_field_formats
};

uint32_t *fx_dev_timestamp = &RGD_STRUCT.ctrl.timestamp;
const FlexseaDeviceSpec *fx_this_device_spec = &fx_rigid_m7_spec;
const uint8_t ** _dev_data_pointers = (const uint8_t **) _rigid_m7_field_pointers;
const uint8_t * _dev_field_formats = _rigid_m7_field_formats;

#undef RGD_STRUCT
#undef PTR2

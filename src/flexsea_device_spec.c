#ifdef __cplusplus
extern "C" {
#endif

#include "flexsea_device_spec.h"
#include "flexsea_dataformats.h"
#include "flexsea_sys_def.h"
#include "stdlib.h"
#include "string.h"
#include "flexsea_user_structs.h"
#include "flexsea_board.h"

#if(defined BOARD_TYPE_FLEXSEA_EXECUTE  || defined BOARD_TYPE_FLEXSEA_PLAN)

// FX_EXECUTE spec starts here
// -------------------------
#define _execute_numFields 16

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

//
// FX_MANAGE spec starts here
// -------------------------

#if(defined BOARD_TYPE_FLEXSEA_PLAN)

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

#endif // BOARD_TYPE_FLEXSEA_PLAN

#if(defined BOARD_TYPE_FLEXSEA_PLAN)

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

#ifdef BOARD_TYPE_FLEXSEA_PLAN

/** Interface specs */
FlexseaDeviceSpec fx_none_spec = {
		.numFields = 0,
		.fieldLabels = NULL,
		.fieldTypes = NULL
};

// initialization goes in payload_ptr initialization which is a hack :(

FlexseaDeviceSpec deviceSpecs[NUM_DEVICE_TYPES];

void initializeDeviceSpecs()
{
	deviceSpecs[FX_NONE] = fx_none_spec;
	deviceSpecs[FX_RIGID] = fx_rigid_spec;
	deviceSpecs[FX_EXECUTE] = fx_execute_spec;
	deviceSpecs[FX_MANAGE] = fx_manage_spec;
	deviceSpecs[FX_PROTO] = fx_proto_spec;
}

#endif

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "flexsea_device_spec.h"
#include "flexsea_dataformats.h"
#include "flexsea_system.h"
#include "stdlib.h"
#include "string.h"

#define IS_FIELD_HIGH(i, map) ( (map)[(i)/32] & (1 << ((i)%32)) )
#define SET_MAP_HIGH(i, map) ( (map)[(i)/32] |= (1 << ((i)%32)) )
#define SET_MAP_LOW(i, map) ( (map)[(i)/32] &= (~(1 << ((i)%32))) )

/** Interface specs */
FlexseaDeviceSpec fx_none_spec = {
		.numFields = 0,
		.fieldLabels = NULL,
		.fieldTypes = NULL
};

// FX_RIGID spec starts here
// -------------------------
#define _rigid_numFields 8								// type
const char* _rigid_fieldlabels[_rigid_numFields] = 		{"rigid", 			"id", 	"accelx", 	"accely", 	"accelz", 	"gyrox", 	"gyroy", 	"gyroz"};
const uint8_t _rigid_field_formats[_rigid_numFields] =	{FORMAT_8U, 	FORMAT_16U, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S };

// only defined on boards not on plan
uint8_t* _rigid_field_pointers[_rigid_numFields] =	{	(uint8_t*)&fx_dev_type, (uint8_t*)&fx_dev_id, \
														(uint8_t*)&rigid1.mn.accel.x, (uint8_t*)&rigid1.mn.accel.y, (uint8_t*)&rigid1.mn.accel.z, \
														(uint8_t*)&rigid1.mn.gyro.x, (uint8_t*)&rigid1.mn.gyro.y, (uint8_t*)&rigid1.mn.gyro.z};
FlexseaDeviceSpec fx_rigid_spec = {
		.numFields = _rigid_numFields,
		.fieldLabels = _rigid_fieldlabels,
		.fieldTypes = _rigid_field_formats
};
// -------------------------
// FX_RIGID spec ends here
// FX_EXECUTE spec starts here
// -------------------------
#define _execute_numFields 5								// type
const char* _execute_fieldlabels[_execute_numFields] = 		{"execute", 		"id", 	"accelx", 	"accely", 	"accelz"};
const uint8_t _execute_field_formats[_execute_numFields] =	{FORMAT_8U, 	FORMAT_16U, FORMAT_16S, FORMAT_16S, FORMAT_16S };

// only defined on boards not on plan
uint8_t* _execute_field_pointers[_execute_numFields] =	{	(uint8_t*)&fx_dev_type, (uint8_t*)&fx_dev_id, \
														(uint8_t*)&exec1.accel.x, (uint8_t*)&exec1.accel.y, (uint8_t*)&exec1.accel.z };
FlexseaDeviceSpec fx_execute_spec = {
		.numFields = _execute_numFields,
		.fieldLabels = _execute_fieldlabels,
		.fieldTypes = _execute_field_formats
};
// -------------------------
// FX_EXECUTE spec ends here
// FX_MANAGE spec starts here
// -------------------------
#define _manage_numFields 4									// type
const char* _manage_fieldlabels[_manage_numFields] = 		{"manage", 		"id", 		"accelx", 	"accely"};
const uint8_t _manage_field_formats[_manage_numFields] =	{FORMAT_8U, 	FORMAT_16U, FORMAT_16S, FORMAT_16S };

// only defined on boards not on plan
uint8_t* _manage_field_pointers[_manage_numFields] =	{	(uint8_t*)&fx_dev_type, (uint8_t*)&fx_dev_id, \
														(uint8_t*)&manag1.accel.x, (uint8_t*)&manag1.accel.y };
FlexseaDeviceSpec fx_manage_spec = {
		.numFields = _manage_numFields,
		.fieldLabels = _manage_fieldlabels,
		.fieldTypes = _manage_field_formats
};
// -------------------------
// FX_MANAGE spec ends here

#if(defined BOARD_TYPE_FLEXSEA_MANAGE)
uint16_t fx_dev_id = 101;
uint8_t fx_dev_type = FX_RIGID;
const FlexseaDeviceSpec *fx_this_device_spec = &fx_rigid_spec;
const uint8_t** _dev_data_pointers = _rigid_field_pointers;
#elif(defined BOARD_TYPE_FLEXSEA_PLAN)
uint16_t fx_dev_id = 999;
uint8_t fx_dev_type = FX_NONE;
const FlexseaDeviceSpec *fx_this_device_spec = &fx_none_spec;
const uint8_t** _dev_data_pointers = NULL;
#elif(defined BOARD_TYPE_FLEXSEA_EXECUTE)
uint16_t fx_dev_id = 102;
uint8_t fx_dev_type = FX_EXECUTE;
const FlexseaDeviceSpec *fx_this_device_spec = &fx_execute_spec;
const uint8_t** _dev_data_pointers = _execute_field_pointers;
#endif

// initialization goes in payload_ptr initialization which is a hack :(
FlexseaDeviceSpec deviceSpecs[NUM_DEVICE_TYPES];

#ifdef BOARD_TYPE_FLEXSEA_PLAN
FlexseaDeviceSpec connectedDeviceSpecs[MAX_CONNECTED_DEVICES];
uint8_t fx_spec_numConnectedDevices = 0;
uint8_t* deviceData[MAX_CONNECTED_DEVICES];

void addConnectedDevice(uint8_t devType, uint16_t devId)
{
	if(fx_spec_numConnectedDevices == MAX_CONNECTED_DEVICES)
	{
		return;
	}

	FlexseaDeviceSpec *ds = &connectedDeviceSpecs[fx_spec_numConnectedDevices];
	switch(devType) {
	case(FX_RIGID):
		*ds = fx_rigid_spec;
		break;
	case(FX_EXECUTE):
		*ds = fx_execute_spec;
		break;
	case(FX_MANAGE):
		*ds = fx_manage_spec;
		break;

	}

	uint16_t i, totalSpaceNeeded = 0;
	for(i=0; i<ds->numFields;i++)
		totalSpaceNeeded += FORMAT_SIZE_MAP[ds->fieldTypes[i]];

	if(totalSpaceNeeded)
	{
		if(deviceData[fx_spec_numConnectedDevices])
			deviceData[fx_spec_numConnectedDevices] = realloc(deviceData[fx_spec_numConnectedDevices], totalSpaceNeeded);
		else
			deviceData[fx_spec_numConnectedDevices] = malloc(totalSpaceNeeded);

		if(deviceData[fx_spec_numConnectedDevices])
			memset(deviceData[fx_spec_numConnectedDevices], 0, totalSpaceNeeded);
	}

	if(deviceData[fx_spec_numConnectedDevices])
	{
		// set the device id and type in the data vector
		deviceData[fx_spec_numConnectedDevices][0] = devType;
		uint16_t *devIdPtr = (uint16_t*)(deviceData[fx_spec_numConnectedDevices]+1);
		(*devIdPtr) = devId;

		fx_spec_numConnectedDevices++;
	}
}

void initializeDeviceSpecs()
{
	deviceSpecs[FX_NONE] = fx_none_spec;
	deviceSpecs[FX_RIGID] = fx_rigid_spec;
	deviceSpecs[FX_EXECUTE] = fx_execute_spec;
	deviceSpecs[FX_MANAGE] = fx_manage_spec;

	int i;

	for(i=0;i<MAX_CONNECTED_DEVICES;i++)
	{
		deviceData[i] = 0;
	}
}

#else
void addConnectedDevice(uint8_t devType, uint16_t devId) {}
void initializeDeviceSpecs() {}
#endif

#ifdef __cplusplus
}
#endif

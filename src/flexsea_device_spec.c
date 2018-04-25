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
		.fieldTypes = NULL,
		.fieldPointers = NULL
};

uint16_t fx_dev_id = 101;
uint8_t fx_dev_type = FX_RIGID_SPEC;

/** FX_RIGID */
#define _rigid_numFields 8
extern const char* _rigid_fieldlabels[_rigid_numFields];
extern const uint8_t _rigid_field_formats[_rigid_numFields];
															// type
const char* _rigid_fieldlabels[_rigid_numFields] = 		{"rigid", 			"id", 	"accelx", 	"accely", 	"accelz", 	"gyrox", 	"gyroy", 	"gyroz"};
const uint8_t _rigid_field_formats[_rigid_numFields] =	{FORMAT_8U, 	FORMAT_16U, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S, FORMAT_16S };

// only defined on boards not on plan
uint8_t* _rigid_field_pointers[_rigid_numFields] =	{	(uint8_t*)&fx_dev_type, (uint8_t*)&fx_dev_id, \
														(uint8_t*)&rigid1.mn.accel.x, (uint8_t*)&rigid1.mn.accel.y, (uint8_t*)&rigid1.mn.accel.z, \
														(uint8_t*)&rigid1.mn.gyro.x, (uint8_t*)&rigid1.mn.gyro.y, (uint8_t*)&rigid1.mn.gyro.z};
FlexseaDeviceSpec fx_rigid_spec = {
		.numFields = 7,
		.fieldLabels = _rigid_fieldlabels,
		.fieldTypes = _rigid_field_formats,
		.fieldPointers = _rigid_field_pointers
};

// initialization goes in payload_ptr initialization which is a hack :(
FlexseaDeviceSpec deviceSpecs[NUM_DEVICE_TYPES];

const FlexseaDeviceSpec *fx_this_device_spec = &fx_rigid_spec;

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
	if(devType == FX_RIGID_SPEC)
		*ds = fx_rigid_spec;
	else
	{
		*ds = fx_none_spec;
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
	deviceSpecs[FX_NONE_SPEC] = fx_none_spec;
	deviceSpecs[FX_RIGID_SPEC] = fx_rigid_spec;

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

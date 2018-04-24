/*
 * flexsea_device_spec.h
 *
 *  Created on: Apr 24, 2018
 *      Author: Dephy Inc
 */

#ifndef FLEXSEA_SYSTEM_INC_FLEXSEA_DEVICE_SPEC_H_
#define FLEXSEA_SYSTEM_INC_FLEXSEA_DEVICE_SPEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "flexsea_dataformats.h"


/* Flexsea Device C Interface defined here - FlexseaDeviceSpec */
typedef struct FlexseaDeviceSpec_s {
	char* name;
	uint8_t numFields;

	const char** fieldLabels;

	const uint8_t* fieldTypes;
	uint8_t** fieldPointers;
} FlexseaDeviceSpec;

/* Specs exist for the following devices */
#define NUM_DEVICE_TYPES 2
#define FX_NONE_SPEC 0
#define FX_RIGID_SPEC 1

/* this array contains all the device specs */
extern FlexseaDeviceSpec deviceSpecs[NUM_DEVICE_TYPES];

/* defines and externs needed by plan only */
#ifdef BOARD_TYPE_FLEXSEA_PLAN
	#define MAX_CONNECTED_DEVICES 10
	extern FlexseaDeviceSpec connectedDeviceSpecs[MAX_CONNECTED_DEVICES];
	extern uint8_t* deviceData[MAX_CONNECTED_DEVICES];
	extern uint8_t fx_spec_numConnectedDevices;

	// convenience define for accessing deviceData
	// only works on little endian systems
	// TODO: actually implement a big_endian/little_endian define so this isn't a bug waiting to happen
	#ifdef BIG_ENDIAN
	#else
		#define DEVICESPEC_TYPE_BY_IDX(i) ( *(deviceData[i]) )
		#define DEVICESPEC_UUID_BY_IDX(i) ( *((uint16_t*)(deviceData[i] + 1)) )
	#endif
#else

#endif //BOARD_TYPE_FLEXSEA_PLAN

void addConnectedDevice(uint8_t devType, uint16_t devId);
void initializeDeviceSpecs();
extern const FlexseaDeviceSpec *fx_this_device_spec;

/* Related to max number of fields, should probably call it max num fields.. */
#define MAX_BYTES_OF_FLAGS 3

#ifdef __cplusplus
}
#endif

#endif /* FLEXSEA_SYSTEM_INC_FLEXSEA_DEVICE_SPEC_H_ */

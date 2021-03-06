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
#include "flexsea_sys_def.h"

#define IS_FIELD_HIGH(i, map) ( (map)[(i)/32] & (1 << ((i)%32)) )
#define SET_MAP_HIGH(i, map) ( (map)[(i)/32] |= (1 << ((i)%32)) )
#define SET_MAP_LOW(i, map) ( (map)[(i)/32] &= (~(1 << ((i)%32))) )
#define SET_FIELD_HIGH(i, map)( (map)[(i)/32] |= (1U << (i%32))  )

/* Flexsea Device C Interface defined here - FlexseaDeviceSpec */
typedef struct FlexseaDeviceSpec_s {
	char* name;
	uint8_t numFields;

	const char** fieldLabels;

	const uint8_t* fieldTypes;
} FlexseaDeviceSpec;

/* Specs exist for the following devices */
#define NUM_DEVICE_TYPES 6

/* this array contains all the device specs */
extern FlexseaDeviceSpec deviceSpecs[NUM_DEVICE_TYPES];

/* defines and externs needed by plan only */
#ifdef BOARD_TYPE_FLEXSEA_PLAN
	void initializeDeviceSpecs();
	#define MAX_CONNECTED_DEVICES 10
	extern FlexseaDeviceSpec connectedDeviceSpecs[MAX_CONNECTED_DEVICES];
	extern uint8_t* deviceData[MAX_CONNECTED_DEVICES];
	extern uint8_t fx_spec_numConnectedDevices;
	#ifdef BIG_ENDIAN
		// TODO: nothing was here but appears like it should have one of these
		// DEVICESPEC definitions
	#else
		#define DEVICESPEC_TYPE_BY_IDX(i) ( *(deviceData[i]) )
		#define DEVICESPEC_UUID_BY_IDX(i) ( *((uint16_t*)(deviceData[i] + 1)) )
	#endif

	// giving access to device specific device specs for plan but it would be nice
	// to just return a pointer to the structure
	extern FlexseaDeviceSpec fx_bms_spec;
	extern FlexseaDeviceSpec fx_exo_spec;

#else // not plan!

	extern const uint16_t * read_num_fields_active;
	extern const uint8_t** read_device_active_field_pointers;
	extern const uint8_t* read_device_active_field_lengths;
	void setActiveFieldsByMap(uint32_t *map);

#endif //BOARD_TYPE_FLEXSEA_PLAN

extern const FlexseaDeviceSpec *fx_this_device_spec;
extern const uint8_t** _dev_data_pointers;
extern uint32_t *fx_dev_timestamp;
extern uint16_t fx_dev_id;
extern uint8_t fx_dev_type;
extern uint16_t _dev_numFields;

/* Related to max number of fields, should probably call it max num fields.. */
#define FX_BITMAP_WIDTH_C 3

extern uint32_t fx_active_bitmap[FX_BITMAP_WIDTH_C];

#ifdef __cplusplus
}
#endif

#endif /* FLEXSEA_SYSTEM_INC_FLEXSEA_DEVICE_SPEC_H_ */

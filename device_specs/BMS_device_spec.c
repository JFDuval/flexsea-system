/*
 * BMS_device_spec.c
 *
 * AUTOGENERATED FILE! ONLY EDIT IF YOU ARE A MACHINE!
 *
 *
 *  Created on: 2019-06-20 11:09:57.070180
 *      Author: Dephy Inc
 */

#include "flexsea_device_spec.h"
#include "BMS_device_spec.h"
#include "flexsea_user_structs.h"

	#define DEVICE_FIELD_COUNT 25

const char *bms_spec_labels[DEVICE_FIELD_COUNT] = {
	"prototype",
	"id",
	"state_time",
	"cells[0].mV",
	"cells[1].mV",
	"cells[2].mV",
	"cells[3].mV",
	"cells[4].mV",
	"cells[5].mV",
	"cells[6].mV",
	"cells[7].mV",
	"cells[8].mV",
	"bms_state",
	"current",
	"timer",
	"balancing",
	"stack_voltage",
	"pack_imbalance",
	"temp[0]",
	"temp[1]",
	"temp[2]",
	"genVar[0]",
	"genVar[1]",
	"genVar[2]",
	"genVar[3]",
};

const uint8_t bms_spec_formats[DEVICE_FIELD_COUNT] = {
	FORMAT_8U,
	FORMAT_16U,
	FORMAT_32U,
	FORMAT_16U,
	FORMAT_16U,
	FORMAT_16U,
	FORMAT_16U,
	FORMAT_16U,
	FORMAT_16U,
	FORMAT_16U,
	FORMAT_16U,
	FORMAT_16U,
	FORMAT_8S,
	FORMAT_32S,
	FORMAT_32U,
	FORMAT_16U,
	FORMAT_16U,
	FORMAT_16S,
	FORMAT_8S,
	FORMAT_8S,
	FORMAT_8S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
};
	
	FlexseaDeviceSpec fx_bms_spec = {
			.numFields = DEVICE_FIELD_COUNT,
			.fieldLabels = bms_spec_labels,
			.fieldTypes = bms_spec_formats
	};

#ifndef BOARD_TYPE_FLEXSEA_PLAN

#include "sccd.h"
#include ""
uint8_t *bms_spec_variables[DEVICE_FIELD_COUNT] = {
	0,
	0,
	(uint8_t *)&rigid1.ctrl.timestamp,
	(uint8_t *)&cells[0].mV,
	(uint8_t *)&cells[1].mV,
	(uint8_t *)&cells[2].mV,
	(uint8_t *)&cells[3].mV,
	(uint8_t *)&cells[4].mV,
	(uint8_t *)&cells[5].mV,
	(uint8_t *)&cells[6].mV,
	(uint8_t *)&cells[7].mV,
	(uint8_t *)&cells[8].mV,
	(uint8_t *)&bmsGui.bms_state,
	(uint8_t *)&bmsGui.current,
	(uint8_t *)&bmsGui.timer,
	(uint8_t *)&bmsGui.balancing,
	(uint8_t *)&bmsGui.stack_voltage,
	(uint8_t *)&bmsGui.pack_imbalance,
	(uint8_t *)&bmsGui.temp[0],
	(uint8_t *)&bmsGui.temp[1],
	(uint8_t *)&bmsGui.temp[2],
	(uint8_t *)&bmsGui.genVar[0],
	(uint8_t *)&bmsGui.genVar[1],
	(uint8_t *)&bmsGui.genVar[2],
	(uint8_t *)&bmsGui.genVar[3],
};

uint32_t *fx_dev_timestamp = &rigid1.ctrl.timestamp;
const FlexseaDeviceSpec *fx_this_device_spec = &fx_bms_spec;
const uint8_t **_dev_data_pointers = (const uint8_t **) bms_spec_variables;
const uint8_t *_dev_field_formats = bms_spec_formats;

uint32_t fx_active_bitmap[FX_BITMAP_WIDTH_C];
uint16_t fx_num_fields_active = 0;

const uint8_t *_device_active_field_pointers[DEVICE_FIELD_COUNT];
uint8_t _device_active_field_lengths[DEVICE_FIELD_COUNT];

const uint16_t *read_num_fields_active = &fx_num_fields_active;
const uint8_t **read_device_active_field_pointers = (const uint8_t **)_device_active_field_pointers;
const uint8_t *read_device_active_field_lengths =_device_active_field_lengths;

void setActiveFieldsByMap(uint32_t *map)
{
	const uint8_t * dev_field_formats = fx_this_device_spec->fieldTypes;

	int i, j=0;
	for(i=0;i<DEVICE_FIELD_COUNT; ++i)
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

#endif // BOARD_TYPE_FLEXSEA_PLAN

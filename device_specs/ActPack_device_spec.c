/*
 * ActPack_device_spec.c
 *
 * AUTOGENERATED FILE! ONLY EDIT IF YOU ARE A MACHINE!
 *
 *
 *  Created on: 2019-07-08 10:06:33.415668
 *      Author: Dephy Inc
 */

#include "flexsea_device_spec.h"
#include "ActPack_device_spec.h"
#include "flexsea_user_structs.h"

	#define DEVICE_FIELD_COUNT 32

const char *actpack_spec_labels[DEVICE_FIELD_COUNT] = {
	"rigid",
	"id",
	"state_time",
	"accelx",
	"accely",
	"accelz",
	"gyrox",
	"gyroy",
	"gyroz",
	"mot_ang",
	"mot_vel",
	"mot_acc",
	"mot_cur",
	"mot_volt",
	"batt_volt",
	"batt_curr",
	"temperature",
	"status_Mn",
	"status_Ex",
	"status_Re",
	"genvar_0",
	"genvar_1",
	"genvar_2",
	"genvar_3",
	"genvar_4",
	"genvar_5",
	"genvar_6",
	"genvar_7",
	"genvar_8",
	"genvar_9",
	"ank_ang",
	"ank_vel",
};

const uint8_t actpack_spec_formats[DEVICE_FIELD_COUNT] = {
	FORMAT_8U,
	FORMAT_16U,
	FORMAT_32U,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_32S,
	FORMAT_32S,
	FORMAT_32S,
	FORMAT_32S,
	FORMAT_32S,
	FORMAT_16U,
	FORMAT_16S,
	FORMAT_8S,
	FORMAT_8U,
	FORMAT_8U,
	FORMAT_8U,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
	FORMAT_16S,
};
	
	FlexseaDeviceSpec fx_actpack_spec = {
			.numFields = DEVICE_FIELD_COUNT,
			.fieldLabels = actpack_spec_labels,
			.fieldTypes = actpack_spec_formats
	};

#ifndef BOARD_TYPE_FLEXSEA_PLAN

uint8_t *actpack_spec_variables[DEVICE_FIELD_COUNT] = {
	0,
	0,
	(uint8_t *)&rigid1.ctrl.timestamp,
	(uint8_t *)&rigid1.mn.accel.x,
	(uint8_t *)&rigid1.mn.accel.y,
	(uint8_t *)&rigid1.mn.accel.z,
	(uint8_t *)&rigid1.mn.gyro.x,
	(uint8_t *)&rigid1.mn.gyro.y,
	(uint8_t *)&rigid1.mn.gyro.z,
	(uint8_t *)&rigid1.ex._enc_ang_,
	(uint8_t *)&rigid1.ex._enc_ang_vel_,
	(uint8_t *)&rigid1.ex.mot_acc,
	(uint8_t *)&rigid1.ex.mot_current,
	(uint8_t *)&rigid1.ex.mot_volt,
	(uint8_t *)&rigid1.re.vb,
	(uint8_t *)&rigid1.re.current,
	(uint8_t *)&rigid1.re.temp,
	(uint8_t *)&rigid1.mn.status,
	(uint8_t *)&rigid1.ex.status,
	(uint8_t *)&rigid1.re.status,
	(uint8_t *)&rigid1.mn.genVar[0],
	(uint8_t *)&rigid1.mn.genVar[1],
	(uint8_t *)&rigid1.mn.genVar[2],
	(uint8_t *)&rigid1.mn.genVar[3],
	(uint8_t *)&rigid1.mn.genVar[4],
	(uint8_t *)&rigid1.mn.genVar[5],
	(uint8_t *)&rigid1.mn.genVar[6],
	(uint8_t *)&rigid1.mn.genVar[7],
	(uint8_t *)&rigid1.mn.genVar[8],
	(uint8_t *)&rigid1.mn.genVar[9],
	(uint8_t *)&rigid1.ex._joint_ang_,
	(uint8_t *)&rigid1.ex._joint_ang_vel_,
};

uint32_t *fx_dev_timestamp = &rigid1.ctrl.timestamp;
const FlexseaDeviceSpec *fx_this_device_spec = &fx_actpack_spec;
const uint8_t **_dev_data_pointers = (const uint8_t **) actpack_spec_variables;
const uint8_t *_dev_field_formats = actpack_spec_formats;

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

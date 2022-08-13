/**
 * @file mpu925x_gyroscope.c
 * @author Ceyhun Şen
 * @brief Settings for gyroscope.
 */

/*
 * MPU-925X Driver is a device driver for MPU-9250 and MPU-9255 sensors.
 * Copyright (C) 2022  Ceyhun Şen
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see:
 * <https://www.gnu.org/licenses/>.
 * */

#include "mpu925x_internals.h"

/**
 * @brief Set gyroscope full-scale range.
 * @param mpu925x MPU-925X struct pointer.
 * @param scale Gyroscope full-scale range to be set.
 * */
void mpu925x_set_gyroscope_scale(mpu925x_t *mpu925x, mpu925x_gyroscope_scale scale)
{
	// Get GYRO_FS_SEL value.
	uint8_t GYRO_FS_SEL = scale << 3;

	// Set gyrocope scale.
	switch (scale) {
		default:
		case mpu925x_250dps:
			mpu925x->settings.gyroscope.lsb = GYROSCOPE_SCALE_250_DPS;
			break;
		case mpu925x_500dps:
			mpu925x->settings.gyroscope.lsb = GYROSCOPE_SCALE_500_DPS;
			break;
		case mpu925x_1000dps:
			mpu925x->settings.gyroscope.lsb = GYROSCOPE_SCALE_1000_DPS;
			break;
		case mpu925x_2000dps:
			mpu925x->settings.gyroscope.lsb = GYROSCOPE_SCALE_2000_DPS;
			break;
	}

	// Write register.
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.general.address, GYRO_CONFIG, &GYRO_FS_SEL, 1);
}

/**
 * @brief Set gyroscope digital low pass filter setting.
 * @param mpu925x MPU-925X struct pointer.
 * @param g_fchoice Gyroscope f_choice bits.
 * @param dlpf Digital low pass filter setting.
 * */
void mpu925x_set_gyroscope_dlpf(mpu925x_t *mpu925x, uint8_t g_fchoice, uint8_t dlpf)
{
	uint8_t buffer;

	// Get bypass value.
	buffer = ~g_fchoice & 0b11;
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.general.address, GYRO_CONFIG, &buffer, 1);

	// Set dlpf.
	buffer = dlpf & 0b111;
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.general.address, CONFIG, &buffer, 1);
}

/**
 * @brief Get and set gyroscope offset cancellation values.
 * @param mpu925x MPU-925X struct pointer.
 * @param sampling_amount Sampling amount for rotation values.
 * @see mpu925x_get_gyroscope_offset
 * @see mpu925x_set_gyroscope_offset
 * */
void mpu925x_gyroscope_offset_cancellation(mpu925x_t *mpu925x, uint16_t sampling_amount)
{
	int16_t offset[3];

	mpu925x_get_gyroscope_offset(mpu925x, sampling_amount, offset);
	mpu925x_set_gyroscope_offset(mpu925x, offset);
}

/**
 * @brief Get gyroscope offset cancellation values.
 * @param mpu925x MPU-925X struct pointer.
 * @param sampling_amount Sampling amount for rotation values.
 * @param offset 3d array which holds gyroscope offset cancellation values.
 * @see mpu925x_set_gyroscope_offset
 * */
void mpu925x_get_gyroscope_offset(mpu925x_t *mpu925x, uint16_t sampling_amount, int16_t *offset)
{
	// Offsets of x, y and z axis are calculated seperately.
	for (uint8_t i = 0; i < 3; i++) {
		int16_t rotation_value;
		float average = 0.0;

		// Read rotation data and get average.
		for (uint16_t j = 0; j < sampling_amount; j++) {
			mpu925x_get_rotation_raw(mpu925x);
			rotation_value = mpu925x->sensor_data.rotation_raw[i];

			// Avoid overflow.
			float a = 1.0 / (j + 1.0);
			float b = 1.0 - a;
			average = a * rotation_value + b * average;
		}

		// Get FS_SEL value.
		uint8_t fs_sel = mpu925x->settings.gyroscope.scale;

		offset[i] = (int16_t)(-average / 4.0 * powerof2(fs_sel));
	}
}

/**
 * @brief Set gyroscope offset cancellation values.
 * @param mpu925x MPU-925X struct pointer.
 * @param offset 3d array which holds gyroscope offset cancellation values.
 * @see mpu925x_get_gyroscope_offset
 * */
void mpu925x_set_gyroscope_offset(mpu925x_t *mpu925x, int16_t *offset)
{
	uint8_t buffer[2];

	// Set gyro offset.
	for (uint8_t i = 0; i < 3; i++) {
		buffer[0] = offset[i] >> 8;
		buffer[1] = offset[i];
		mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.general.address, XG_OFFSET_H + i * 2, buffer, 2);
	}
}

/**
 * @file mpu925x_accelerometer.c
 * @author Ceyhun Şen
 * @brief Settings for accelerometer.
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
 * @brief Get accelerometer bias.
 * @param mpu925x MPU-925X struct pointer.
 * @param bias Buffer for acceleromter bias values.
 * */
static void mpu925x_accelerometer_get_bias(mpu925x_t *mpu925x, int16_t *bias)
{
	uint8_t buffer[8];

	// Read bias registers.
	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.general.address, XA_OFFSET_H, buffer, 8);

	// Convert raw values to 16 bit integers.
	for (uint8_t i = 0; i < 3; i++) {
		bias[i] = convert_8_bit_to_16_bit(buffer[i * 3], buffer[i * 3 + 1]);
	}
}

/**
 * @brief Set accelerometer full-scale range.
 * @param mpu925x MPU-925X struct pointer.
 * @param scale Accelerometer full-scale range to be set.
 * */
void mpu925x_accelerometer_set_scale(mpu925x_t *mpu925x, mpu925x_accelerometer_scale scale)
{
	// Get ACCEL_FS_SEL value.
	uint8_t ACCEL_FS_SEL = scale << 3;

	// Write to register.
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.general.address, ACCEL_CONFIG, &ACCEL_FS_SEL, 1);

	// Save accelerometer LSB.
	mpu925x->settings.accelerometer.lsb = INT16_MAX / power_of_2(scale) / 2 + 1;
}

/**
 * @brief Set accelerometer digital low pass filter setting.
 * @param mpu925x MPU-925X struct pointer.
 * @param dlpf Bandwith choice for dlpf.
 * */
void mpu925x_accelerometer_set_dlpf(mpu925x_t *mpu925x, mpu925x_accelerometer_dlpf dlpf)
{
	uint8_t buffer;

	// Get register value according to setting.
	switch (dlpf) {
		case mpu925x_1_13k:
			buffer = 1 << 3 | (0 << 2);
			break;
		default:
			buffer = (0 << 3) | (dlpf << 2);
			break;
	}

	// Write to the register.
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.general.address, ACCEL_CONFIG_2, &buffer, 1);
}

/**
 * @brief Get and set accelerometer offset cancellation values.
 * @param mpu925x MPU-925X struct pointer.
 * @param sampling_amount Sampling amount for acceleration values.
 * @see mpu925x_accelerometer_get_offset
 * @see mpu925x_accelerometer_set_offset
 * */
void mpu925x_accelerometer_offset_cancellation(mpu925x_t *mpu925x, uint16_t sampling_amount)
{
	int16_t offset[3];

	// Get and set offset.
	mpu925x_accelerometer_get_offset(mpu925x, sampling_amount, offset);
	mpu925x_accelerometer_set_offset(mpu925x, offset);
}

/**
 * @brief Get accelerometer offset cancellation value.
 * @param mpu925x MPU-925X struct pointer.
 * @param sampling_amount Sampling amount for acceleration values.
 * @param offset Buffer for offset cancellation values.
 * */
void mpu925x_accelerometer_get_offset(mpu925x_t *mpu925x, uint16_t sampling_amount, int16_t *offset)
{
	float average[3] = {0.0, 0.0, 0.0};

	for (uint16_t i = 0; i < sampling_amount; i++) {
		// Read data.
		mpu925x_get_acceleration_raw(mpu925x);

		for (uint8_t j = 0; j < 3; j++) {
			// Avoid overflow.	
			float a = 1 / (i + 1);
			float b = 1 - a;
			average[j] = a * mpu925x->sensor_data.acceleration_raw[j] + b * average[j];
		}
	}

	// Remove gravity depending on orientation.
	switch (mpu925x->settings.general.orientation) {
		case mpu925x_x_plus:
			average[0] += mpu925x->settings.accelerometer.lsb;
			break;
		case mpu925x_x_minus:
			average[0] -= mpu925x->settings.accelerometer.lsb;
			break;
		case mpu925x_y_plus:
			average[1] += mpu925x->settings.accelerometer.lsb;
			break;
		case mpu925x_y_minus:
			average[1] -= mpu925x->settings.accelerometer.lsb;
			break;
		default:
		case mpu925x_z_plus:
			average[2] += mpu925x->settings.accelerometer.lsb;
			break;
		case mpu925x_z_minus:
			average[2] -= mpu925x->settings.accelerometer.lsb;
			break;
	}

	// Get accelerometer bias.
	int16_t bias[3];
	mpu925x_accelerometer_get_bias(mpu925x, bias);

	// Get divider based on scale.
	uint8_t divider = mpu925x->settings.accelerometer.lsb / ACCELEROMETER_SCALE_16G;

	// Get final offset.
	for (uint8_t i = 0; i < 3; i++) {
		offset[i] = (int16_t)(average[i] / divider);
		offset[i] = bias[i] - (offset[i] & ~1);
	}
}

/**
 * @brief Set accelerometer offset cancellation value.
 * @param mpu925x MPU-925X struct pointer.
 * @param offset Buffer for offset cancellation values.
 * */
void mpu925x_accelerometer_set_offset(mpu925x_t *mpu925x, int16_t *offset)
{
	uint8_t buffer[2];

	// Set accelerometer offsets for each axis.
	for (uint8_t i = 0; i < 3; i++) {
		// Convert data to big-endian.
		buffer[0] = (uint8_t)((offset[i] >> 8) & 0xFF);
		buffer[1] = (uint8_t)(offset[i] & 0xFF);

		// Write to registers.
		mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.general.address, XA_OFFSET_H + (i * 3), buffer, 2);
	}
}

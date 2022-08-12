/**
 * @file
 * @author Ceyhun Şen
 * @brief Settings for MPU-925X driver.
 * */

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
#include <stdint.h>

/*******************************************************************************
 * General Settings
 ******************************************************************************/

/**
 * @brief Set sample rate divider.
 * @param mpu925x MPU-925X struct pointer.
 * @param sample_rate_divider Sample rate divider sentence.
 * */
void mpu925x_set_sample_rate_divider(mpu925x_t *mpu925x, uint8_t sample_rate_divider)
{
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, SMPLRT_DIV, &sample_rate_divider, 1);
}

/**
 * @brief Set clock source.
 * @param mpu925x MPU-925X struct pointer.
 * @param clock Clock select option.
 * @see mpu925x_clock
 * */
void mpu925x_set_clock_source(mpu925x_t *mpu925x, mpu925x_clock clock)
{
	uint8_t buffer = 0;

	switch (clock) {
		case mpu925x_internal_20_hz_clock:
			buffer = 0;
			break;
		default:
		case mpu925x_auto_select_pll:
			buffer = 1;
			break;
	}

	mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, PWR_MGMT_1, &buffer, 1, 0b11111000);
}

/*******************************************************************************
 * Accelerometer settings
 ******************************************************************************/

/**
 * @brief Set accelerometer full-scale range.
 * @param mpu925x MPU-925X struct pointer.
 * @param scale Accelerometer full-scale range to be set.
 * */
void mpu925x_set_accelerometer_scale(mpu925x_t *mpu925x, mpu925x_accelerometer_scale scale)
{
	// Get ACCEL_FS_SEL value.
	uint8_t ACCEL_FS_SEL = scale << 3;

	// Set accelerometer lsb.
	mpu925x->settings.acceleration_lsb = INT16_MAX / powerof2(scale) / 2 + 1;

	// Write register.
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, ACCEL_CONFIG, &ACCEL_FS_SEL, 1);
}

/**
 * @brief Set accelerometer digital low pass filter setting.
 * @param mpu925x MPU-925X struct pointer.
 * @param a_fchoice Accelerometer fchoice bit.
 * @param dlpf Digital low pass filter choice.
 * */
void mpu925x_set_accelerometer_dlpf(mpu925x_t *mpu925x, uint8_t a_fchoice, uint8_t dlpf)
{
	uint8_t buffer;

	// Invert value to get bypass value.
	buffer = (~a_fchoice & 1) << 3;

	buffer |= dlpf & 0b111;

	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, ACCEL_CONFIG_2, &buffer, 1);
}

/**
 * @brief Get and set accelerometer offset cancellation values.
 * @param mpu925x MPU-925X struct pointer.
 * @param sampling_amount Sampling amount for acceleration values.
 * @see mpu925x_get_accelerometer_offset
 * @see mpu925x_set_accelerometer_offset
 * */
void mpu925x_accelerometer_offset_cancellation(mpu925x_t *mpu925x, uint16_t sampling_amount)
{
	int16_t offset[3];

	mpu925x_get_accelerometer_offset(mpu925x, sampling_amount, offset);
	mpu925x_set_accelerometer_offset(mpu925x, offset);
}

/**
 * @brief Get accelerometer offset cancellation value.
 * @param mpu925x MPU-925X struct pointer.
 * @param sampling_amount Sampling amount for acceleration values.
 * @param offset 3d array which will hold accelerometer offset cancellation values.
 * */
void mpu925x_get_accelerometer_offset(mpu925x_t *mpu925x, uint16_t sampling_amount, int16_t *offset)
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
	switch (mpu925x->settings.orientation) {
		case mpu925x_x_plus:
			average[0] += mpu925x->settings.acceleration_lsb;
			break;
		case mpu925x_x_minus:
			average[0] -= mpu925x->settings.acceleration_lsb;
			break;
		case mpu925x_y_plus:
			average[1] += mpu925x->settings.acceleration_lsb;
			break;
		case mpu925x_y_minus:
			average[1] -= mpu925x->settings.acceleration_lsb;
			break;
		default:
		case mpu925x_z_plus:
			average[2] += mpu925x->settings.acceleration_lsb;
			break;
		case mpu925x_z_minus:
			average[2] -= mpu925x->settings.acceleration_lsb;
			break;
	}

	// Get bias.
	int16_t bias[3];
	mpu925x_get_accelerometer_bias(mpu925x, bias);

	// Get divider based on scale.
	uint8_t divider = mpu925x->settings.acceleration_lsb / ACCELEROMETER_SCALE_16G;

	for (uint8_t i = 0; i < 3; i++) {
		offset[i] = (int16_t)(average[i] / divider);
		offset[i] = bias[i] - (offset[i] & ~1);
	}
}

/**
 * @brief Set accelerometer offset cancellation value.
 * @param mpu925x MPU-925X struct pointer.
 * @param offset 3d array which holds accelerometer offset cancellation values.
 * */
void mpu925x_set_accelerometer_offset(mpu925x_t *mpu925x, int16_t *offset)
{
	uint8_t buffer[2];

	// Set accelerometer offsets.
	for (uint8_t i = 0; i < 3; i++) {
		buffer[0] = (uint8_t)((offset[i] >> 8) & 0xFF);
		buffer[1] = (uint8_t)(offset[i] & 0xFF);
		mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, XA_OFFSET_H + (i * 3), buffer, 2);
	}
}

/*******************************************************************************
 * Gyroscope settings
 ******************************************************************************/

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
			mpu925x->settings.gyroscope_lsb = GYROSCOPE_SCALE_250_DPS;
			break;
		case mpu925x_500dps:
			mpu925x->settings.gyroscope_lsb = GYROSCOPE_SCALE_500_DPS;
			break;
		case mpu925x_1000dps:
			mpu925x->settings.gyroscope_lsb = GYROSCOPE_SCALE_1000_DPS;
			break;
		case mpu925x_2000dps:
			mpu925x->settings.gyroscope_lsb = GYROSCOPE_SCALE_2000_DPS;
			break;
	}

	// Write register.
	mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, GYRO_CONFIG, &GYRO_FS_SEL, 1, 0b11100111);
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
	mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, GYRO_CONFIG, &buffer, 1, 0b11111100);

	// Set dlpf.
	buffer = dlpf & 0b111;
	mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, CONFIG, &buffer, 1, 0b11111000);
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
		uint8_t fs_sel = mpu925x->settings.gyroscope_scale;

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
		mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, XG_OFFSET_H + i * 2, buffer, 2);
	}
}

/*******************************************************************************
 * Magnetometer settings
 ******************************************************************************/

/**
 * @brief Set magnetometer measurement mode.
 * @param mpu925x MPU-925X struct pointer.
 * @param measurement_mode Measurement mode for magnetometer to be set.
 * @see mpu925x_magnetometer_measurement_mode
 * */
void mpu925x_set_magnetometer_measurement_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_measurement_mode measurement_mode)
{
	uint8_t buffer;

	// Save measurement mode.
	mpu925x->settings.measurement_mode = measurement_mode;

	mpu925x->master_specific.bus_read(mpu925x, AK8963_ADDRESS, CNTL1, &buffer, 1);
	buffer &= 0b11110000;

	switch (measurement_mode) {
		case mpu925x_power_down_mode:
			buffer |= 0b0000;
			break;
		case mpu925x_single_measurement_mode:
			buffer |= 0b0001;
			break;
		case mpu925x_continuous_measurement_mode_1:
			buffer |= 0b0010;
			break;
		default:
		case mpu925x_continuous_measurement_mode_2:
			buffer |= 0b0110;
			break;
		case mpu925x_external_trigger_measurement_mode:
			buffer |= 0b0100;
			break;
		case mpu925x_self_test_mode:
			buffer |= 0b1000;
			break;
		case mpu925x_fuse_rom_access_mode:
			buffer |= 0b1111;
			break;
	}

	mpu925x->master_specific.bus_write(mpu925x, AK8963_ADDRESS, CNTL1, &buffer, 1);

	mpu925x->master_specific.delay_ms(mpu925x, 100);
}

/**
 * @brief Set magnetometer bit mode.
 * @param mpu925x MPU-925X struct pointer.
 * @param bit_mode Bit mode for magnetometer to be set.
 * @see mpu925x_magnetometer_bit_mode
 * */
void mpu925x_set_magnetometer_bit_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_bit_mode bit_mode)
{
	uint8_t buffer;

	// Save bit mode.
	mpu925x->settings.bit_mode = bit_mode;

	mpu925x->master_specific.bus_read(mpu925x, AK8963_ADDRESS, CNTL1, &buffer, 1);
	buffer &= 0b11101111;

	switch (bit_mode) {
		case mpu925x_14_bit:
			buffer |= 0 << 4;
			mpu925x->settings.magnetometer_lsb = MAGNETOMETER_SCALE_14_BIT;
			break;
		default:
		case mpu925x_16_bit:
			buffer |= 1 << 4;
			mpu925x->settings.magnetometer_lsb = MAGNETOMETER_SCALE_16_BIT;
			break;
	}

	mpu925x->master_specific.bus_write(mpu925x, AK8963_ADDRESS, CNTL1, &buffer, 1);

	mpu925x->master_specific.delay_ms(mpu925x, 100);
}

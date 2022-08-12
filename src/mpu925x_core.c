/**
 * @file
 * @author Ceyhun Şen
 * @brief Core functions for MPU-925X driver.
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

/**
 * @brief Initialize MPU-925X sensor.
 * @param mpu925x MPU-925X struct pointer.
 * @param ad0 Last bit of the slave address (depends on ad0 pin connection).
 * @returns 0 on success, 1 on failure on mpu925x, 2 on failure on AK8963.
 * */
uint8_t mpu925x_init(mpu925x_t *mpu925x, uint8_t ad0)
{
	uint8_t return_value = 0;

	// Set address.
	mpu925x->settings.address = MPU925X_ADDRESS | (ad0 & 1);

	// Reset sensor.
	mpu925x_reset(mpu925x);

	// Configure MPU-925X.
	return_value = __mpu925x_init(mpu925x);
	if (return_value != 0)
		return 1;

	// Configure AK8963.
	return_value = __ak8963_init(mpu925x);
	if (return_value != 0)
		return 2;

	return 0;
}

/**
 * @brief Get all sensor data at once.
 * @param mpu925x MPU-925X struct pointer.
 * */
void mpu925x_get_all(mpu925x_t *mpu925x)
{
	mpu925x_get_acceleration(mpu925x);
	mpu925x_get_rotation(mpu925x);
	mpu925x_get_magnetic_field(mpu925x);
	mpu925x_get_temperature(mpu925x);
}

/**
 * @brief Get all raw sensor data at once.
 * @param mpu925x MPU-925X struct pointer.
 * */
void mpu925x_get_all_raw(mpu925x_t *mpu925x)
{
	mpu925x_get_acceleration_raw(mpu925x);
	mpu925x_get_rotation_raw(mpu925x);
	mpu925x_get_magnetic_field_raw(mpu925x);
	mpu925x_get_temperature_raw(mpu925x);
}

/**
 * @brief Get acceleration in G's.
 * @param mpu925x MPU-925X struct pointer.
 * @see mpu925x_get_acceleration_raw
 * */
void mpu925x_get_acceleration(mpu925x_t *mpu925x)
{
	mpu925x_get_acceleration_raw(mpu925x);

	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->sensor_data.acceleration[i] = mpu925x->sensor_data.acceleration_raw[i] / mpu925x->settings.acceleration_lsb;
	}
}

/**
 * @brief Get raw acceleration data.
 * @param mpu925x MPU-925X struct pointer.
 * @see mpu925x_get_acceleration
 * */
void mpu925x_get_acceleration_raw(mpu925x_t *mpu925x)
{
	uint8_t buffer[6];

	// Read raw acceleration data.
	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.address, ACCEL_XOUT_H, buffer, 6);
	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->sensor_data.acceleration_raw[i] = convert8bitto16bit(buffer[i * 2], buffer[i * 2 + 1]);
	}
}

/**
 * @brief Get rotation in degrees per second.
 * @param mpu925x MPU-925X struct pointer.
 * @see mpu925x_get_rotation_raw
 * */
void mpu925x_get_rotation(mpu925x_t *mpu925x)
{
	mpu925x_get_rotation_raw(mpu925x);

	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->sensor_data.rotation[i] = mpu925x->sensor_data.rotation_raw[i] / mpu925x->settings.gyroscope_lsb;
	}
}

/**
 * @brief Get raw rotation data.
 * @param mpu925x MPU-925X struct pointer.
 * @see mpu925x_get_rotation
 * */
void mpu925x_get_rotation_raw(mpu925x_t *mpu925x)
{
	uint8_t buffer[6];

	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.address, GYRO_XOUT_H, buffer, 6);
	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->sensor_data.rotation_raw[i] = convert8bitto16bit(buffer[i * 2], buffer[i * 2 + 1]);
	}
}

/**
 * @brief Get magnetic field in micro Gauss.
 * @param mpu925x MPU-925X struct pointer.
 * */
void mpu925x_get_magnetic_field(mpu925x_t *mpu925x)
{
	mpu925x_get_magnetic_field_raw(mpu925x);

	// Calculate magnetic_field data in micro Gauss.
	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->sensor_data.magnetic_field[i] = mpu925x->sensor_data.magnet_raw[i] * mpu925x->settings.magnetometer_lsb * mpu925x->settings.magnetometer_coefficient[i];
	}
}

/**
 * @brief Get raw magnetic field data.
 * @param mpu925x MPU-925X struct pointer.
 * */
void mpu925x_get_magnetic_field_raw(mpu925x_t *mpu925x)
{
	uint8_t buffer[7];

	// Check if data is ready in single measurent mode or self test mode.
	switch (mpu925x->settings.measurement_mode) {
		case mpu925x_single_measurement_mode:
		case mpu925x_self_test_mode:
			mpu925x->master_specific.bus_read(mpu925x, AK8963_ADDRESS, ST1, buffer, 1);
			if ((buffer[0] & 1) != 1) {
				return;
			}
			break;
		default:
			break;
	}

	// Read raw data and ST2 overflow register.
	mpu925x->master_specific.bus_read(mpu925x, AK8963_ADDRESS, HXL, buffer, 7);

	// Check overflow.
	if ((buffer[6] & 0x08) == 0x08) {
		return;
	}

	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->sensor_data.magnet_raw[i] = convert8bitto16bit(buffer[i * 2 + 1], buffer[i * 2]);
	}
}

/**
 * @brief Get temperature in celsius degree.
 * @param mpu925x MPU-925X struct pointer.
 * */
void mpu925x_get_temperature(mpu925x_t *mpu925x)
{
	mpu925x_get_temperature_raw(mpu925x);

	// Calculate temperature in celsius.
	mpu925x->sensor_data.temperature = ((mpu925x->sensor_data.temperature_raw - 0) / TEMPERATURE_SCALE) + 21;
}

/**
 * @brief Get raw temperature data.
 * @param mpu925x MPU-925X struct pointer.
 * */
void mpu925x_get_temperature_raw(mpu925x_t *mpu925x)
{
	uint8_t buffer[2];

	// Read raw temperature data.
	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.address, TEMP_OUT_H, buffer, 2);
	mpu925x->sensor_data.temperature_raw = convert8bitto16bit(buffer[0], buffer[1]);
}

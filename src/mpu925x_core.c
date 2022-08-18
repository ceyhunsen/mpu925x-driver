/**
 * @file mpu925x_core.c
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

static void mpu925x_reset(mpu925x_t *mpu925x);
static void ak8963_reset(mpu925x_t *mpu925x);

/**
 * @brief Initialize MPU-925X sensor.
 * @param mpu925x MPU-925X struct pointer.
 * @param ad0 Last bit of the slave address (depends on ad0 pin connection).
 * @returns 0 on success, 1 on failure on MPU-925X, 2 on failure on AK8963.
 * */
uint8_t mpu925x_init(mpu925x_t *mpu925x, uint8_t ad0)
{
	uint8_t buffer = 0;

	// Set address.
	mpu925x->settings.general.address = MPU925X_ADDRESS | (ad0 & 1);

	// WHO_AM_I register should return 0x71 for MPU-9250 and 0x73 for MPU-9255.
	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.general.address, WHO_AM_I, &buffer, 1);
	if (buffer != 0x71 && buffer != 0x73)
		return 1;

	// Reset sensor.
	mpu925x_reset(mpu925x);

	// Enable bypass.
	buffer = 1 << 1;
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.general.address, INT_PIN_CFG, &buffer, 1);

	// WIA register should return 0x48 for AK8963.
	mpu925x->master_specific.bus_read(mpu925x, AK8963_ADDRESS, WIA, &buffer, 1);
	if (buffer != 0x48)
		return 1;

	// Reset AK8963.
	ak8963_reset(mpu925x);

	// Save default accelerometer scale.
	mpu925x->settings.accelerometer.lsb = ACCELEROMETER_SCALE_2G;

	// Save default gyroscope scale.
	mpu925x->settings.gyroscope.lsb = GYROSCOPE_SCALE_250_DPS;

	// Save default magnetometer scale.
	mpu925x->settings.magnetometer.lsb = MAGNETOMETER_SCALE_14_BIT;

	// Save default temperature room offset.
	mpu925x->settings.thermometer.room_temperature_offset = 0;

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
		mpu925x->sensor_data.acceleration[i] = mpu925x->sensor_data.acceleration_raw[i] / mpu925x->settings.accelerometer.lsb;
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
	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.general.address, ACCEL_XOUT_H, buffer, 6);
	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->sensor_data.acceleration_raw[i] = convert_8_bit_to_16_bit(buffer[i * 2], buffer[i * 2 + 1]);
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
		mpu925x->sensor_data.rotation[i] = mpu925x->sensor_data.rotation_raw[i] / mpu925x->settings.gyroscope.lsb;
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

	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.general.address, GYRO_XOUT_H, buffer, 6);
	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->sensor_data.rotation_raw[i] = convert_8_bit_to_16_bit(buffer[i * 2], buffer[i * 2 + 1]);
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
		mpu925x->sensor_data.magnetic_field[i] = mpu925x->sensor_data.magnet_raw[i] * mpu925x->settings.magnetometer.lsb * mpu925x->settings.magnetometer.coefficient[i];
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
	switch (mpu925x->settings.magnetometer.measurement_mode) {
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
		mpu925x->sensor_data.magnet_raw[i] = convert_8_bit_to_16_bit(buffer[i * 2 + 1], buffer[i * 2]);
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
	mpu925x->sensor_data.temperature =
		((mpu925x->sensor_data.temperature_raw -
		mpu925x->settings.thermometer.room_temperature_offset)
		/ TEMPERATURE_SCALE) + 21;
}

/**
 * @brief Get raw temperature data.
 * @param mpu925x MPU-925X struct pointer.
 * */
void mpu925x_get_temperature_raw(mpu925x_t *mpu925x)
{
	uint8_t buffer[2];

	// Read raw temperature data.
	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.general.address, TEMP_OUT_H, buffer, 2);
	mpu925x->sensor_data.temperature_raw = convert_8_bit_to_16_bit(buffer[0], buffer[1]);
}

/**
 * @brief Reset MPU-925X sensor.
 * @param mpu925x MPU-925X struct pointer.
 * */
static void mpu925x_reset(mpu925x_t *mpu925x)
{
	uint8_t buffer = 1 << 7;
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.general.address, PWR_MGMT_1, &buffer, sizeof buffer);
	mpu925x->master_specific.delay_ms(mpu925x, 100);
}

/**
 * @brief Reset AK8963 sensor.
 * @param mpu925x MPU-925X struct pointer.
 * */
static void ak8963_reset(mpu925x_t *mpu925x)
{
	uint8_t buffer = 1;
	mpu925x->master_specific.bus_write(mpu925x, AK8963_ADDRESS, CNTL2, &buffer, sizeof buffer);
	mpu925x->master_specific.delay_ms(mpu925x, 100);
}

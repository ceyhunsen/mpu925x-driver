/**
 * @file
 * @brief Core functions for MPU-925x driver.
 * @author Ceyhun Şen
 * */

/*
 * MIT License
 * 
 * Copyright (c) 2022 Ceyhun Şen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * */

#include "mpu925x_internals.h"

/**
 * @brief Initialize MPU-925X sensor.
 * @returns 0 on success, 1 on failure on mpu925x, 2 on failure on AK8963.
 * */
uint8_t mpu925x_init(mpu925x_t *mpu925x, uint8_t ad0)
{
	// Set address.
	mpu925x->settings.address = MPU925X_ADDRESS | (ad0 & 1);

	// Reset sensor.
	mpu925x_reset(mpu925x);

	// Configure MPU-925X.
	uint8_t mpu925x_ret = __mpu925x_init(mpu925x);
	// Configure AK8963.
	uint8_t ak8963_ret = __ak8963_init(mpu925x);

	return (mpu925x_ret != 0)? 1: (ak8963_ret != 0)? 2: 0;
}

/**
 * @brief Get all sensor data at once.
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
 * @brief Get rotation in dps's.
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
 * @brief Get magnetic field data.
 * */
void mpu925x_get_magnetic_field(mpu925x_t *mpu925x)
{
	mpu925x_get_magnetic_field_raw(mpu925x);

	// Calculate magnetic_field data in mikro Gauss.
	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->sensor_data.magnetic_field[i] = mpu925x->sensor_data.magnet_raw[i] * mpu925x->settings.magnetometer_lsb * mpu925x->settings.magnetometer_coefficient[i];
	}
}

/**
 * @brief Get raw magnetic field data.
 * */
void mpu925x_get_magnetic_field_raw(mpu925x_t *mpu925x)
{
	uint8_t buffer[7];

	// Check if data is ready in single measurent mode or self test mode.
	if (mpu925x->settings.measurement_mode == single_measurement_mode || mpu925x->settings.measurement_mode == self_test_mode) {
		ak8963_bus_read(mpu925x, ST1, buffer, 1);
		if ((buffer[0] & 1) != 1) {
			return;
		}
	}

	// Read raw data and ST2 overflow register.
	ak8963_bus_read(mpu925x, HXL, buffer, 7);

	// Check overflow.
	if ((buffer[6] & 0x08) == 0x08) {
		return;
	}

	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->sensor_data.magnet_raw[i] = convert8bitto16bit(buffer[i * 2 + 1], buffer[i * 2]);
	}
}

/**
 * @brief Get temperature data.
 * */
void mpu925x_get_temperature(mpu925x_t *mpu925x)
{
	mpu925x_get_temperature_raw(mpu925x);

	// Calculate temperature in celsius.
	mpu925x->sensor_data.temperature = ((mpu925x->sensor_data.temperature_raw - 0) / mpu925x->settings.temperature_lsb) + 21;
}

/**
 * @brief Get temperature data.
 * */
void mpu925x_get_temperature_raw(mpu925x_t *mpu925x)
{
	uint8_t buffer[2];

	// Read raw temperature data.
	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.address, TEMP_OUT_H, buffer, 2);
	mpu925x->sensor_data.temperature_raw = convert8bitto16bit(buffer[0], buffer[1]);
}

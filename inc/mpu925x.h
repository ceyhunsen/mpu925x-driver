/**
 * @file
 * @author Ceyhun Şen
 * @brief Header file for MPU-925X driver.
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

#ifndef __MPU925X_H
#define __MPU925X_H

// C++ compatibility.
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>

/**
 * @enum mpu925x_clock
 * Clock settings for MPU-925X.
 * */
typedef enum mpu925x_clock {
	mpu925x_internal_20_hz_clock,
	mpu925x_auto_select_pll
} mpu925x_clock;

/**
 * @enum mpu925x_orientation
 * @brief Orientation of the sensor.
 * */
typedef enum mpu925x_orientation {
	mpu925x_x_plus,
	mpu925x_x_minus,
	mpu925x_y_plus,
	mpu925x_y_minus,
	mpu925x_z_plus,
	mpu925x_z_minus
} mpu925x_orientation;

/**
 * @enum mpu925x_accelerometer_scale
 * @brief Accelerometer full-scale ranges.
 * */
typedef enum mpu925x_accelerometer_scale {
	mpu925x_2g,
	mpu925x_4g,
	mpu925x_8g,
	mpu925x_16g
} mpu925x_accelerometer_scale;

/**
 * @enum mpu925x_gyroscope_scale
 * @brief Gyroscope full-scale ranges for gyroscope.
 * */
typedef enum mpu925x_gyroscope_scale {
	mpu925x_250dps,
	mpu925x_500dps,
	mpu925x_1000dps,
	mpu925x_2000dps
} mpu925x_gyroscope_scale;

/**
 * @enum mpu925x_magnetometer_measurement_mode
 * Measurement modes for AK8963.
 * */
typedef enum mpu925x_magnetometer_measurement_mode {
	mpu925x_power_down_mode,
	mpu925x_single_measurement_mode,
	mpu925x_continuous_measurement_mode_1,
	mpu925x_continuous_measurement_mode_2,
	mpu925x_external_trigger_measurement_mode,
	mpu925x_self_test_mode,
	mpu925x_fuse_rom_access_mode
} mpu925x_magnetometer_measurement_mode;

/**
 * @enum mpu925x_magnetometer_bit_mode
 * Bit modes for AK8963.
 * */
typedef enum mpu925x_magnetometer_bit_mode {
	mpu925x_14_bit,
	mpu925x_16_bit
} mpu925x_magnetometer_bit_mode;

/**
 * @struct mpu925x_t mpu925x.h mpu925x.h
 * @brief Main struct for MPU-925X driver.
 * 
 * This structs includes sensor data, driver settings and master spesific
 * handle, bus and delay function pointers.
 * */
typedef struct mpu925x_t {
	/**
	 * @struct sensor_data
	 * @brief Holds sensor data.
	 * */
	struct sensor_data {
		int16_t acceleration_raw[3], rotation_raw[3], magnet_raw[3], temperature_raw;
		float acceleration[3], rotation[3], magnetic_field[3], temperature;
	} sensor_data;

	/**
	 * @struct settings
	 * @brief Holds sensor settings.
	 * */
	struct settings {
		mpu925x_orientation orientation;
		mpu925x_accelerometer_scale accelerometer_scale;
		mpu925x_gyroscope_scale gyroscope_scale;
		mpu925x_magnetometer_measurement_mode measurement_mode;
		mpu925x_magnetometer_bit_mode bit_mode;
		float acceleration_lsb, gyroscope_lsb, magnetometer_lsb, temperature_lsb;
		float magnetometer_coefficient[3];
		uint8_t address;
	} settings;

	/**
	 * @struct master_specific
	 * @brief Holds master spesific pointers.
	 * */
	struct master_specific {
		uint8_t (*bus_read)(struct mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size);
		uint8_t (*bus_write)(struct mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size);
		void (*delay_ms)(struct mpu925x_t *mpu925x, uint32_t delay);
		void *bus_handle;
	} master_specific;
} mpu925x_t;

// Core
uint8_t mpu925x_init(mpu925x_t *mpu925x, uint8_t ad0);

// Sensor data
void mpu925x_get_all_raw(mpu925x_t *mpu925x);
void mpu925x_get_all(mpu925x_t *mpu925x);
void mpu925x_get_acceleration_raw(mpu925x_t *mpu925x);
void mpu925x_get_acceleration(mpu925x_t *mpu925x);
void mpu925x_get_rotation_raw(mpu925x_t *mpu925x);
void mpu925x_get_rotation(mpu925x_t *mpu925x);
void mpu925x_get_magnetic_field_raw(mpu925x_t *mpu925x);
void mpu925x_get_magnetic_field(mpu925x_t *mpu925x);
void mpu925x_get_temperature_raw(mpu925x_t *mpu925x);
void mpu925x_get_temperature(mpu925x_t *mpu925x);

// General settings
void mpu925x_set_sample_rate_divider(mpu925x_t *mpu925x, uint8_t sample_rate_divider);
void mpu925x_set_clock_source(mpu925x_t *mpu925x, mpu925x_clock clock);

// Accelerometer settings
void mpu925x_set_accelerometer_scale(mpu925x_t *mpu925x, mpu925x_accelerometer_scale scale);
void mpu925x_set_accelerometer_dlpf(mpu925x_t *mpu925x, uint8_t a_fchoice, uint8_t dlpf);
void mpu925x_accelerometer_offset_cancellation(mpu925x_t *mpu925x, uint16_t sampling_amount);
void mpu925x_get_accelerometer_offset(mpu925x_t *mpu925x, uint16_t sampling_amount, int16_t *offset);
void mpu925x_set_accelerometer_offset(mpu925x_t *mpu925x, int16_t *offset);

// Gyroscope settings
void mpu925x_set_gyroscope_scale(mpu925x_t *mpu925x, mpu925x_gyroscope_scale scale);
void mpu925x_set_gyroscope_dlpf(mpu925x_t *mpu925x, uint8_t a_fchoice, uint8_t dlpf);
void mpu925x_gyroscope_offset_cancellation(mpu925x_t *mpu925x, uint16_t sampling_amount);
void mpu925x_get_gyroscope_offset(mpu925x_t *mpu925x, uint16_t sampling_amount, int16_t *offset);
void mpu925x_set_gyroscope_offset(mpu925x_t *mpu925x, int16_t *offset);

// Magnetometer settings
void mpu925x_set_magnetometer_measurement_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_measurement_mode measurement_mode);
void mpu925x_set_magnetometer_bit_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_bit_mode bit_mode);

// C++ compatibility.
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __MPU925X_H

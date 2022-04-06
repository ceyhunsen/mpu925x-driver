/**
 * @file
 * @brief Header file for MPU-925X driver.
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

#ifndef __MPU925X_H
#define __MPU925X_H

// C++ compatibility.
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>

// FIFO sentence defines.
#define MPU925X_FIFO_ENABLE_ALL      (0xFF)
#define MPU925X_FIFO_DISABLE_ALL     (0x00)
#define MPU925X_FIFO_ENABLE_TEMP     (1 << 7)
#define MPU925X_FIFO_DISABLE_TEMP    (0 << 7)
#define MPU925X_FIFO_ENABLE_GYRO_X   (1 << 6)
#define MPU925X_FIFO_DISABLE_GYRO_X  (0 << 6)
#define MPU925X_FIFO_ENABLE_GYRO_Y   (1 << 5)
#define MPU925X_FIFO_DISABLE_GYRO_Y  (0 << 5)
#define MPU925X_FIFO_ENABLE_GYRO_Z   (1 << 4)
#define MPU925X_FIFO_DISABLE_GYRO_Z  (0 << 4)
#define MPU925X_FIFO_ENABLE_ACCEL    (1 << 3)
#define MPU925X_FIFO_DISABLE_ACCEL   (0 << 3)
#define MPU925X_FIFO_ENABLE_SLV_2    (1 << 2)
#define MPU925X_FIFO_DISABLE_SLV_2   (0 << 2)
#define MPU925X_FIFO_ENABLE_SLV_1    (1 << 1)
#define MPU925X_FIFO_DISABLE_SLV_1   (0 << 1)
#define MPU925X_FIFO_ENABLE_SLV_0    (1 << 0)
#define MPU925X_FIFO_DISABLE_SLV_0   (0 << 0)

/**
 * @enum mpu925x_clock
 * Clock settings for MPU-925X.
 * */
typedef enum mpu925x_clock {
	internal_20_hz_clock,
	auto_select_pll
} mpu925x_clock;

/**
 * @enum mpu925x_orientation
 * Orientation of the sensor.
 * */
typedef enum mpu925x_orientation {
	x_plus,
	x_minus,
	y_plus,
	y_minus,
	z_plus,
	z_minus
} mpu925x_orientation;

/**
 * @enum mpu925x_fifo_mode
 * FIFO operating modes.
 * */
typedef enum mpu925x_fifo_mode {
	disabled,
	overflow_enabled,
	overflow_disabled
} mpu925x_fifo_mode;

/**
 * @enum mpu925x_accelerometer_scale
 * Acceleration ranges for accelerometer.
 * */
typedef enum mpu925x_accelerometer_scale {
	_2g,
	_4g,
	_8g,
	_16g
} mpu925x_accelerometer_scale;

/**
 * @enum mpu925x_gyroscope_scale
 * Gyro ranges for gyrometer.
 * */
typedef enum mpu925x_gyroscope_scale {
	_250dps,
	_500dps,
	_1000dps,
	_2000dps
} mpu925x_gyroscope_scale;

/**
 * @enum mpu925x_magnetometer_measurement_mode
 * Measurement modes for AK8963.
 * */
typedef enum mpu925x_magnetometer_measurement_mode {
	power_down_mode,
	single_measurement_mode,
	continuous_measurement_mode_1,
	continuous_measurement_mode_2,
	external_trigger_measurement_mode,
	self_test_mode,
	fuse_rom_access_mode
} mpu925x_magnetometer_measurement_mode;

/**
 * @enum mpu925x_magnetometer_bit_mode
 * Bit modes for AK8963.
 * */
typedef enum mpu925x_magnetometer_bit_mode {
	_14_bit,
	_16_bit
} mpu925x_magnetometer_bit_mode;

/**
 * @struct mpu925x_t mpu925x.h mpu925x.h
 * @brief Main struct for MPU-925X driver.
 * This structs includes sensor data, driver settings and master spesific
 * handle data, bus and delay function pointers.
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
void mpu925x_set_clock_source(mpu925x_t *mpu925x, uint8_t clock);

// FIFO operations
void mpu925x_set_fifo_mode(mpu925x_t *mpu925x, mpu925x_fifo_mode mode);
void mpu925x_set_fifo(mpu925x_t *mpu925x, uint8_t fifo_sentence);
uint16_t mpu925x_get_fifo_count(mpu925x_t *mpu925x);
int16_t mpu925x_get_fifo_data(mpu925x_t *mpu925x);
void mpu925x_clear_fifo(mpu925x_t *mpu925x);

// Accelerometer settings
void mpu925x_set_accelerometer_scale(mpu925x_t *mpu925x, mpu925x_accelerometer_scale scale);
void mpu925x_set_accelerometer_dlpf(mpu925x_t *mpu925x, uint8_t a_fchoice, uint8_t dlpf);
void mpu925x_accelerometer_offset_cancellation(mpu925x_t *mpu925x, uint16_t sample_count);
void mpu925x_get_accelerometer_offset(mpu925x_t *mpu925x, uint16_t sample_count, int16_t *offset);
void mpu925x_set_accelerometer_offset(mpu925x_t *mpu925x, int16_t *offset);

// Gyroscope settings
void mpu925x_set_gyroscope_scale(mpu925x_t *mpu925x, mpu925x_gyroscope_scale scale);
void mpu925x_set_gyroscope_dlpf(mpu925x_t *mpu925x, uint8_t a_fchoice, uint8_t dlpf);
void mpu925x_gyroscope_offset_cancellation(mpu925x_t *mpu925x, uint16_t sample_count);
void mpu925x_get_gyroscope_offset(mpu925x_t *mpu925x, uint16_t sample_count, int16_t *offset);
void mpu925x_set_gyroscope_offset(mpu925x_t *mpu925x, int16_t *offset);

// Magnetometer settings
void mpu925x_set_magnet_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_measurement_mode measurement_mode, mpu925x_magnetometer_bit_mode bit_mode);
void mpu925x_set_magnetometer_measurement_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_measurement_mode measurement_mode);
void mpu925x_set_magnetometer_bit_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_bit_mode bit_mode);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __MPU925X_H

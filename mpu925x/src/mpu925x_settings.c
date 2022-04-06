/**
 * @file
 * @brief Settings for MPU-925x driver.
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
#include <stdint.h>

/*******************************************************************************
 * General settings
 ******************************************************************************/

/**
 * @brief Set sample rate divider.
 * @param mpu925x Main struct.
 * @param sample_rate_divider Sample rate divider.
 * */
void mpu925x_set_sample_rate_divider(mpu925x_t *mpu925x, uint8_t sample_rate_divider)
{
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, SMPLRT_DIV, &sample_rate_divider, 1);
}

/**
 * @brief Set clock source.
 * @param mpu925x Main struct.
 * @param clock Clock select option. 0 for internal 20 Hz clock, 1 for PLL.
 * */
void mpu925x_set_clock_source(mpu925x_t *mpu925x, mpu925x_clock clock)
{
	uint8_t buffer = 0;

	switch (clock) {
		case internal_20_hz_clock:
			buffer = 0;
			break;
		case auto_select_pll:
			buffer = 1;
			break;
		default:
			buffer = 1;
			break;
	}

	mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, PWR_MGMT_1, &buffer, 1, 0b11111000);
}

/*******************************************************************************
 * FIFO operations
 ******************************************************************************/

/**
 * @brief Set FIFO mode.
 * @param mpu925x Main struct.
 * @param mode FIFO mode.
 * */
void mpu925x_set_fifo_mode(mpu925x_t *mpu925x, mpu925x_fifo_mode mode)
{
	uint8_t buffer;

	switch (mode) {
		case disabled:
			buffer = 0 << 6;
			mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, USER_CTRL, &buffer, 1, ~(1 << 6));
			break;
		case overflow_disabled:
			buffer = 1 << 6;
			mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, USER_CTRL, &buffer, 1, ~(1 << 6));
			buffer = 1 << 6;
			mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, CONFIG, &buffer, 1, ~(1 << 6));
			break;
		case overflow_enabled:
			buffer = 1 << 6;
			mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, USER_CTRL, &buffer, 1, ~(1 << 6));
			buffer = 0 << 6;
			mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, CONFIG, &buffer, 1, ~(1 << 6));
			break;
		default:
			buffer = 0 << 6;
			mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, USER_CTRL, &buffer, 1, ~(1 << 6));
			break;
	}
}

/**
 * @brief Enable or disable FIFO for specific axis and... things.
 * See FIFO enable defines in mpu925x.h for more clearance.
 * @param mpu925x Main struct.
 * @param fifo_sentence This sentence will be directly written to FIFO enable register.
 * */
void mpu925x_set_fifo(mpu925x_t *mpu925x, uint8_t fifo_sentence)
{
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, FIFO_EN, &fifo_sentence, 1);
}

/**
 * @brief Get FIFO count.
 * @param mpu925x Main struct.
 * @returns FIFO count.
 * */
uint16_t mpu925x_get_fifo_count(mpu925x_t *mpu925x)
{
	uint8_t buffer[2];
	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.address, FIFO_COUNTH, buffer, 2);

	return convert8bitto16bit(buffer[0], buffer[1]);
}

/**
 * @brief Get FIFO data.
 * @param mpu925x Main struct.
 * @returns 2 byte FIFO data.
 * */
int16_t mpu925x_get_fifo_data(mpu925x_t *mpu925x)
{
	uint8_t buffer[2];

	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.address, FIFO_R_W, buffer, 2);

	return convert8bitto16bit(buffer[0], buffer[1]);
}

/**
 * @brief Clear FIFO buffer.
 * @param mpu925x Main struct.
 * */
void mpu925x_clear_fifo(mpu925x_t *mpu925x)
{
	uint8_t buffer = 1 << 2;
	mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, USER_CTRL, &buffer, 1, ~(1 << 2));
}

/*******************************************************************************
 * Accelerometer settings
 ******************************************************************************/

/**
 * @brief Set acceleration scale range.
 * @param mpu925x mpu925x_t data struct.
 * @param scale Accelerometer scale.
 * */
void mpu925x_set_accelerometer_scale(mpu925x_t *mpu925x, mpu925x_accelerometer_scale scale)
{
	uint8_t ACCEL_FS_SEL = 0;

	switch (scale) {
		case _2g:
			ACCEL_FS_SEL = 0 << 3;
			mpu925x->settings.acceleration_lsb = ACCELEROMETER_SCALE_2G;
			break;
		case _4g:
			ACCEL_FS_SEL = 1 << 3;
			mpu925x->settings.acceleration_lsb = ACCELEROMETER_SCALE_4G;
			break;
		case _8g:
			ACCEL_FS_SEL = 2 << 3;
			mpu925x->settings.acceleration_lsb = ACCELEROMETER_SCALE_8G;
			break;
		case _16g:
			ACCEL_FS_SEL = 3 << 3;
			mpu925x->settings.acceleration_lsb = ACCELEROMETER_SCALE_16G;
			break;
		default:
			ACCEL_FS_SEL = 0 << 3;
			mpu925x->settings.acceleration_lsb = ACCELEROMETER_SCALE_2G;
			break;
	}

	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, ACCEL_CONFIG, &ACCEL_FS_SEL, 1);
}

/**
 * @brief Set accelerometer digital low pass filter.
 * @param mpu925x Main struct.
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
 * @brief Calibrate accelerometer.
 * @param mpu925x Main struct.
 * @param sample_count Sample count.
 * @see mpu925x_get_accelerometer_offset
 * @see mpu925x_set_accelerometer_offset
 * */
void mpu925x_accelerometer_offset_cancellation(mpu925x_t *mpu925x, uint16_t sample_count)
{
	int16_t offset[3];

	mpu925x_get_accelerometer_offset(mpu925x, sample_count, offset);
	mpu925x_set_accelerometer_offset(mpu925x, offset);
}

/**
 * @brief Get accelerometer offset cancellation value.
 * */
void mpu925x_get_accelerometer_offset(mpu925x_t *mpu925x, uint16_t sample_count, int16_t *offset)
{
	float average[3] = {0.0, 0.0, 0.0};
	for (uint16_t i = 0; i < sample_count; i++) {
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
		case x_plus:
			average[0] += mpu925x->settings.acceleration_lsb;
			break;
		case x_minus:
			average[0] -= mpu925x->settings.acceleration_lsb;
			break;
		case y_plus:
			average[1] += mpu925x->settings.acceleration_lsb;
			break;
		case y_minus:
			average[1] -= mpu925x->settings.acceleration_lsb;
			break;
		case z_plus:
			average[2] += mpu925x->settings.acceleration_lsb;
			break;
		case z_minus:
			average[2] -= mpu925x->settings.acceleration_lsb;
			break;
		default:
			average[2] += mpu925x->settings.acceleration_lsb;
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
 * @brief Set gyro scale.
 * @param mpu925x mpu925x_t data struct.
 * @param scale Gyro scale.
 * */
void mpu925x_set_gyroscope_scale(mpu925x_t *mpu925x, mpu925x_gyroscope_scale scale)
{
	uint8_t GYRO_FS_SEL = 0;

	switch (scale) {
		case _250dps:
			GYRO_FS_SEL = 0 << 3;
			mpu925x->settings.gyroscope_lsb = GYROSCOPE_SCALE_250_DPS;
			break;
		case _500dps:
			GYRO_FS_SEL = 1 << 3;
			mpu925x->settings.gyroscope_lsb = GYROSCOPE_SCALE_500_DPS;
			break;
		case _1000dps:
			GYRO_FS_SEL = 2 << 3;
			mpu925x->settings.gyroscope_lsb = GYROSCOPE_SCALE_1000_DPS;
			break;
		case _2000dps:
			GYRO_FS_SEL = 3 << 3;
			mpu925x->settings.gyroscope_lsb = GYROSCOPE_SCALE_2000_DPS;
			break;
		default:
			GYRO_FS_SEL = 0 << 3;
			mpu925x->settings.gyroscope_lsb = GYROSCOPE_SCALE_250_DPS;
			break;
	}

	mpu925x_bus_write_preserve(mpu925x, mpu925x->settings.address, GYRO_CONFIG, &GYRO_FS_SEL, 1, 0b11100111);
}

/**
 * @brief Set gyroscope digital low pass filter setting.
 * @param mpu925x Main struct.
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
 * @brief Get and set gyroscope offset.
 * @param mpu925x Main struct.
 * @param sample_count Amount of data that will be read.
 * */
void mpu925x_gyroscope_offset_cancellation(mpu925x_t *mpu925x, uint16_t sample_count)
{
	int16_t offset[3];

	mpu925x_get_gyroscope_offset(mpu925x, sample_count, offset);
	mpu925x_set_gyroscope_offset(mpu925x, offset);
}

/**
 * @brief Gets the offset values of x, y and z axis of gyroscope.
 * @param mpu925x Main struct.
 * @param sample_count Amount of data that will be read.
 * @param offset Array of size 3 that will hold offset values.
 * @see mpu925x_set_gyroscope_offset
 * */
void mpu925x_get_gyroscope_offset(mpu925x_t *mpu925x, uint16_t sample_count, int16_t *offset)
{
	// Offsets of x, y and z axis is seperate.
	for (uint8_t i = 0; i < 3; i++) {
		// Read rotation data and get average.
		int16_t fifo_data;
		float average = 0.0;
		for (uint16_t j = 0; j < sample_count; j++) {
			mpu925x_get_rotation_raw(mpu925x);
			fifo_data = mpu925x->sensor_data.rotation_raw[i];

			// Avoid overflow.
			float a = 1.0 / (j + 1.0);
			float b = 1.0 - a;
			average = a * fifo_data + b * average;
		}

		uint8_t fs_sel = 0;
		switch(mpu925x->settings.gyroscope_scale) {
			case _250dps:
				fs_sel = 0;
				break;
			case _500dps:
				fs_sel = 1;
				break;
			case _1000dps:
				fs_sel = 2;
				break;
			case _2000dps:
				fs_sel = 3;
				break;
			default:
				fs_sel = 0;
				break;
		}

		offset[i] = (int16_t)(-average / 4.0 * powerof2(fs_sel));
	}
}

/**
 * @brief Sets the offset value of x, y and z axis of gyroscope.
 * */
void mpu925x_set_gyroscope_offset(mpu925x_t *mpu925x, int16_t *offset)
{
	uint8_t buffer[2];

	// Set gyro offset.
	for (uint8_t i = 0; i < 3; i++) {
		buffer[0] = (uint8_t)(offset[i] >> 8);
		buffer[1] = (uint8_t)(offset[i] & 0xFF);
		mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, XG_OFFSET_H + i * 2, buffer, 2);
	}
}

/*******************************************************************************
 * Magnetometer settings
 ******************************************************************************/

/**
 * @brief Set magnetometer measurement mode.
 * @param mpu925x Main struct.
 * @param measurement_mode Measurement mode for magnetometer.
 * */
void mpu925x_set_magnetometer_measurement_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_measurement_mode measurement_mode)
{
	uint8_t buffer;

	// Save measurement mode.
	mpu925x->settings.measurement_mode = measurement_mode;

	ak8963_bus_read(mpu925x, CNTL1, &buffer, 1);
	buffer &= 0b11110000;

	switch (measurement_mode) {
		case power_down_mode:
			buffer |= 0b0000;
			break;
		case single_measurement_mode:
			buffer |= 0b0001;
			break;
		case continuous_measurement_mode_1:
			buffer |= 0b0010;
			break;
		case continuous_measurement_mode_2:
			buffer |= 0b0110;
			break;
		case external_trigger_measurement_mode:
			buffer |= 0b0100;
			break;
		case self_test_mode:
			buffer |= 0b1000;
			break;
		case fuse_rom_access_mode:
			buffer |= 0b1111;
			break;
		default:
			buffer |= 0b0000;
			break;
	}

	ak8963_bus_write(mpu925x, CNTL1, &buffer, 1);

	mpu925x->master_specific.delay_ms(mpu925x, 100);
}

/**
 * @brief Set magnetometers bit mode.
 * @param mpu925x Main struct.
 * @param bit_mode Bit mode for magnetometer.
 * */
void mpu925x_set_magnetometer_bit_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_bit_mode bit_mode)
{
	uint8_t buffer;

	// Save bit mode.
	mpu925x->settings.bit_mode = bit_mode;

	ak8963_bus_read(mpu925x, CNTL1, &buffer, 1);
	buffer &= 0b11101111;

	switch (bit_mode) {
		case _14_bit:
			buffer |= 0 << 4;
			mpu925x->settings.magnetometer_lsb = MAGNETOMETER_SCALE_14_BIT;
			break;
		case _16_bit:
			buffer |= 1 << 4;
			mpu925x->settings.magnetometer_lsb = MAGNETOMETER_SCALE_16_BIT;
			break;
		default:
			mpu925x->settings.magnetometer_lsb = MAGNETOMETER_SCALE_16_BIT;
			buffer |= 1 << 4;
			break;
	}

	ak8963_bus_write(mpu925x, CNTL1, &buffer, 1);

	mpu925x->master_specific.delay_ms(mpu925x, 100);
}

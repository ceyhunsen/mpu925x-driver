/**
 * @file
 * @author Ceyhun Şen
 * @brief Internal functions for MPU-925X driver.
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

/**
 * @brief Initialize accelerometer and gyro.
 * @param mpu925x MPU-925X struct pointer.
 * @returns 0 on success, 1 on failure.
 * */
uint8_t __mpu925x_init(mpu925x_t *mpu925x)
{
	uint8_t buffer;

	// WHO_AM_I register should return 0x71 for MPU-9250 and 0x73 for MPU-9255.
	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.address, WHO_AM_I, &buffer, 1);
	if (buffer != 0x71 && buffer != 0x73)
		return 1;

	// Enable PLL.
	mpu925x_set_clock_source(mpu925x, mpu925x_auto_select_pll);

	// Enable bypass.
	buffer = 1 << 1;
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, INT_PIN_CFG, &buffer, 1);

	// Disable I2C master mode.
	buffer = 0 << 5;
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, USER_CTRL, &buffer, 1);

	// Set acceleration range.
	mpu925x_set_accelerometer_scale(mpu925x, mpu925x->settings.accelerometer_scale);

	// Set gyro range.
	mpu925x_set_gyroscope_scale(mpu925x, mpu925x->settings.gyroscope_scale);

	// Set temperature lsb.
	mpu925x->settings.temperature_lsb = TEMPERATURE_SCALE;

	return 0;
}

/**
 * @brief Initialize magnetometer.
 * @param mpu925x MPU-925X struct pointer.
 * @returns 0 on success, 1 on failure.
 * */
uint8_t __ak8963_init(mpu925x_t *mpu925x)
{
	uint8_t buffer;

	// Check WIA register. WIA register should return 0x48.
	mpu925x->master_specific.bus_read(mpu925x, AK8963_ADDRESS, WIA, &buffer, 1);
	if (buffer != 0x48)
		return 1;

	// Reset AK8963.
	ak8963_reset(mpu925x);

	// Set power down mode.
	mpu925x_set_magnetometer_measurement_mode(mpu925x, mpu925x_power_down_mode);

	// Enable Fuse ROM access mode.
	mpu925x_set_magnetometer_measurement_mode(mpu925x, mpu925x_fuse_rom_access_mode);

	// Read coefficient data and save it.
	uint8_t coef_data[3];
	mpu925x->master_specific.bus_read(mpu925x, AK8963_ADDRESS, ASAX, coef_data, 3);
	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->settings.magnetometer_coefficient[i] = (coef_data[i] - 128) * 0.5 / 128 + 1;
	}

	// Set power down mode.
	mpu925x_set_magnetometer_measurement_mode(mpu925x, mpu925x_power_down_mode);

	// Set measurement and bit mode.
	mpu925x_set_magnetometer_measurement_mode(mpu925x, mpu925x_continuous_measurement_mode_2);
	mpu925x_set_magnetometer_bit_mode(mpu925x, mpu925x_16_bit);

	return 0;
}

/**
 * @brief Get acceleration bias.
 * @param mpu925x MPU-925X struct pointer.
 * @param bias 3d array which will hold bias values.
 * */
void mpu925x_get_accelerometer_bias(mpu925x_t *mpu925x, int16_t *bias)
{
	uint8_t buffer[8];

	// Read bias registers.
	mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.address, XA_OFFSET_H, buffer, 8);

	// Convert them to 16 bit.
	for (uint8_t i = 0; i < 3; i++) {
		bias[i] = convert8bitto16bit(buffer[i * 3], buffer[i * 3 + 1]);
	}
}

/**
 * @brief Write data to bus whilst preserving other bits (not ready for usage).
 * @param mpu925x MPU-925X struct pointer.
 * */
void mpu925x_bus_write_preserve(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size, uint8_t and_sentence)
{
	uint8_t read_buffer;

	for (uint16_t i = 0; i < size; i++) {
		mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.address, reg, &read_buffer, size);
		read_buffer &= and_sentence;
		read_buffer |= buffer[i];
		mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, reg, &read_buffer, size);
	}
}

/**
 * @brief Reset MPU-925X sensor.
 * @param mpu925x MPU-925X struct pointer.
 * */
void mpu925x_reset(mpu925x_t *mpu925x)
{
	uint8_t buffer = 1 << 7;
	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, PWR_MGMT_1, &buffer, 1);
	mpu925x->master_specific.delay_ms(mpu925x, 100);
}

/**
 * @brief Reset AK8963 sensor.
 * @param mpu925x MPU-925X struct pointer.
 * */
void ak8963_reset(mpu925x_t *mpu925x)
{
	uint8_t buffer = 1;
	mpu925x->master_specific.bus_write(mpu925x, AK8963_ADDRESS, CNTL2, &buffer, 1);
	mpu925x->master_specific.delay_ms(mpu925x, 100);
}

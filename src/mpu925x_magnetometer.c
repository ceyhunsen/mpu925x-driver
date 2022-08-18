/**
 * @file mpu925x_magnetometer.c
 * @author Ceyhun Şen
 * @brief Settings for magnetometer.
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

void mpu925x_magnetometer_calibrate(mpu925x_t *mpu925x)
{
	// Set power down mode.
	mpu925x_magnetometer_set_measurement_mode(mpu925x, mpu925x_power_down_mode);

	// Enable Fuse ROM access mode.
	mpu925x_magnetometer_set_measurement_mode(mpu925x, mpu925x_fuse_rom_access_mode);

	// Read coefficient data and save it.
	uint8_t coef_data[3];
	mpu925x->master_specific.bus_read(mpu925x, AK8963_ADDRESS, ASAX, coef_data, 3);
	for (uint8_t i = 0; i < 3; i++) {
		mpu925x->settings.magnetometer.coefficient[i] = (coef_data[i] - 128) * 0.5 / 128 + 1;
	}

	// Set power down mode.
	mpu925x_magnetometer_set_measurement_mode(mpu925x, mpu925x_power_down_mode);

	// Set measurement and bit mode.
	mpu925x_magnetometer_set_measurement_mode(mpu925x, mpu925x_continuous_measurement_mode_2);
	mpu925x_magnetometer_set_bit_mode(mpu925x, mpu925x_16_bit);
}

/**
 * @brief Set magnetometer measurement mode.
 * @param mpu925x MPU-925X struct pointer.
 * @param measurement_mode Measurement mode for magnetometer to be set.
 * @see mpu925x_magnetometer_measurement_mode
 * */
void mpu925x_magnetometer_set_measurement_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_measurement_mode measurement_mode)
{
	uint8_t buffer;

	// Save measurement mode.
	mpu925x->settings.magnetometer.measurement_mode = measurement_mode;

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
void mpu925x_magnetometer_set_bit_mode(mpu925x_t *mpu925x, mpu925x_magnetometer_bit_mode bit_mode)
{
	uint8_t buffer;

	// Save bit mode.
	mpu925x->settings.magnetometer.bit_mode = bit_mode;

	mpu925x->master_specific.bus_read(mpu925x, AK8963_ADDRESS, CNTL1, &buffer, 1);
	buffer &= 0b11101111;

	switch (bit_mode) {
		case mpu925x_14_bit:
			buffer |= 0 << 4;
			mpu925x->settings.magnetometer.lsb = MAGNETOMETER_SCALE_14_BIT;
			break;
		default:
		case mpu925x_16_bit:
			buffer |= 1 << 4;
			mpu925x->settings.magnetometer.lsb = MAGNETOMETER_SCALE_16_BIT;
			break;
	}

	mpu925x->master_specific.bus_write(mpu925x, AK8963_ADDRESS, CNTL1, &buffer, 1);

	mpu925x->master_specific.delay_ms(mpu925x, 100);
}

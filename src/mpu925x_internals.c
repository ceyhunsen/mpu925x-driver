/**
 * @file mpu925x_internals.c
 * @author Ceyhun Şen
 * @brief Internal functions for MPU-925X driver.
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

/**
 * @brief Initialize accelerometer and gyro.
 * @param mpu925x MPU-925X struct pointer.
 * @returns 0 on success, 1 on failure.
 * */
// uint8_t __mpu925x_init(mpu925x_t *mpu925x)
// {
// 	uint8_t buffer;

	

	// // Enable PLL.
	// mpu925x_set_clock_source(mpu925x, mpu925x_auto_select_pll);


	// Set acceleration range.
	// mpu925x_set_accelerometer_scale(mpu925x, mpu925x->settings.accelerometer_scale);

	// // Set gyro range.
	// mpu925x_set_gyroscope_scale(mpu925x, mpu925x->settings.gyroscope_scale);

	// Set temperature lsb.
	// mpu925x->settings.temperature_lsb = TEMPERATURE_SCALE;

// 	return 0;
// }

/**
 * @brief Initialize magnetometer.
 * @param mpu925x MPU-925X struct pointer.
 * @returns 0 on success, 1 on failure.
 * */
// uint8_t __ak8963_init(mpu925x_t *mpu925x)
// {
// 	uint8_t buffer;

	


	// Set power down mode.
	// mpu925x_set_magnetometer_measurement_mode(mpu925x, mpu925x_power_down_mode);

	// // Enable Fuse ROM access mode.
	// mpu925x_set_magnetometer_measurement_mode(mpu925x, mpu925x_fuse_rom_access_mode);

	// // Read coefficient data and save it.
	// uint8_t coef_data[3];
	// mpu925x->master_specific.bus_read(mpu925x, AK8963_ADDRESS, ASAX, coef_data, 3);
	// for (uint8_t i = 0; i < 3; i++) {
	// 	mpu925x->settings.magnetometer_coefficient[i] = (coef_data[i] - 128) * 0.5 / 128 + 1;
	// }

	// // Set power down mode.
	// mpu925x_set_magnetometer_measurement_mode(mpu925x, mpu925x_power_down_mode);

	// // Set measurement and bit mode.
	// mpu925x_set_magnetometer_measurement_mode(mpu925x, mpu925x_continuous_measurement_mode_2);
	// mpu925x_set_magnetometer_bit_mode(mpu925x, mpu925x_16_bit);

// 	return 0;
// }

/**
 * @brief Write data to bus whilst preserving other bits (not ready for usage).
 * @param mpu925x MPU-925X struct pointer.
 * */
// void mpu925x_bus_write_preserve(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size, uint8_t and_sentence)
// {
// 	uint8_t read_buffer;

// 	for (uint16_t i = 0; i < size; i++) {
// 		mpu925x->master_specific.bus_read(mpu925x, mpu925x->settings.address, reg, &read_buffer, size);
// 		read_buffer &= and_sentence;
// 		read_buffer |= buffer[i];
// 		mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, reg, &read_buffer, size);
// 	}
// }


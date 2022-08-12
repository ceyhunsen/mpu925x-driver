/**
 * @file mpu925x_settings.c
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

	mpu925x->master_specific.bus_write(mpu925x, mpu925x->settings.address, PWR_MGMT_1, &buffer, 1);
}

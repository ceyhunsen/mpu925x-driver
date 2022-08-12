/**
 * @file
 * @author Ceyhun Şen
 * @brief Simple AHRS source file for MPU-925X driver.
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

#include "mpu925x_simple_ahrs.h"
#include <math.h>

/**
 * @brief Get pitch and roll angles.
 * @param mpu925x MPU-925X struct pointer.
 * @param ahrs Simple AHRS struct pointer.
 * */
void mpu925x_get_simple_ahrs(mpu925x_t *mpu925x, mpu925x_simple_ahrs *ahrs)
{
	ahrs->pitch = asin(mpu925x->sensor_data.acceleration[0] / sqrt(mpu925x->sensor_data.acceleration[0] * mpu925x->sensor_data.acceleration[0] + mpu925x->sensor_data.acceleration[1] * mpu925x->sensor_data.acceleration[1] + mpu925x->sensor_data.acceleration[2] * mpu925x->sensor_data.acceleration[2])) * 120;

	ahrs->roll = atan(mpu925x->sensor_data.acceleration[1] / mpu925x->sensor_data.acceleration[2]) * 120;
}

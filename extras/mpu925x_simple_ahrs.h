/**
 * @file
 * @author Ceyhun Şen
 * @brief Simple AHRS header file for MPU-925X driver.
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

#ifndef __MPU925X_SIMPLE_AHRS_H
#define __MPU925X_SIMPLE_AHRS_H

#include "mpu925x.h"

/**
 * @brief Pitch and roll angles for simple AHRS.
 * */
typedef struct mpu925x_simple_ahrs {
	float pitch;
	float roll;
} mpu925x_simple_ahrs;

void mpu925x_get_simple_ahrs(mpu925x_t *mpu925x, mpu925x_simple_ahrs *ahrs);

#endif // __MPU925X_SIMPLE_AHRS_H

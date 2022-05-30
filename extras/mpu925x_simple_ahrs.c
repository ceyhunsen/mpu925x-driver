/**
 * @file
 * @author Ceyhun Şen
 * @brief Simple AHRS source file for MPU-925X driver.
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

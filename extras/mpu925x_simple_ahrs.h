/**
 * @file
 * @author Ceyhun Şen
 * @brief Simple AHRS header file for MPU-925X driver.
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

/**
 * @file
 * @author Ceyhun Şen
 * @brief Internal definitions for MPU-925X driver.
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

#ifndef __MPU925X_INTERNALS_H
#define __MPU925X_INTERNALS_H

// C++ compatibility.
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "mpu925x.h"

// Internal functions
uint8_t __mpu925x_init(mpu925x_t *mpu925x);
uint8_t __ak8963_init(mpu925x_t *mpu925x);

void mpu925x_reset(mpu925x_t *mpu925x);
void ak8963_reset(mpu925x_t *mpu925x);

void mpu925x_get_accelerometer_bias(mpu925x_t *mpu925x, int16_t *bias);

void mpu925x_bus_write_preserve(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size, uint8_t and_sentence);

#define convert8bitto16bit(x, y)   (((x) << 8) | (y))
#define powerof2(x)                (1 << (x))

// Slave addresses
#define MPU925X_ADDRESS            0b1101000
#define AK8963_ADDRESS             0x0C

// Accelerometer lsb values
#define ACCELEROMETER_SCALE_2G     16384
#define ACCELEROMETER_SCALE_4G     8192
#define ACCELEROMETER_SCALE_8G     4096
#define ACCELEROMETER_SCALE_16G    2048

// Gyroscope lsb values
#define GYROSCOPE_SCALE_250_DPS    131.0
#define GYROSCOPE_SCALE_500_DPS    65.5
#define GYROSCOPE_SCALE_1000_DPS   32.8
#define GYROSCOPE_SCALE_2000_DPS   16.4

// Magnetometer lsb values
#define MAGNETOMETER_SCALE_14_BIT  (4800.0 / 16383.0)
#define MAGNETOMETER_SCALE_16_BIT  (4800.0 / INT16_MAX)

// Temperature lsb values
#define TEMPERATURE_SCALE          333.87

// MPU-925X registers
#define SELF_TEST_X_GYRO           0x00
#define SELF_TEST_Y_GYRO           0x01
#define SELF_TEST_Z_GYRO           0x02
#define SELF_TEST_X_ACCEL          0x0D
#define SELF_TEST_Y_ACCEL          0x0E
#define SELF_TEST_Z_ACCEL          0x0F
#define XG_OFFSET_H                0x13
#define XG_OFFSET_L                0x14
#define YG_OFFSET_H                0x15
#define YG_OFFSET_L                0x16
#define ZG_OFFSET_H                0x17
#define ZG_OFFSET_L                0x18
#define SMPLRT_DIV                 0x19
#define CONFIG                     0x1A
#define GYRO_CONFIG                0x1B
#define ACCEL_CONFIG               0x1C
#define ACCEL_CONFIG_2             0x1D
#define LP_ACCEL_ODR               0x1E
#define WOM_THR                    0x1F
#define FIFO_EN                    0x23
#define I2C_MST_CTRL               0x24
#define I2C_SLV0_ADDR              0x25
#define I2C_SLV0_REG               0x26
#define I2C_SLV0_CTRL              0x27
#define I2C_SLV1_ADDR              0x28
#define I2C_SLV1_REG               0x29
#define I2C_SLV1_CTRL              0x2A
#define I2C_SLV2_ADDR              0x2B
#define I2C_SLV2_REG               0x2C
#define I2C_SLV2_CTRL              0x2D
#define I2C_SLV3_ADDR              0x2E
#define I2C_SLV3_REG               0x2F
#define I2C_SLV3_CTRL              0x30
#define I2C_SLV4_ADDR              0x31
#define I2C_SLV4_REG               0x32
#define I2C_SLV4_DO                0x33
#define I2C_SLV4_CTRL              0x34
#define I2C_SLV4_DI                0x35
#define I2C_MST_STATUS             0x36
#define INT_PIN_CFG                0x37
#define INT_ENABLE                 0x38
#define INT_STATUS                 0x3A
#define ACCEL_XOUT_H               0x3B
#define ACCEL_XOUT_L               0x3C
#define ACCEL_YOUT_H               0x3D
#define ACCEL_YOUT_L               0x3E
#define ACCEL_ZOUT_H               0x3F
#define ACCEL_ZOUT_L               0x40
#define TEMP_OUT_H                 0x41
#define TEMP_OUT_L                 0x42
#define GYRO_XOUT_H                0x43
#define GYRO_XOUT_L                0x44
#define GYRO_YOUT_H                0x45
#define GYRO_YOUT_L                0x46
#define GYRO_ZOUT_H                0x47
#define GYRO_ZOUT_L                0x48
#define EXT_SENS_DATA_00           0x49
#define EXT_SENS_DATA_01           0x4A
#define EXT_SENS_DATA_02           0x4B
#define EXT_SENS_DATA_03           0x4C
#define EXT_SENS_DATA_04           0x4D
#define EXT_SENS_DATA_05           0x4E
#define EXT_SENS_DATA_06           0x4F
#define EXT_SENS_DATA_07           0x50
#define EXT_SENS_DATA_08           0x51
#define EXT_SENS_DATA_09           0x52
#define EXT_SENS_DATA_10           0x53
#define EXT_SENS_DATA_11           0x54
#define EXT_SENS_DATA_12           0x55
#define EXT_SENS_DATA_13           0x56
#define EXT_SENS_DATA_14           0x57
#define EXT_SENS_DATA_15           0x58
#define EXT_SENS_DATA_16           0x59
#define EXT_SENS_DATA_17           0x5A
#define EXT_SENS_DATA_18           0x5B
#define EXT_SENS_DATA_19           0x5C
#define EXT_SENS_DATA_20           0x5D
#define EXT_SENS_DATA_21           0x5E
#define EXT_SENS_DATA_22           0x5F
#define EXT_SENS_DATA_23           0x60
#define I2C_SLV0_DO                0x63
#define I2C_SLV1_DO                0x64
#define I2C_SLV2_DO                0x65
#define I2C_SLV3_DO                0x66
#define I2C_MST_DELAY_CTRL         0x67
#define SIGNAL_PATH_RESET          0x68
#define MOT_DETECT_CTRL            0x69
#define USER_CTRL                  0x6A
#define PWR_MGMT_1                 0x6B
#define PWR_MGMT_2                 0x6C
#define FIFO_COUNTH                0x72
#define FIFO_COUNTL                0x73
#define FIFO_R_W                   0x74
#define WHO_AM_I                   0x75
#define XA_OFFSET_H                0x77
#define XA_OFFSET_L                0x78
#define YA_OFFSET_H                0x7A
#define YA_OFFSET_L                0x7B
#define ZA_OFFSET_H                0x7D
#define ZA_OFFSET_L                0x7E

// AK8963 registers
#define WIA                        0x00
#define INFO                       0x01
#define ST1                        0x02
#define HXL                        0x03
#define HXH                        0x04
#define HYL                        0x05
#define HYH                        0x06
#define HZL                        0x07
#define HZH                        0x08
#define ST2                        0x09
#define CNTL1                      0x0A
#define CNTL2                      0x0B
#define ASTC                       0x0C
#define TS1                        0x0D
#define TS2                        0x0E
#define I2CDIS                     0x0F
#define ASAX                       0x10
#define ASAY                       0x11
#define ASAZ                       0x12

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __MPU925X_INTERNALS_H

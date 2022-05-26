/**
 * @file
 * @author Ceyhun Şen
 * @brief Test header for MPU-925X driver.
 * */

#ifndef __TEST_H
#define __TEST_H

#include "unity.h"
#include "mpu925x.h"
#include "mpu925x_internals.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t mpuvirtmem[256];
uint8_t akvirtmem[256];

uint8_t mock_read(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
{
	if (slave_address == MPU925X_ADDRESS) {
		for (uint16_t i = 0; i < size; i++) {
			buffer[i] = mpuvirtmem[reg + i];
		}
	}
	if (slave_address == AK8963_ADDRESS) {
		for (uint16_t i = 0; i < size; i++) {
			buffer[i] = akvirtmem[reg + i];
		}
	}

	return 0;
}

uint8_t mock_write(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
{
	if (slave_address == MPU925X_ADDRESS) {
		for (uint16_t i = 0; i < size; i++) {
			mpuvirtmem[reg + i] = buffer[i];
		}
	}
	if (slave_address == AK8963_ADDRESS) {
		for (uint16_t i = 0; i < size; i++) {
			akvirtmem[reg + i] = buffer[i];
		}
	}

	return 0;
}

void mock_delay(mpu925x_t *mpu925x, uint32_t delay) {}

// Create mpu925x_t struct instance.
mpu925x_t mpu925x = {
	.master_specific = {
		// Bus functions
		.bus_read = mock_read,
		.bus_write = mock_write,
		.delay_ms = mock_delay
	},

	.settings = {
		// Other settings
		.accelerometer_scale = mpu925x_2g,
		.gyroscope_scale = mpu925x_250dps,
		.orientation = mpu925x_z_plus
	}
};

#endif // __TEST_H

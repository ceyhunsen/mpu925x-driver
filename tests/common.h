/**
 * @file common.h
 * @author Ceyhun Şen
 * @brief Common stuff for tests.
 */

#ifndef __COMMON_H
#define __COMMON_H

#include "mpu925x_internals.h"
#include "mpu925x.h"
#include "unity.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define VIRT_MEMORY_SIZE 256

uint8_t mpu_virt_mem[VIRT_MEMORY_SIZE];
uint8_t ak_virt_mem[VIRT_MEMORY_SIZE];

/**
 * @brief Read data from virtual memory.
 * 
 * @param mpu925x Main struct pointer.
 * @param slave_address Slave address of the sensor.
 * @param reg Register.
 * @param buffer Data buffer.
 * @param size Data buffer size.
 * @return uint8_t 0
 */
uint8_t mock_read(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
{
	if (slave_address == MPU925X_ADDRESS) {
		for (uint16_t i = 0; i < size; i++) {
			buffer[i] = mpu_virt_mem[reg + i];
		}
	}
	if (slave_address == AK8963_ADDRESS) {
		for (uint16_t i = 0; i < size; i++) {
			buffer[i] = ak_virt_mem[reg + i];
		}
	}

	return 0;
}

/**
 * @brief Write data to virtual memory.
 * 
 * @param mpu925x Main struct pointer.
 * @param slave_address Slave address of the sensor.
 * @param reg Register.
 * @param buffer Data buffer.
 * @param size Data buffer size.
 * @return uint8_t 0
 */
uint8_t mock_write(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
{
	if (slave_address == MPU925X_ADDRESS) {
		for (uint16_t i = 0; i < size; i++) {
			mpu_virt_mem[reg + i] = buffer[i];
		}
	}
	if (slave_address == AK8963_ADDRESS) {
		for (uint16_t i = 0; i < size; i++) {
			ak_virt_mem[reg + i] = buffer[i];
		}
	}

	return 0;
}

/**
 * @brief Mock delay function. This function is not necessary on tests but
 * necessary for driver.
 * 
 * @param mpu925x Main struct pointer.
 * @param delay Delay time in milliseconds.
 */
void mock_delay(mpu925x_t *mpu925x, uint32_t delay)
{

}

// Create mpu925x_t struct instance.
mpu925x_t mpu925x = {
	.master_specific = {
		// Bus functions
		.bus_read = mock_read,
		.bus_write = mock_write,
		.delay_ms = mock_delay
	},

	.settings = {
		.general = {
			.orientation = mpu925x_z_plus
		}
	}
};

/**
 * @brief Setup function for Unity.
 */
void setUp()
{
	// Clean virtual memory.
	memset(mpu_virt_mem, 0, sizeof(mpu_virt_mem));
	memset(ak_virt_mem, 0, sizeof(ak_virt_mem));

	// Set WHO_AM_I and WIA registers.
	mpu_virt_mem[WHO_AM_I] = 0x73;
	ak_virt_mem[WIA] = 0x48;

	// Set accelerometer values.
	mpu_virt_mem[ACCEL_ZOUT_L] = ACCELEROMETER_SCALE_2G & 0xFF;
	mpu_virt_mem[ACCEL_ZOUT_H] = ACCELEROMETER_SCALE_2G >> 8;
}

/**
 * @brief Teardown function for Unity.
 */
void tearDown()
{

}

#endif // __COMMON_H

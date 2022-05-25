/**
 * @file
 * @author Ceyhun Şen
 * @brief Test file for MPU-925X driver.
 * */

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
		.accelerometer_scale = _2g,
		.gyroscope_scale = _250dps,
		.orientation = z_plus
	}
};

/*******************************************************************************
 * Tests Start
 ******************************************************************************/

void test_mock()
{
	uint8_t address = I2C_SLV1_DO;
	uint8_t tmp = 10;

	mpu925x.master_specific.bus_write(&mpu925x, MPU925X_ADDRESS, address, &tmp, 1);

	TEST_ASSERT_EQUAL(mpuvirtmem[address], tmp);

	uint8_t tmp2 = 11;

	TEST_ASSERT_NOT_EQUAL(tmp, tmp2);

	mpu925x.master_specific.bus_read(&mpu925x, MPU925X_ADDRESS, address, &tmp2, 1);

	TEST_ASSERT_EQUAL(tmp, tmp2);
}

void test_init()
{
	uint8_t ret = mpu925x_init(&mpu925x, 0);

	TEST_ASSERT_EQUAL(ret, 0);
}
 
/*******************************************************************************
 * Tests End
 ******************************************************************************/

void setUp()
{
	memset(mpuvirtmem, 0, sizeof(mpuvirtmem));
	mpuvirtmem[WHO_AM_I] = 0x73;

	memset(akvirtmem, 0, sizeof(akvirtmem));
	akvirtmem[WIA] = 0x48;
}

void tearDown()
{
	printf("---------------------------------------------------------------\n");
	printf("Register values after test (no 0's):\n");
	printf("MPU-925X:\n");
	for (uint16_t i = 0; i < 256; i++) {
		if (mpuvirtmem[i]) {
			printf("%u:\t%u\n", i, mpuvirtmem[i]);
		}
	}
	printf("AK8963:\n");
	for (uint16_t i = 0; i < 256; i++) {
		if (akvirtmem[i]) {
			printf("%u:\t%u\n", i, akvirtmem[i]);
		}
	}
}

int main()
{
	RUN_TEST(test_mock);
	RUN_TEST(test_init);
	return UnityEnd();
}

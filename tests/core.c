/**
 * @file core.c
 * @author Ceyhun Şen
 * @brief Test file for core.
 */

#include "common.h"

void test_init()
{
	// Initialize driver.
	uint8_t ret = mpu925x_init(&mpu925x, 0);

	// Check if successfull.
	TEST_ASSERT_EQUAL(ret, 0);
}

void test_i2c_bypass()
{
	// Initialize driver.
	mpu925x_init(&mpu925x, 0);

	TEST_ASSERT_EQUAL(mpu_virt_mem[INT_PIN_CFG], 1 << 1);
}

void test_accelerometer()
{
	// Initialize driver.
	mpu925x_init(&mpu925x, 0);

	// Read sensor data.
	mpu925x_get_acceleration(&mpu925x);

	// By default, sensor should read 1g on z axis and 0g on both x and y axises.
	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.acceleration[0], 0.0f);
	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.acceleration[1], 0.0f);
	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.acceleration[2], 1.0f);
}

int main()
{
	RUN_TEST(test_init);
	RUN_TEST(test_i2c_bypass);
	RUN_TEST(test_accelerometer);

	return UnityEnd();
}

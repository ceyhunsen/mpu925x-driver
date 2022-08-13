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

	// Change x axis to 0.5g.
	int16_t value = ACCELEROMETER_SCALE_2G / 2;
	mpu_virt_mem[ACCEL_XOUT_L] = value & 0xFF;
	mpu_virt_mem[ACCEL_XOUT_H] = value >> 8;

	// Read sensor data.
	mpu925x_get_acceleration(&mpu925x);

	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.acceleration[0], 0.5f);
	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.acceleration[1], 0.0f);
	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.acceleration[2], 1.0f);
}

void test_gyroscope()
{
	// Initialize driver.
	mpu925x_init(&mpu925x, 0);

	// Read sensor data.
	mpu925x_get_rotation(&mpu925x);

	// By default, sensor should read 0 dps on all axises.
	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.rotation[0], 0.0f);
	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.rotation[1], 0.0f);
	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.rotation[2], 0.0f);

	// Change x axis to 3 dps and y axis to 5.5 dps.
	int16_t value = GYROSCOPE_SCALE_250_DPS * 3;
	mpu_virt_mem[GYRO_XOUT_L] = value & 0xFF;
	mpu_virt_mem[GYRO_XOUT_H] = value >> 8;
	value = GYROSCOPE_SCALE_250_DPS * 5.5;
	mpu_virt_mem[GYRO_YOUT_L] = value & 0xFF;
	mpu_virt_mem[GYRO_YOUT_H] = value >> 8;

	// Read sensor data.
	mpu925x_get_rotation(&mpu925x);

	// Y axis is checked with integer conversion because of loss in integer
	// conversion in previous lines..
	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.rotation[0], 3.0f);
	TEST_ASSERT_EQUAL(mpu925x.sensor_data.rotation[1], 5);
	TEST_ASSERT_EQUAL_FLOAT(mpu925x.sensor_data.rotation[2], 0.0f);
}

int main()
{
	RUN_TEST(test_init);
	RUN_TEST(test_i2c_bypass);
	RUN_TEST(test_accelerometer);
	RUN_TEST(test_gyroscope);

	return UnityEnd();
}

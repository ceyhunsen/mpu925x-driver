/**
 * @file accelerometer.c
 * @author Ceyhun Şen
 * @brief Test file for accelerometer.
 */

#include "common.h"

void test_accelerometer_scale()
{
	mpu925x_set_accelerometer_scale(&mpu925x, mpu925x_8g);

	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG], 0b10 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.acceleration_lsb, ACCELEROMETER_SCALE_8G);

	mpu925x_set_accelerometer_scale(&mpu925x, mpu925x_2g);

	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG], 0b00 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.acceleration_lsb, ACCELEROMETER_SCALE_2G);

	mpu925x_set_accelerometer_scale(&mpu925x, mpu925x_16g);

	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG], 0b11 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.acceleration_lsb, ACCELEROMETER_SCALE_16G);

	mpu925x_set_accelerometer_scale(&mpu925x, mpu925x_4g);

	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG], 0b01 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.acceleration_lsb, ACCELEROMETER_SCALE_4G);
}

int main()
{
	RUN_TEST(test_accelerometer_scale);

	return UnityEnd();
}

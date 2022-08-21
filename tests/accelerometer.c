/**
 * @file accelerometer.c
 * @author Ceyhun Şen
 * @brief Test file for accelerometer.
 */

#include "common.h"

void test_scale()
{
	// Set scale to 2g.
	mpu925x_accelerometer_set_scale(&mpu925x, mpu925x_2g);

	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG], 0 << 3);

	// Set scale to 4g.
	mpu925x_accelerometer_set_scale(&mpu925x, mpu925x_4g);

	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG], 1 << 3);

	// Set scale to 8g.
	mpu925x_accelerometer_set_scale(&mpu925x, mpu925x_8g);

	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG], 2 << 3);

	// Set scale to 16g.
	mpu925x_accelerometer_set_scale(&mpu925x, mpu925x_16g);

	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG], 3 << 3);
}

void test_dlpf()
{
	uint8_t buffer = 0;

	// Set DLPF to 1.13 KHz.
	mpu925x_accelerometer_set_dlpf(&mpu925x, mpu925x_1_13k);

	buffer = (1 << 3);
	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG_2], buffer);

	// Set DLPF to 92 Hz.
	mpu925x_accelerometer_set_dlpf(&mpu925x, mpu925x_92);

	buffer = (0 << 3) | (2 << 2);
	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG_2], buffer);

	// Set DLPF to 5 Hz.
	mpu925x_accelerometer_set_dlpf(&mpu925x, mpu925x_5);

	buffer = (0 << 3) | (6 << 2);
	TEST_ASSERT_EQUAL(mpu_virt_mem[ACCEL_CONFIG_2], buffer);
}

int main()
{
	RUN_TEST(test_scale);
	RUN_TEST(test_dlpf);

	return UnityEnd();
}

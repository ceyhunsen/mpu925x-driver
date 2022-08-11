/**
 * @file mock.c
 * @author Ceyhun Şen
 * @brief Tests for mock interface.
 */

#include "common.h"

void test_mock_mpu_write()
{
	uint8_t write_data = 0x1F;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, MPU925X_ADDRESS, 0, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(mpu_virt_mem[0], write_data);
}

void test_mock_mpu_read()
{
	uint8_t write_data = 0x1F, read_data = 0x00;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, MPU925X_ADDRESS, 0, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(mpu_virt_mem[0], write_data);

	// Check initial state is not the same.
	TEST_ASSERT_NOT_EQUAL(mpu_virt_mem[0], read_data);

	// Read data using bus function.
	mpu925x.master_specific.bus_read(&mpu925x, MPU925X_ADDRESS, 0, &read_data, sizeof read_data);

	// Check read data.
	TEST_ASSERT_EQUAL(mpu_virt_mem[0], read_data);
	TEST_ASSERT_EQUAL(write_data, read_data);
}

int main()
{
	RUN_TEST(test_mock_mpu_write);
	RUN_TEST(test_mock_mpu_read);

	return UNITY_END();
}

/**
 * @file mock.c
 * @author Ceyhun Şen
 * @brief Tests for mock interface.
 */

#include "common.h"

/**
 * @brief Test mock write interface for MPU.
 */
void test_mock_mpu_write()
{
	uint8_t address_start = 0, write_data = 0x1F;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, MPU925X_ADDRESS, address_start, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(mpu_virt_mem[address_start], write_data);
}

/**
 * @brief Test mock read interface for MPU.
 */
void test_mock_mpu_read()
{
	uint8_t address_start = 0, write_data = 0x1F, read_data = 0x00;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, MPU925X_ADDRESS, address_start, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(mpu_virt_mem[address_start], write_data);

	// Check initial state is not the same.
	TEST_ASSERT_NOT_EQUAL(mpu_virt_mem[address_start], read_data);

	// Read data using bus function.
	mpu925x.master_specific.bus_read(&mpu925x, MPU925X_ADDRESS, address_start, &read_data, sizeof read_data);

	// Check read data.
	TEST_ASSERT_EQUAL(mpu_virt_mem[address_start], read_data);
	TEST_ASSERT_EQUAL(write_data, read_data);
}

/**
 * @brief Test mock write interface for AK.
 */
void test_mock_ak_write()
{
	uint8_t address_start = 0, write_data = 0x1F;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, AK8963_ADDRESS, address_start, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(ak_virt_mem[address_start], write_data);
}

/**
 * @brief Test mock read interface for AK.
 */
void test_mock_ak_read()
{
	uint8_t address_start = 0, write_data = 0x1F, read_data = 0x00;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, AK8963_ADDRESS, address_start, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(ak_virt_mem[address_start], write_data);

	// Check initial state is not the same.
	TEST_ASSERT_NOT_EQUAL(ak_virt_mem[address_start], read_data);

	// Read data using bus function.
	mpu925x.master_specific.bus_read(&mpu925x, AK8963_ADDRESS, address_start, &read_data, sizeof read_data);

	// Check read data.
	TEST_ASSERT_EQUAL(ak_virt_mem[address_start], read_data);
	TEST_ASSERT_EQUAL(write_data, read_data);
}

int main()
{
	RUN_TEST(test_mock_mpu_write);
	RUN_TEST(test_mock_mpu_read);
	RUN_TEST(test_mock_ak_write);
	RUN_TEST(test_mock_ak_read);

	return UNITY_END();
}

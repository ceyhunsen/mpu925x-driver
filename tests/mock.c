/**
 * @file mock.c
 * @author Ceyhun Şen
 * @brief Tests for mock interface.
 */

#include "common.h"

/**
 * Write random data random virtual memory address.
 * @see test_virt_mem_reset_2
 */
void test_virt_mem_reset_1()
{
	uint8_t tmp_address = 10;

	TEST_ASSERT_NOT_EQUAL(mpu_virt_mem[tmp_address], 0xEF);
	mpu_virt_mem[tmp_address] = 0xEF;

	TEST_ASSERT_NOT_EQUAL(ak_virt_mem[tmp_address], 0xEF);
	ak_virt_mem[tmp_address] = 0xEF;
}

/**
 * Check if it's deleted in new test.
 * @see test_virt_mem_reset_1
 */
void test_virt_mem_reset_2()
{
	uint8_t tmp_address = 10;

	TEST_ASSERT_NOT_EQUAL(mpu_virt_mem[tmp_address], 0xEF);

	TEST_ASSERT_NOT_EQUAL(ak_virt_mem[tmp_address], 0xEF);
}

void test_mock_mpu_write()
{
	uint8_t address_start = 10, write_data = 0x1F;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, MPU925X_ADDRESS, address_start, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(mpu_virt_mem[address_start], write_data);
}

void test_mock_mpu_read()
{
	uint8_t address_start = 10, write_data = 0x1F, read_data = 0x00;

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

void test_mock_ak_write()
{
	uint8_t address_start = 10, write_data = 0x1F;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, AK8963_ADDRESS, address_start, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(ak_virt_mem[address_start], write_data);
}

void test_mock_ak_read()
{
	uint8_t address_start = 10, write_data = 0x1F, read_data = 0x00;

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

void test_mock_mpu_write_edge()
{
	uint8_t address_start = 0, write_data = 0x1F;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, MPU925X_ADDRESS, address_start, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(mpu_virt_mem[address_start], write_data);

	// Write to the end.
	address_start = 0xFF;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, MPU925X_ADDRESS, address_start, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(mpu_virt_mem[address_start], write_data);
}

void test_mock_mpu_read_edge()
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

	// Read from end.
	address_start = 0xFF, read_data = 0x00;

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

void test_mock_ak_write_edge()
{
	uint8_t address_start = 0, write_data = 0x1F;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, AK8963_ADDRESS, address_start, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(ak_virt_mem[address_start], write_data);

	// Write to the end.
	address_start = 0xFF;

	// Write data using bus function.
	mpu925x.master_specific.bus_write(&mpu925x, AK8963_ADDRESS, address_start, &write_data, sizeof write_data);

	// Check written data.
	TEST_ASSERT_EQUAL(ak_virt_mem[address_start], write_data);
}

void test_mock_ak_read_edge()
{
	uint8_t address_start = 10, write_data = 0x1F, read_data = 0x00;

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

	// Read from end.
	address_start = 0xFF, read_data = 0x00;

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

void test_write_preserve()
{
	// Data should be changed from 26 to 25 in this test.

	// Set previous data.
	uint8_t previous_data = (3 << 3) | (2 << 0);
	mpu_virt_mem[GYRO_CONFIG] = previous_data;

	// Check previous data is 26.
	TEST_ASSERT_EQUAL(mpu_virt_mem[GYRO_CONFIG], 26);

	// Create data to be written.
	uint8_t write_data = 1 << 0;

	// Write using interface.
	__mpu925x_bus_write_preserve(&mpu925x, MPU925X_ADDRESS, GYRO_CONFIG, &write_data, sizeof write_data, 0b11);

	// Check both are written.
	TEST_ASSERT_EQUAL(mpu_virt_mem[GYRO_CONFIG], 25);

	// Check it's not a simple or operation.
	TEST_ASSERT_NOT_EQUAL(mpu_virt_mem[GYRO_CONFIG], previous_data | write_data);
}

int main()
{
	RUN_TEST(test_virt_mem_reset_1);
	RUN_TEST(test_virt_mem_reset_2);

	RUN_TEST(test_mock_mpu_write);
	RUN_TEST(test_mock_mpu_read);
	RUN_TEST(test_mock_ak_write);
	RUN_TEST(test_mock_ak_read);
	RUN_TEST(test_mock_mpu_write_edge);
	RUN_TEST(test_mock_mpu_read_edge);
	RUN_TEST(test_mock_ak_write_edge);
	RUN_TEST(test_mock_ak_read_edge);

	RUN_TEST(test_write_preserve);

	return UNITY_END();
}

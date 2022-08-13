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

int main()
{
	RUN_TEST(test_init);
	RUN_TEST(test_i2c_bypass);

	return UnityEnd();
}

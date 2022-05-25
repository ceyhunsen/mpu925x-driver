/**
 * @file
 * @author Ceyhun Şen
 * @brief Test file for MPU-925X driver.
 * */

#include "test.h"

/**
 * Test mock functions.
 * */
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

/**
 * Test initialization.
 * */
void test_init()
{
	uint8_t ret = mpu925x_init(&mpu925x, 0);

	TEST_ASSERT_EQUAL(ret, 0);
}
 
//******************************************************************************

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

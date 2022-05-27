/**
 * @file
 * @author Ceyhun Şen
 * @brief Test file for MPU-925X driver.
 * */

#include "test.h"

//~ #define PRINT_REGISTERS

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

void test_accelerometer_scale()
{
	mpu925x_set_accelerometer_scale(&mpu925x, mpu925x_8g);

	TEST_ASSERT_EQUAL(mpuvirtmem[ACCEL_CONFIG], 0b10 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.acceleration_lsb, ACCELEROMETER_SCALE_8G);

	mpu925x_set_accelerometer_scale(&mpu925x, mpu925x_2g);

	TEST_ASSERT_EQUAL(mpuvirtmem[ACCEL_CONFIG], 0b00 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.acceleration_lsb, ACCELEROMETER_SCALE_2G);

	mpu925x_set_accelerometer_scale(&mpu925x, mpu925x_16g);

	TEST_ASSERT_EQUAL(mpuvirtmem[ACCEL_CONFIG], 0b11 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.acceleration_lsb, ACCELEROMETER_SCALE_16G);

	mpu925x_set_accelerometer_scale(&mpu925x, mpu925x_4g);

	TEST_ASSERT_EQUAL(mpuvirtmem[ACCEL_CONFIG], 0b01 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.acceleration_lsb, ACCELEROMETER_SCALE_4G);
}

void test_gyroscope_scale()
{
	mpu925x_set_gyroscope_scale(&mpu925x, mpu925x_250dps);

	TEST_ASSERT_EQUAL(mpuvirtmem[GYRO_CONFIG], 0b00 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.gyroscope_lsb, GYROSCOPE_SCALE_250_DPS);

	mpu925x_set_gyroscope_scale(&mpu925x, mpu925x_500dps);

	TEST_ASSERT_EQUAL(mpuvirtmem[GYRO_CONFIG], 0b01 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.gyroscope_lsb, GYROSCOPE_SCALE_500_DPS);

	mpu925x_set_gyroscope_scale(&mpu925x, mpu925x_1000dps);

	TEST_ASSERT_EQUAL(mpuvirtmem[GYRO_CONFIG], 0b10 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.gyroscope_lsb, GYROSCOPE_SCALE_1000_DPS);

	mpu925x_set_gyroscope_scale(&mpu925x, mpu925x_2000dps);

	TEST_ASSERT_EQUAL(mpuvirtmem[GYRO_CONFIG], 0b11 << 3);
	TEST_ASSERT_EQUAL(mpu925x.settings.gyroscope_lsb, GYROSCOPE_SCALE_2000_DPS);
}

void test_sensor_values()
{
	mpu925x_set_accelerometer_scale(&mpu925x, mpu925x_2g);
	mpu925x_set_gyroscope_scale(&mpu925x, mpu925x_250dps);

	TEST_ASSERT_EQUAL(mpu925x.sensor_data.acceleration_raw[0], 0);
	TEST_ASSERT_EQUAL(mpu925x.sensor_data.rotation_raw[0], 0);

	mpu925x_get_all(&mpu925x);

	TEST_ASSERT_EQUAL(mpu925x.sensor_data.acceleration_raw[0], 0xFF);
	TEST_ASSERT_EQUAL(mpu925x.sensor_data.rotation_raw[0], 0xFF);
}

//******************************************************************************

void setUp()
{
	memset(mpuvirtmem, 0, sizeof(mpuvirtmem));
	// Set WHO_AM_I register.
	mpuvirtmem[WHO_AM_I] = 0x73;
	// Set default sensor values.
	mpuvirtmem[ACCEL_XOUT_L] = 0xFF;
	mpuvirtmem[ACCEL_YOUT_L] = 0xFF;
	mpuvirtmem[ACCEL_ZOUT_L] = 0xFF;
	mpuvirtmem[GYRO_XOUT_L] = 0xFF;
	mpuvirtmem[GYRO_YOUT_L] = 0xFF;
	mpuvirtmem[GYRO_ZOUT_L] = 0xFF;

	memset(akvirtmem, 0, sizeof(akvirtmem));
	// Set WIA register.
	akvirtmem[WIA] = 0x48;
}

void tearDown()
{
	#ifdef PRINT_REGISTERS
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
	#endif // PRINT_REGISTERS
}

int main()
{
	RUN_TEST(test_mock);
	RUN_TEST(test_init);
	RUN_TEST(test_accelerometer_scale);
	RUN_TEST(test_gyroscope_scale);
	RUN_TEST(test_sensor_values);
	return UnityEnd();
}

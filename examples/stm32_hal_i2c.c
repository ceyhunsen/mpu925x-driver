/**
 * @file
 * @author Ceyhun Şen
 * @brief Example usage for MPU-925X driver.
 * It probably won't compile, so just use this file as an example and modify 
 * your own code.
 * */

// Change this library to your STM32's HAL library.
#include "stm32f4xx_hal.h"

#include "mpu925x.h"
#include <stdint.h>

I2C_HandleTypeDef hi2c1;

// Define needed interfaces.

/**
 * @brief Bus read interface.
 * */
uint8_t mpu925x_stm32_i2c_hal_read(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
{
	return HAL_I2C_Mem_Read(mpu925x->master_specific.bus_handle, slave_address << 1, reg, 1, buffer, size, HAL_MAX_DELAY);
}

/**
 * @brief Bus write interface.
 * */
uint8_t mpu925x_stm32_i2c_hal_write(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
{
	return HAL_I2C_Mem_Write(mpu925x->master_specific.bus_handle, slave_address << 1, reg, 1, buffer, size, HAL_MAX_DELAY);
}

/**
 * @brief Bus wait ms interface.
 * */
void mpu925x_stm32_hal_delay_ms(mpu925x_t *mpu925x, uint32_t delay)
{
	HAL_Delay(delay);
}

int main()
{
	// ...

	// Create mpu925x_t struct instance.
	mpu925x_t mpu925x = {
		.master_specific = {
			// STM32 HAL I2C handle
			.bus_handle = &hi2c1,

			// Bus functions
			.bus_read = mpu925x_stm32_i2c_hal_read,
			.bus_write = mpu925x_stm32_i2c_hal_write,
			.delay_ms = mpu925x_stm32_hal_delay_ms
		},

		.settings = {
			// Other settings
			.accelerometer_scale = _2g,
			.gyroscope_scale = _250dps,
			.orientation = z_minus
		}
	};

	// Wait till' initializition is complete. Will be in endless loop if sensor
	// is unreachable (wiring is not correct, sensor is damaged...).
	while (mpu925x_init(&mpu925x, 0));

	while (1) {
		// Get sensor data.
		mpu925x_get_all(&mpu925x);

		// Use sensor data (e.g. print).
		printf("Acceleration: %f, %f, %f\n"
		       "Rotation: %f, %f, %f\n"
		       "Magnetic field: %f, %f, %f\n",
		       mpu925x.sensor_data.acceleration[0], mpu925x.sensor_data.acceleration[1], mpu925x.sensor_data.acceleration[2],
		       mpu925x.sensor_data.rotation[0], mpu925x.sensor_data.rotation[1], mpu925x.sensor_data.rotation[2],
		       mpu925x.sensor_data.magnetic_field[0], mpu925x.sensor_data.magnetic_field[1], mpu925x.sensor_data.magnetic_field[2],);
	}

	return 0;
}

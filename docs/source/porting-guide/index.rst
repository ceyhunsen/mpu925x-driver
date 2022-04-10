.. _porting-guide:

Porting Guide
=============

This driver needs:

* Bus read
* Bus write
* Delay microseconds
* Bus handle (optional, depending on your platform)

function and struct pointers. You must define your own functions and pass their pointers to ``mpu925x_t`` struct before initialization.

Given slave address is 7 bit, so if your bus interface needs an 8 bit address, shift slave address to 1 bit left (e.g. address << 1).

Bus Read Function
^^^^^^^^^^^^^^^^^

Bus read functions' prototype is like this:

.. code-block:: c

	uint8_t (*bus_read)(struct mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size);

This function wants to read ``size`` byte amount of data from bus slave's ``reg`` register which address is ``slave_address`` and stores it in ``buffer`` array.

Arduino I2C example:

.. code-block:: c

	uint8_t arduino_i2c_read(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
	{
	  Wire.begin();
	  Wire.beginTransmission(slave_address);
	  Wire.write(reg);
	  Wire.endTransmission(false);
	  Wire.requestFrom(slave_address, size);
	  for (uint16_t i = 0; i < size; i++) {
		if (Wire.available()) {
		  buffer[i] = (uint8_t)Wire.read();
		}
		else {
		  return 1;
		}
	  }
	  return 0;
	}

STM32 HAL I2C example:

.. code-block:: c

	uint8_t mpu925x_stm32_i2c_hal_read(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
	{
		return HAL_I2C_Mem_Read(mpu925x->master_specific.bus_handle, slave_address << 1, reg, 1, buffer, size, HAL_MAX_DELAY);
	}

	mpu925x.master_specific.bus_read = mpu925x_stm32_i2c_hal_read;

Bus Write Function
^^^^^^^^^^^^^^^^^^

Bus write function's prototype is like this:

.. code-block:: c

	uint8_t (*bus_write)(struct mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size);

This function wants to write ``size`` byte amount of data from ``buffer`` array to bus slave's ``reg`` register which address is ``slave_address``.

Arduino I2C example:

.. code-block:: c

	uint8_t arduino_i2c_write(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
	{
	  Wire.beginTransmission(slave_address);
	  Wire.write(reg);
	  for (uint16_t i = 0; i < size; i++) {
		Wire.write(buffer[i]);
	  }
	  Wire.endTransmission();
	  return 0;
	}

STM32 HAL I2C example:

.. code-block:: c

	uint8_t mpu925x_stm32_i2c_hal_write(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
	{
		return HAL_I2C_Mem_Write(mpu925x->master_specific.bus_handle, slave_address << 1, reg, 1, buffer, size, HAL_MAX_DELAY);
	}

	mpu925x.master_specific.bus_write = mpu925x_stm32_i2c_hal_write;

Delay Microseconds Function
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Delay microseconds functions prototype is like this:

.. code-block:: c

	void (*delay_ms)(struct mpu925x_t *mpu925x, uint32_t delay);

This functions wants to wait at least ``delay`` microseconds.

Arduino I2C example:

.. code-block:: c

	void arduino_delay_ms(mpu925x_t *mpu925x, uint32_t dly)
	{
	  delay(dly);
	}

STM32 HAL example:

.. code-block:: c

	void mpu925x_stm32_hal_delay_ms(mpu925x_t *mpu925x, uint32_t delay)
	{
		HAL_Delay(delay);
	}

	mpu925x.master_specific.delay_ms = mpu925x_stm32_hal_delay_ms;

Bus Handle Struct
^^^^^^^^^^^^^^^^^

Bus handle is for platform specific structs or other types of data. If your platform needs some other information for bus or delay functions, you can pass the pointer and use it in bus or delay functions via ``mpu925x.master_specific.bus_handle``.

STM32 HAL example:

.. code-block:: c

	I2C_HandleTypeDef hi2c1;

	mpu925x.master_specific.bus_handle = &hi2c1;

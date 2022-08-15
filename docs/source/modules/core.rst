.. _core-module:

Core Module
===========

Core module is capable of:

* Initialize sensor/driver
* Read raw/converted sensor data

Initialization
--------------

Initialization steps are following:

1. Define needed functions and data structures (See: :ref:`porting guide<porting-guide>`).
2. Create `mpu925x_t` struct.
3. Pass defined function and data structure pointers to the ``mpu925x_t`` struct.
4. Call ``mpu925x_init()`` function with created ``mpu925x_t`` struct and AD0 pin value as parameters and save it's return value.
5. If return value is not 0, go to step 4.

.. doxygenfunction:: mpu925x_init
	:project: mpu925x-driver

.. code-block:: c
	:caption: Example Code

	#include "mpu925x.h"

	// Create mpu925x_t struct instance.
	mpu925x_t mpu925x = {
		.master_specific = {
			// Bus handle.
			.bus_handle = &my_bus_handle,

			// Bus functions.
			.bus_read = my_bus_read_function_pointer,
			.bus_write = my_bus_write_function_pointer,
			.delay_ms = my_delay_function_pointer
		},

		.settings = {
			.general = {
				// Sensor's real life orientation.
				.orientation = mpu925x_z_minus
			}
		}
	};

	// Wait till' initializition is complete. Will be in endless loop if sensor
	// is unreachable (wiring is not correct, sensor is damaged...).
	while (mpu925x_init(&mpu925x, 0));

Read Sensor Data
----------------

If any of the sensor read functions called, they will store read data in the given ``mpu925x_t`` struct pointer. In this way, every sensor data can be read with just one function.

All of the read function name starts with ``mpu925x_get_`` prefix. Also names include magnitude, not the sensor name (e.g. ``mpu925x_get_acceleration``, not ``mpu925x_get_accelerometer``). Finally there is an option to read just the raw data and those functions include ``_raw`` suffix (e.g. ``mpu925x_get_acceleration_raw``). Every read function calls it's corresponding raw function internally.

Read sensor data will be in ``mpu925x_t.sensor_data`` struct (e.g. ``mpu925x_t.sensor_data.acceleration``).

.. code-block:: c
	:caption: Example Code

	// Insert initialization code here.

	// Get acceleration, angular rotation, magnetic field and temperature seperately.
	mpu925x_get_acceleration(&mpu925x);
	mpu925x_get_rotation(&mpu925x);
	mpu925x_get_magnetic_field(&mpu925x);
	mpu925x_get_temperature(&mpu925x);

	// Use calculated values.
	float acceleration = mpu925x.sensor_data.acceleration;
	int16_t acceleration_raw = mpu925x.sensor_data.acceleration_raw;
	// ...

	// Read only raw data.
	mpu925x_get_acceleration_raw(&mpu925x);
	mpu925x_get_rotation_raw(&mpu925x);
	mpu925x_get_magnetic_field_raw(&mpu925x);
	mpu925x_get_temperature_raw(&mpu925x);

	// Use read data.
	float old_acceleration = mpu925x.sensor_data.acceleration; // This data will be old because it is not calculated after read operation.
	int16_t acceleration_raw = mpu925x.sensor_data.acceleration_raw;
	// ...

	// Or just call the get all function.
	mpu925x_get_all(&mpu925x); // Every sensor data is now read and calculated to it's unit.

	// Use sensor data like before...

API Reference
-------------

.. doxygenfile:: mpu925x_core.c
	:project: mpu925x-driver

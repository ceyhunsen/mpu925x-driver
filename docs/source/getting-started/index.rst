.. _getting-started:

Getting Started
===============

Adding to Your Project
^^^^^^^^^^^^^^^^^^^^^^

1. Copy ``mpu925x-driver`` directory to your project's ``drivers`` directory.
2. Add ``inc`` directory to your toolchain's include path.
3. Add ``src/mpu925x_core.c``, ``src/mpu925x_settings.c`` and ``src/mpu925x_internals.c`` source files to your project's build toolchain. 
4. Provide bus handle, bus read, bus write and delay functions depending on your platform (see: :ref:`porting guide<porting-guide>`).
5. Include ``mpu925x.h`` header to your desired source files.

Simple Usage
^^^^^^^^^^^^

.. code-block:: c
	:caption: Example Code

	#include "mpu925x.h"

	// Create mpu925x_t struct instance.
	mpu925x_t mpu925x = {
		.master_specific = {
			.bus_handle = &my_bus_handle,

			// Bus functions
			.bus_read = my_bus_read_function_pointer,
			.bus_write = my_bus_write_function_pointer,
			.delay_ms = my_delay_function_pointer
		},

		.settings = {
			// Other settings
			.accelerometer_scale = mpu925x_2g,
			.gyroscope_scale = mpu925x_250dps,
			.orientation = mpu925x_z_plus
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

See :ref:`advanced usage<advanced-usage>` for more functionalities (e.g. hardware offset cancellation, sensor ranges...).

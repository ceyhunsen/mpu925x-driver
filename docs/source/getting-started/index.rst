.. _getting-started:

Getting Started
===============

Modules
^^^^^^^

This driver is separeted into different and independent modules. Each module includes new functionalities to the driver. These modules are:

* Core [`mpu925x_core.c`]
* Settings [`mpu925x_settings.c`]
* Accelerometer [`mpu925x_accelerometer.c`]
* Gyroscope [`mpu925x_gyroscope.c`]
* Magnetometer [`mpu925x_magnetometer.c`]

Also there are some extra modules:

* Simple AHRS [`mpu925x_simple_ahrs.c`]

Each module must be compiled with the target program to be used. See: :ref:`modules section<modules>` for more information.

Adding to Your Project
^^^^^^^^^^^^^^^^^^^^^^

1. Copy ``mpu925x-driver`` directory to your project's ``drivers`` directory or any other directory.
2. Add ``inc`` directory to your toolchain's include path.
3. Add ``src/mpu925x_core.c`` source file to your project's build toolchain.
4. Include ``mpu925x.h`` header to your desired source files.
5. Provide bus handle, bus read, bus write and delay functions depending on your platform (see: :ref:`porting guide<porting-guide>`).
6. [EXTRAS] If any of the modules other than the core are used, add it's corresponding source files to the project's build toolchain and include path.

Simple Usage
^^^^^^^^^^^^

This example code shows core module's initialization and sensor read functionalities.

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
				.orientation = mpu925x_z_minus
			}
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

See :ref:`modules section<modules>` for other functionalities.

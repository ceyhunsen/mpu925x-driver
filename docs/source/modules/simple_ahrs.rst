.. _simple-ahrs-module:

Simple AHRS
===========

Simple attitude and heading reference system module can return pitch and roll angles using acceleration value. Include ``mpu925x_simple_ahrs.h`` in desired source file and compile ``mpu925x_simple_ahrs.c`` source file with target program.

.. code-block:: c
	:caption: Example Code

	#include "mpu925x.h"
	#include "mpu925x_simple_ahrs.h"

	// Create struct instances.
	mpu925x_t mpu925x;
	mpu925x_simple_ahrs ahrs;

	// Initialize driver.
	mpu925x_init(&mpu925x, 0);

	// Get values.
	mpu925x_get_acceleration(&mpu925x);
	mpu925x_get_simple_ahrs(&mpu925x, &ahrs);

	printf("Pitch: %f, Roll: %f\n", ahrs.pitch, ahrs.roll);

API Reference
^^^^^^^^^^^^^

	.. doxygenfile:: mpu925x_simple_ahrs.h
	:project: mpu925x-driver

.. _accelerometer-module:

Accelerometer Module
====================

Full-Scale Select
-----------------

Accelerometer scale can be selected with this function and enum combination:

.. doxygenfunction:: mpu925x_accelerometer_set_scale
	:project: mpu925x-driver

.. doxygenenum:: mpu925x_accelerometer_scale
	:project: mpu925x-driver

Digital Low Pass Filter
-----------------------

Accelerometer digital low pass filter can be activated with this function and enum combination:

.. doxygenfunction:: mpu925x_accelerometer_set_dlpf
	:project: mpu925x-driver

.. doxygenenum:: mpu925x_accelerometer_dlpf
	:project: mpu925x-driver

.. csv-table:: Accelerometer DLPF Data Rates and Bandwiths (Normal Mode)
	:header: "Bandwith (Hz)", "Delay (ms)", "Noise Density (ug/rtHz)", "Rate (kHz)"

	1.13 K, 0.75, 250, 4
	460, 1.94, 250, 4
	184, 5.80, 250, 4
	92, 7.80, 250, 4
	41, 11.80, 250, 4
	20, 19.80, 250, 4
	10, 35.70, 250, 4
	5, 66.96, 250, 4

Offset Cancellation
-------------------

Offset cancellation consist 2 steps: Getting and setting offset cancellation values. While getting offset cancellation values, sensor must stand still. Also orientation of sensor is very important. Set orientation of sensor with ``orientation`` variable in ``mpu925x_t.sensor_settings`` struct before offset cancellation.

Also there is 2 available options for offset cancellation: manually get and set or let the unified function do it. Unified function calls get and set functions internally. So both methods result the same thing. But if unified function is called, offset values can't be accessed by user. If offset values wanted to stored somewhere else, offset cancellation should be done manually.

.. doxygenfunction:: mpu925x_accelerometer_get_offset
	:project: mpu925x-driver

.. doxygenfunction:: mpu925x_accelerometer_set_offset
	:project: mpu925x-driver

.. doxygenfunction:: mpu925x_accelerometer_offset_cancellation
	:project: mpu925x-driver

.. doxygenenum:: mpu925x_orientation
	:project: mpu925x-driver

.. code-block:: c
	:caption: Example Code

	mpu925x.settings.orientation = mpu925x_y_plus; // Depends on how sensor is mounted.
	// Sensor must stand still while offset cancellation.
	mpu925x_accelerometer_offset_cancellation(&mpu925x, 200);

API Reference
-------------

.. doxygenfile:: mpu925x_accelerometer.c
	:project: mpu925x-driver

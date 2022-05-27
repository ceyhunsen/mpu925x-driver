.. _accelerometer:

Accelerometer Advanced Usage
============================

Raw And Translated Data
^^^^^^^^^^^^^^^^^^^^^^^

Raw and translated acceleration datas are available. Calling ``mpu925x_get_acceleration`` function also calls ``mpu925x_get_acceleration_raw`` function internally. Both functions stores data in given ``mpu925x_t`` struct.

.. doxygenfunction:: mpu925x_get_acceleration
	:project: mpu925x-driver

.. doxygenfunction:: mpu925x_get_acceleration_raw
	:project: mpu925x-driver

Full-Scale Select
^^^^^^^^^^^^^^^^^

One can set accelerometer full-scale range of 2g, 4g, 8g and 16g.

.. doxygenfunction:: mpu925x_set_accelerometer_scale
	:project: mpu925x-driver

.. doxygenenum:: mpu925x_accelerometer_scale
	:project: mpu925x-driver

Digital Low Pass Filter
^^^^^^^^^^^^^^^^^^^^^^^

One can enable hardware digital low pass filter. Look datasheet for more info.

.. doxygenfunction:: mpu925x_set_accelerometer_dlpf
	:project: mpu925x-driver

Offset Cancellation
^^^^^^^^^^^^^^^^^^^

Offset cancellation consist 2 steps: Getting and setting offset cancellation values. While getting offset cancellation values, sensor must stand still. Also orientation of sensor is very important. Set orientation of sensor with ``.sensor_settings.orientation`` variable in ``mpu925x_t`` struct.

.. doxygenenum:: mpu925x_orientation
	:project: mpu925x-driver

.. doxygenfunction:: mpu925x_get_accelerometer_offset
	:project: mpu925x-driver

.. doxygenfunction:: mpu925x_set_accelerometer_offset
	:project: mpu925x-driver

One can use these two methods to get and set offset values or call one unified function which does 2 of them at once.

.. doxygenfunction:: mpu925x_accelerometer_offset_cancellation
	:project: mpu925x-driver

.. code-block:: c
	:caption: Example Code

	mpu925x.settings.orientation = mpu925x_y_plus; // Depends on how sensor is mounted.
	// Sensor must stand still while offset cancellation.
	mpu925x_accelerometer_offset_cancellation(&mpu925x, 200);

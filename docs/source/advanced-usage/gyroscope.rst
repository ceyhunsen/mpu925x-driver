.. _gyroscope:

Gyroscope Advanced Usage
========================

Raw And Translated Data
^^^^^^^^^^^^^^^^^^^^^^^

Raw and translated rotation datas are available. Calling ``mpu925x_get_rotation`` function also calls ``mpu925x_get_rotation_raw`` function internally. Both functions stores data in given ``mpu925x_t`` struct.

.. doxygenfunction:: mpu925x_get_rotation
	:project: mpu925x-driver

.. doxygenfunction:: mpu925x_get_rotation_raw
	:project: mpu925x-driver

Full-Scale Select
^^^^^^^^^^^^^^^^^

One can set gyroscope full-scale range of 250 dps, 500 dps, 1000 dps and 2000 dps.

.. doxygenfunction:: mpu925x_set_gyroscope_scale
	:project: mpu925x-driver

.. doxygenenum:: mpu925x_gyroscope_scale
	:project: mpu925x-driver

Digital Low Pass Filter
^^^^^^^^^^^^^^^^^^^^^^^

One can enable hardware digital low pass filter. Look datasheet for more info.

.. doxygenfunction:: mpu925x_set_gyroscope_dlpf
	:project: mpu925x-driver

Offset Cancellation
^^^^^^^^^^^^^^^^^^^

Offset cancellation consist 2 steps: Getting and setting offset cancellation values. While getting offset cancellation values, sensor must stand still.

.. doxygenfunction:: mpu925x_get_gyroscope_offset
	:project: mpu925x-driver

.. doxygenfunction:: mpu925x_set_gyroscope_offset
	:project: mpu925x-driver

One can use these two methods to get and set offset values or call one unified function which does 2 of them at once.

.. doxygenfunction:: mpu925x_gyroscope_offset_cancellation
	:project: mpu925x-driver

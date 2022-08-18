.. _magnetometer-module:

Magnetometer Module
===================

Measurement Mode
----------------

Operation mode is configurable. See AK8963's datasheet for more info.

.. warning::

	Other than ``continuous_measurement_mode_1`` and ``continuous_measurement_mode_2``, operating modes are not tested. Wait for full version of this driver to use them properly.

.. doxygenfunction:: mpu925x_magnetometer_set_measurement_mode
	:project: mpu925x-driver

.. doxygenenum:: mpu925x_magnetometer_measurement_mode
	:project: mpu925x-driver

Bit Mode
--------

Bit mode is configurable. See AK8963's datasheet for more info.

.. doxygenfunction:: mpu925x_magnetometer_set_bit_mode
	:project: mpu925x-driver

.. doxygenenum:: mpu925x_magnetometer_bit_mode
	:project: mpu925x-driver

API Reference
-------------

.. doxygenfile:: mpu925x_magnetometer.c
	:project: mpu925x-driver

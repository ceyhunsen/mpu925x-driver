.. _general-settings:

General Settings
================

Sample Rate Divider
^^^^^^^^^^^^^^^^^^^

Internal sample rate divider is configurable. See MPU-9250 or MPU-9255's datasheet for more info.

.. doxygenfunction:: mpu925x_set_sample_rate_divider
	:project: mpu925x-driver

Clock Source
^^^^^^^^^^^^

Internal clock source is configurable.

.. doxygenfunction:: mpu925x_set_clock_source
	:project: mpu925x-driver

.. doxygenenum:: mpu925x_clock
	:project: mpu925x-driver

.. _magnetometer:

Magnetometer Advanced Usage
===========================

Raw And Translated Data
^^^^^^^^^^^^^^^^^^^^^^^

Raw and translated magnetic field datas are available. Calling ``mpu925x_get_magnetic_field`` function also calls ``mpu925x_get_magnetic_field_raw`` function internally. Both functions stores data in given ``mpu925x_t`` struct.

.. doxygenfunction:: mpu925x_get_magnetic_field
	:project: mpu925x-driver

.. doxygenfunction:: mpu925x_get_magnetic_field_raw
	:project: mpu925x-driver

Measurement Mode
^^^^^^^^^^^^^^^^

Operation mode is configurable. See AK8963's datasheet for more info.

.. warning::

	Other than ``continuous_measurement_mode_1`` and ``continuous_measurement_mode_2``, operating modes are not tested. Wait for full version of this driver to use them properly.

.. doxygenfunction:: mpu925x_set_magnetometer_measurement_mode
	:project: mpu925x-driver

.. doxygenenum:: mpu925x_magnetometer_measurement_mode
	:project: mpu925x-driver

Bit Mode
^^^^^^^^

Bit mode is configurable. See AK8963's datasheet for more info.

.. doxygenfunction:: mpu925x_set_magnetometer_bit_mode
	:project: mpu925x-driver

.. doxygenenum:: mpu925x_magnetometer_bit_mode
	:project: mpu925x-driver

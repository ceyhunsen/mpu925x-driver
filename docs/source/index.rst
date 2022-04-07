.. _introduction:

Introduction
============

Welcome to the documentation for MPU-925X driver. This driver supports MPU-9250 and MPU-9255 sensors.

Source code is available at `Github <https://github.com/ceyhunsen/mpu925x-driver>`_.

Units
^^^^^

* Acceleration from accelerometer: G
* Rotation from gyroscope: Degrees per second
* Magnetic field from magnetometer: Gauss
* Temperature from thermometer: C degree

Compatibility
^^^^^^^^^^^^^

This driver is designed to be portable. So, it can be used on any platform that has a C compiler which supports ``C99`` standart. See :ref:`porting guide<porting-guide>` and ``examples`` directory for more information.

License
^^^^^^^

This project is licensed under MIT license.

.. literalinclude:: ../../LICENSE

.. toctree::
	:maxdepth: 3
	:caption: Contents:

	self
	getting-started/index
	porting-guide/index
	advanced-usage/index
	api-reference/index

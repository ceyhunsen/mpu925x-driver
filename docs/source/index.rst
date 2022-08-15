.. _introduction:

Introduction
============

Welcome to the documentation of platform independent device driver for MPU-9250 and MPU-9255 sensors.

Main features of the driver are:

* Written in `C` with `C99` standard
* Platform independent bus interface
* Separate and independent compilation units for configurable binary size
* Read raw/converted sensor data
* Configure sensor/driver

Units
^^^^^

* Acceleration from accelerometer: G
* Angular rotation from gyroscope: Degrees per second
* Magnetic field from magnetometer: Micro Gauss
* Temperature from thermometer: Celsius degree

Compatibility
^^^^^^^^^^^^^

This driver is designed to be portable. So, it can be used on any platform that has a `C` compiler which supports ``C99`` standard. See :ref:`porting guide<porting-guide>` and ``examples`` directory for more information.

.. toctree::
	:maxdepth: 3
	:caption: Contents:

	self
	getting-started/index
	modules/index
	porting-guide/index
	.. advanced-usage/index
	api-reference/index

License
=======

This project is licensed under ``GNU Lesser General Public License v3.0``.

.. literalinclude:: ../../COPYING.LESSER
	:language: text

.. _initialization:

Initialization
==============

Initialization steps are following:

1. Define needed functions and pass their pointers (See: :ref:`porting guide<porting-guide>`).
2. Create ``mpu925x_t`` struct instance with initial values (See: :ref:`accelerometer<accelerometer>`, :ref:`gyroscope<gyroscope>` and :ref:`magnetometer<magnetometer>`).
3. Call mpu925x_init() function with created ``mpu925x_t`` struct and AD0 pin values.

mpu925x_init() Function
^^^^^^^^^^^^^^^^^^^^^^^

Init function takes ``mpu925x_t`` struct and AD0 pin values as parameters. AD0 pin value depends on physical sensor and is most probably 0. See :ref:`api reference<api-reference>` for more info.

Init function will return 0 on success, 1 on accelerometer and gyroscope fail and 2 on magnetometer fail. One can use accelerometer and gyroscope with return value of 2. But with return value of 1, nothing works, so check wiring and sensor damage.

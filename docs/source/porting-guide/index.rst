.. _porting-guide:

Porting Guide
=============

This driver needs:

* Bus read
* Bus write
* Delay microseconds
* Bus handle (optional, depending on your platform)

function pointers. You must define your own functions and pass their pointers to ``mpu925x_t`` struct before initialization.

Function Prototypes And Parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Bus Read Function
"""""""""""""""""

Bus read functions' prototype is like this:

.. code-block:: c

	uint8_t (*bus_read)(struct mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size);

This function wants to read ``size`` byte amount of data from bus slave's ``reg`` register which address is ``slave_address`` and stores it in ``buffer`` array.

Bus Write Function
""""""""""""""""""

Bus write function's prototype is like this:

.. code-block:: c

	uint8_t (*bus_write)(struct mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size);

This function wants to write ``size`` byte amount of data from ``buffer`` array to bus slave's ``reg`` register which address is ``slave_address``.

Delay Microseconds Function
"""""""""""""""""""""""""""

Delay microseconds functions prototype is like this:

.. code-block:: c

	void (*delay_ms)(struct mpu925x_t *mpu925x, uint32_t delay);

This functions wants to wait at least ``delay`` microseconds.

Bus Handle
""""""""""

Bus handle is for platform specific structs or other types of data. If your platform needs some other information for bus or delay functions, you can pass the pointer and use it in bus or delay functions via ``mpu925x.master_specific.bus_handle``.

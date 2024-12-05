.. _bt_a2dp_source:

Bluetooth: A2DP
####################

Overview
********

Application demonstrating usage of the A2dp Profile APIs.

This sample can be found under :zephyr_file:`samples/bluetooth/a2dp_source` in
the Zephyr tree.

Check :ref:`bluetooth samples section <bluetooth-samples>` for details.

Requirements
************

* A board with Bluetooth BR/EDR (Classic) support

Building and Running
********************

When building targeting mimxrt1170_evk@B board with the murata 2EL Controller.

Building for an mimxrt1170_evk@B + murata 2EL
-------------------------------------------

.. zephyr-app-commands::
   :zephyr-app: samples/bluetooth/a2dp_source/
   :board: mimxrt1170_evk@B/mimxrt1176/cm7
   :goals: build

For example: west build -b mimxrt1170_evk@B/mimxrt1176/cm7 .\samples\bluetooth\a2dp_source

1. input `bt discover` to discover BT devices.
2. input `bt connect <index>` to create the first connection with the selected BT device, the <index> is the result of step 1.
3. input `bt connect <index>` to create the second connection with the selected BT device, the <index> is the result of step 1.
4. input `bt play` to play prepared audio to the connected devices.

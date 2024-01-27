# IRLib Library <!-- omit in toc -->

- [IRLib](#irlib)
  - [Overview](#overview)
    - [Code Conventional](#code-conventional)
  - [Macros](#macros)
  - [Functions](#functions)
  - [Structures, States, and Flags](#structures-states-and-flags)
    - [Structures, Unions](#structures-unions)
    - [States](#states)
    - [Flags](#flags)
- [ReadWriteLib](#readwritelib)
  - [Overview](#overview-1)
    - [Code Conventional:](#code-conventional-1)
  - [Macros](#macros-1)
- [config](#config)

# IRLib

## Overview

The `IRLib` is a library designed for detecting and decoding [**NEC2 protocol frames**](https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol) based on logic change interrupts in a pin of ATmega328P. It is part of the final project for the System Embedded course at Hanoi University of Science and Technology (HUST), semester 20231.

### Code Conventional
The following prefix suffix is used for easy coding and reading: 
- `_H`: .h library
- `_S`: state for FSM
- `_FLG`: flag
- `_t`: defined struct by NPLinh
- `_cb`: callback function

## Macros

| Macro                  | Description                                                                      |
|------------------------|----------------------------------------------------------------------------------|
| `NEC_BURST`            | Width of a NEC burst in microsecond, which is 562.5 microseconds.                |
| `NEC_ADDRESS_LEN`      | Number of bits in the address field of a NEC2 frame.                             |
| `NEC_COMMAND_LEN`      | Number of bits in the command field of a NEC2 frame.                             |
| `NEC_DATA_LEN`         | Total number of bits in a NEC2 frame.                                            |
| `NEC_START_PULSE`      | Duration of the start pulse in a NEC2 frame (nearly 9000 microseconds).          |
| `NEC_START_SPACE`      | Duration of the start space in a NEC2 frame (nearly 4500 microseconds).          |
| `NEC_BIT_PULSE`        | Duration of a single bit pulse in a NEC2 frame.                                  |
| `NEC_ONE_SPACE`        | Duration of the space for logical 1 in a NEC2 frame (nearly 1690 microseconds).  |
| `NEC_ZERO_SPACE`       | Duration of the space for logical 0 in a NEC2 frame.                             |
| `NEC_REPEAT_SPACE`     | Duration of the space for a repeated frame (nearly 2250 microseconds).           |
| `NEC_REPEAT_PULSE`     | Duration of the pulse for a repeated frame (nearly 9000 microseconds).           |
| `NEC_REPEAT_INTERVAL`  | Interval between repeated frames (110000 microseconds).                          |
| `NEC_MAX_REPEAT_SPACE` | Maximum space duration for a repeated frame.                                     |
| `IR_RECEIVER_PIN`      | (defined in config file) Pin used for receiving IR signal (default = 2)          |
| `USE_USER_ISR_IR_RECEIVER_CB` | (defined in config file) if defined, enable user's callback function      |

## Functions

| Function             | Description                                               | Parameters  | Return Value                            |
|----------------------|-----------------------------------------------------------|-------------|-----------------------------------------|
| `ir_recv_init`       | Initialize the IR receiver.                               | None        | `true` if successful, `false` otherwise.|
| `isr_enable`         | Enable the interrupt service routine (ISR).               | None        | None                                    |
| `isr_disable`        | Disable the interrupt service routine (ISR).              | None        | None                                    |
| `user_ir_isr_handle` | User-defined ISR callback when decode successful a frame. | None        | None                                    |
| `ir_isr_handler_cb`  | Callback function by NPLink to store data, decode and assign flag whenever a change in receive pin occurs (DO NOT modify it). | None | None |


## Structures, States, and Flags

### Structures, Unions

| Name                     | Description                                                |
|--------------------------|------------------------------------------------------------|
| `ir_data_t`              | Union representing the structure of the received IR data (32 bits).  |
| `ir_receiver_t`          | Struct representing the current status, bit received of from the IR receiver (is updated during decode phase and used in FSM for decoding). |
| `ir_receiver_data_cb_t`  | Struct storing the decoded data. It will be updated right after decoding successfully a frame and before calling the user callback to transfer received data to the main loop for further processing. |

### States

| Name                        | Description                                      |
|-----------------------------|--------------------------------------------------|
| `RECEIVER_IDLE_S`           | IDLE state, wait for the start pulse              |
| `RECEIVER_WAIT_START_SPACE_S`| SYNC state, wait for the start space             |
| `RECEIVER_WAIT_FIRST_DATA_PULSE_S`| Wait for the first data pulse          |
| `RECEIVER_WAIT_DATA_SPACE_S`| Wait for the data space                          |
| `RECEIVER_WAIT_DATA_PULSE_S`| Wait for the data pulse                          |

### Flags

| Name                     | Description                                        |
|--------------------------|----------------------------------------------------|
| `IR_EMPTY_FLG`           | Empty flag                                         |
| `IR_NEW_CODE_FLG`        | New code flag                                      |
| `IR_REPEAT_CODE_FLG`     | Repeat code flag                                   |
| `IR_PARITY_ERROR_FLG`    | Parity error flag                                  |



# ReadWriteLib

## Overview

The `ReadWriteLib` is a library designed for fast read and write operations on a digital pin of ATmega328P using a bare-metal approach. It is part of the final project for the System Embedded course at Hanoi University of Science and Technology (HUST), semester 20231.

### Code Conventional:
The following prefix suffix is used for easy coding and reading: 
- `_H`: .h library


## Macros

| Macro                  | Description                                                                      |
|------------------------|----------------------------------------------------------------------------------|
| `MASK(bit)`            | Create a bitmask with the specified bit set to 1.                                |
| `SET(reg, bit)`        | Set the specified bit in the register to 1.                                      |
| `CLEAR(reg, bit)`      | Clear the specified bit in the register (set to 0).                               |
| `READ(reg, bit)`       | Read the value of the specified bit in the register.                              |
| `WRITE(reg, bit, value)`| Write the specified value to the specified bit in the register.                  |
| `PinToPort(pin)`       | Convert pin number to the corresponding PORT register.                            |
| `PinToDDR(pin)`        | Convert pin number to the corresponding DDR (Data Direction Register) register. |
| `PinToPIN(pin)`        | Convert pin number to the corresponding PIN register.                            |
| `PinToBit(pin)`        | Convert pin number to the bit index in the register.                              |
| `ReadPin(pinNum)`      | Read the digital value of a pin.                                                 |
| `WritePin(pinNum, value)`| Write a digital value to a pin.                                                 |

# config
The `config.h` file store global config of the project such as pin number used to receive demodulated IR signal; using user's callback or not.

---
*By Linh Nguyen Phuong (deLini), December 2023*

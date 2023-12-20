# Infrared Decoder Project (YT-150 Decoder)

## Overview

This directory contains the source code for the Embedded Systems major project. The directory structure includes two main folders: `lib` and `src`. 

**lib** Directory:
- Provides libraries developed by me (**deLini1612**) for detecting, reading, and decoding infrared signals using the **NEC** protocol, utilizing interrupts (without the need for a timer).
- Designed specifically to work with the ATMega328P microcontroller.

**src** Directory:
- Houses the main code file for the microcontroller, utilizing libraries from the `lib` directory and other relevant libraries.
- Focuses on decoding signals from the YT-150 projector remote used in classrooms and displaying the status on an LCD, meeting the project requirements set by the instructor.


To build and flash the code for the Arduino Uno R3, I use **Platform IO**. Alternatively, you can use other IDEs such as Arduino IDE.

## Table of Contents

- [Infrared Decoder Project (YT-150 Decoder)](#infrared-decoder-project-yt-150-decoder)
  - [Overview](#overview)
  - [Table of Contents](#table-of-contents)
  - [Directory Structure](#directory-structure)
  - [Usage](#usage)
    - [Using IRLib Library in Your Own Source Code](#using-irlib-library-in-your-own-source-code)
    - [Installing and Running the Source Code](#installing-and-running-the-source-code)
  - [Decoding Commands](#decoding-commands)

## Directory Structure
```
IR Projector Decoder
│
├── lib
│   ├── IRLib
│   │   └── ... (IRLib library for NEC IR signal detection and decoding)
│   │
│   ├── ReadWriteLib
│   │   └── ... (ReadWriteLib library for digital pin read/write using bare metal approach)
│   │
│   ├── config
│   │   └── ... (User's code configurations, macros, and pin definitions)
│   │
│   └── ... (other necessary libraries)
│
└── src
    └── main.c
``````
## Usage
This section will guide you through the steps to run my program and use IRLib to build your own program. Details regarding the macros, functions, and structures used in IRLib will be provided specifically within [IRLib documentation](lib/IRLib/README.md).


### Using IRLib Library in Your Own Source Code

1. Integrate the IRLib library into your project and define all macros, pins that you want to use.
2. Utilize the library to detect and decode raw NEC IR signals in your source code.
3. Experiment by pressing all buttons on your remote to identify the decoding commands for your specific remote. Then you can code your own programe to handle with each button.

### Installing and Running the Source Code

1. Clone this repository to your local machine.
2. Open the project in your preferred IDE (PlatformIO recommended).
3. Ensure your hardware setup is correctly connected. If you want to change the pin used for IR Receiver or the LED pin, modify macro in [config file](lib/config/config.h)
4. Build and flash the code to your ATMega328P MCU.

## Decoding Commands

The following table lists the decoding commands for the Casio YT-150 projector remote:

| Command (DEC) | Command (HEX) | Button       |
| ------------- | ------------- | ------------ |
| 10            | 0x0A          | INPUT        |
| 11            | 0x0B          | POWER        |
| 12            | 0x0C          | MENU         |
| 14            | 0x0E          | ESC          |
| 42            | 0x2A          | KEYSTONE +   |
| 43            | 0x2B          | D-ZOOM +     |
| 44            | 0x2C          | VOLUME +     |
| 45            | 0x2D          | KEYSTONE -   |
| 46            | 0x2E          | D-ZOOM -     |
| 47            | 0x2F          | VOLUME -     |
| 58            | 0x3A          | BLANK        |
| 59            | 0x3B          | FREEZE       |
| 60            | 0x3C          | ECO          |
| 74            | 0x4A          | UP           |
| 75            | 0x4B          | DOWN         |
| 76            | 0x4C          | ENTER        |
| 77            | 0x4D          | LEFT         |
| 78            | 0x4E          | RIGHT        |
| 90            | 0x5A          | TIMER        |
| 91            | 0x5B          | AUTO         |
| 92            | 0x5C          | ASPECT       |
| 93            | 0x5D          | FUNC         |

*By Linh Nguyen Phuong (deLini), December 2023*
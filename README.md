# YT-150 Remote IR Decoder - Embedded System Project <!-- omit in toc -->

Welcome to this repository - a repository for the final project of the Embedded System Design at Hanoi University of Science and Technology (HUST), semester 20231.

---

- [Project Overview](#project-overview)
  - [Project High Level Requirements](#project-high-level-requirements)
  - [Project Scope](#project-scope)
- [Repository Contents](#repository-contents)
- [Resource Usage](#resource-usage)
- [Functionality](#functionality)
  - [Simulated Button](#simulated-button)
  - [Functions Operations](#functions-operations)
- [Getting Started](#getting-started)


## Project Overview

This repository is dedicated to simulating, coding, and deploying the final project assigned in the Embedded System Design course at HUST. 

The primary objective of this project is to design and implement a module for decoding remote control signals from a classroom projector (Casio YT-150).

### Project High Level Requirements

The task involves decoding messages (corresponding to at least 4 commands) received from an infrared photodiode when controlled by the available remote control unit of the classroom projector (Casio YT-150). 

The implementation should use an 8-bit microcontroller and be programmed in C or its equivalent, without relying on any external libraries.

### Project Scope

It's essential to note that this project ***DOES NOT*** not involve the physical manipulation of a ***real*** projector's components. The emphasis is on the embedded system's capability to interpret and respond to remote control signals, demonstrating a proof of concept rather than a fully functional physical projector.

While the simulation doesn't modify physical projector parameters, it showcases the responsiveness of the embedded system to infrared signals. For instance, when the user presses the 'Decrease Brightness' button on the remote control, the LCD will reflect the corresponding decrease in brightness value on the screen, even though the actual projector's brightness remains unaffected. You can find list of functionalities we simulate [here](#functionality).

## Repository Contents

The repository includes the following components:

- **Source Code**: The detailed source code with comprehensive Readme can be found in [this folder](./IR%20Projector%20Decoder).
- **Report**: A comprehensive report detailing the project as well as a slide deck for the final project presentation.
- **Docs**: Containing others documentations (such as MCU's datasheet) that we use.
- **Demo Video**: A video to demo the final product can be found [here](./Media/Demo.mp4).


## Resource Usage

This project utilizes the following resources:

- **Microcontroller Model**: Arduino UNO R3 with the ATMega328P chip.
- **Programming Language**: C.
- **Infrared Signal Capture**: IC 1838T.
- **Display Unit**: LCD 16x2 with an I2C IC.

## Functionality

### Simulated Button 

| Button | Brief Description                                  |
| --------------- | -------------------------------------------------- |
| POWER           | Turn on/off the projector.  |
| VOLUME +        | Increase volume (1 step/press or continuous hold). |
| VOLUME -        | Decrease volume (1 step/press or continuous hold). |
| FREEZE          | Switch projector to FREEZE mode. |
| BLANK           | Switch projector to BLANK mode. |
| ESC             | Escape from a screen/mode/function to the main screen.|
| ECO             | Switch to ECO SCREEN, used to adjust energy-saving settings. |
| LEFT            | Left button of the remote, used to change energy-saving status or decrease energy-saving level. |
| RIGHT           | Right button of the remote, used to Change energy-saving status or increase energy-saving level. |
| DOWN            | Down button of the remote, used to select functions in a screen (e.g., adjusting energy-saving level in ECO SCREEN). |

### Functions Operations
1. Power ON/OFF: POWER button
   - Turn on/off the projector.
   - When on, LCD lights up and displays content.
   - When off, LCD screen goes dark and ***DO NOT*** update any status (change volume, change mode,...)
2. Adjust volume: VOLUME+ and VOLUME- buttons
   - Increase/decrease volume (1 step/press or continuous hold).
   - Realtime volume value displayed on the LCD.
3. Change mode: FREEZE, BLANK, and ESC buttons
   - By default, when turned on, the projector is in NORMAL mode.
   - Switch projector to FREEZE or BLANK mode using corresponding button.
   - Display mode on LCD.
   - Press ESC or button matching current mode to return to NORMAL mode.
4. Energy-saving: ECO, LEFt, RIGHT, DOWN, and ESC buttons
   - Press the ECO button in MAIN SCREEN will switch to ECO SCREEN.
   - Display ON/OFF status of eco mode and current saving level (if eco mode is on).
   - Use LEFT/RIGHT button to adjust eco status.
   - To adjust eco level, press DOWN button then using LEFT/RIGHT button to adjust eco level.
   - In ECO SCREEN, unable to switch modes or adjust volume.
   - Press ESC to return to the MAIN SCREEN.
  
Note: Simulations are performed on the LCD to show the status and realtime values of the functions.

## Getting Started

To get started with the project, navigate to the [this folder](./IR%20Projector%20Decoder), where you will find the list of all command of YT-150 as well as the source code and related files. Make sure to follow the [guidelines](./IR%20Projector%20Decoder/README.md) mentioned in the project documentation.

Feel free to explore and contribute to the development of this Embedded System project! If you have any questions or suggestions, please reach out to the project maintainers.

Happy coding!

---
*By Linh Nguyen Phuong (deLini), December 2023*

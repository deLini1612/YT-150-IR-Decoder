# YT-150 Remote IR Decoder - Embedded System Project

Welcome to this repository - a repository for the final project of the Embedded System Design at Hanoi University of Science and Technology (HUST), semester 20231.

## Project Overview

This repository is dedicated to simulating, coding, and deploying the final project assigned in the Embedded System Design course at HUST. 

The primary objective of this project is to design and implement a module for decoding remote control signals from a classroom projector (Casio YT-150).

### Project High Level Requirements

The task involves decoding messages (corresponding to at least 4 commands) received from an infrared photodiode when controlled by the available remote control unit of the classroom projector (Casio YT-150). 

The implementation should use an 8-bit microcontroller and be programmed in C or its equivalent, without relying on any external libraries.

### Repository Contents

The repository includes the following components:

- **Source Code**: The detailed source code with comprehensive Readme can be found in [this folder](./IR%20Projector%20Decoder).
- **PCB Layout**: The layout of the printed circuit board (PCB) used in the project.
- **Report**: A comprehensive report detailing the project.
- **Presentation Slides**: Slide deck for the final project presentation.
- **Demo Video**: A video to demo the final product.


## Project Scope

It's essential to note that this project ***DOES NOT*** not involve the physical manipulation of a ***real*** projector's components. The emphasis is on the embedded system's capability to interpret and respond to remote control signals, demonstrating a proof of concept rather than a fully functional physical projector.

While the simulation doesn't modify physical projector parameters, it showcases the responsiveness of the embedded system to infrared signals. For instance, when the user presses the 'Decrease Brightness' button on the remote control, the LCD will reflect the corresponding decrease in brightness, even though the actual projector's brightness remains unaffected.

## Resource Usage

This project utilizes the following resources:

- **Microcontroller Model**: Arduino UNO R3 with the ATMega328P chip.
- **Programming Language**: C.
- **Infrared Signal Capture**: IC 1838T.
- **Display Unit**: LCD.

## Functionality

The product provides the following functionalities:

- **Power On/Off**: Control the power state of the projector.
- **Brightness Control**: Adjust (increase or decrease) the brightness level of the projector.
- **Escape Command**: ESC to back and escape the current configuration page (not save the adjust configuration)  

## Getting Started

To get started with the project, navigate to the [this folder](./IR%20Projector%20Decoder), where you will find the source code and related files. Make sure to follow the guidelines mentioned in the project documentation.

Feel free to explore and contribute to the development of this Embedded System project! If you have any questions or suggestions, please reach out to the project maintainers.

Happy coding!

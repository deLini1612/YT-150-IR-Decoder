/*
ReadWriteLib.h
Copyright (C) 2023 deLini1612 (Linh Nguyen Phuong)

- This is a part of final project of System Embedded course at HUST, semester 20231
- This lib is used to read/write to an digital pin of ATmega328P in a faster way than digitalRead function provied by Adruino
- This lib will use barenmetal approach

- I/O map: ATmega328P has 3 ports
    + Port B: Pins 8 to 13 (as PB0 to PB5)
    + Port C: Pins A0 to A5 (as PC0 to PC5)
    + Port D: Pins 0 to 7 (as PD0 to PD7)

- The are 3 main reg to control state of a I/O port:
    + PORT: Data Register               --> Writing data to the output pin or configure pull up of input pin (reset: tri-stated)
        * Input pin: 1 = ACtivate Pull Up, 0 = Disable Pull Up 
        * Output pin: 1 = Drive HIGH, 0 = Drive LOW
    + DDR: Data Direction Register      --> Configure direction of the pin
        * 1 = Output, 0 = Input
    + PIN: Input Pins Address           --> Reading input data of input pin or togglling PORT
*/

#ifndef _READ_WRITE_LIB_H
#define _READ_WRITE_LIB_H

//================ Include other libs ================
#include <Arduino.h>

//============= Check the microcontroller is supported or not =============
#ifndef __AVR_ATmega328P__
#   error "Unsupported microcontroller. This code is intended for ATmega328P."
#endif  //ends for check MCU

//===================== Bit Operator Macro =====================
#define MASK(bit)                   (1UL << (bit))
#define SET(reg, bit)               ((reg) |= MASK(bit))
#define CLEAR(reg, bit)             ((reg) &= ~MASK(bit))
#define READ(reg, bit)              (((reg) & MASK(bit)) >> (bit))
#define WRITE(reg, bit, value)      (value ? SET(reg, bit) : CLEAR(reg, bit))


//============ Convert pin number to coressponding reg ============
#define PinToPort(pin)          ((pin<8)?PORTD:((pin<14)?PORTB:PORTC))
#define PinToDDR(pin)           ((pin<8)?DDRD:((pin<14)?DDRB:DDRC))
#define PinToPIN(pin)           ((pin<8)?PIND:((pin<14)?PINB:PINC))
#define PinToBit(pin)           ((pin<8)?pin:((pin<14)?(pin-8):(pin-14)))   //Convert pin number to bit index in reg

//================ Operator with pin number ================

//Remember to use __builtin_constant_p to check whether pinNum and value are known to be a compile-time constant
#define ReadPin(pinNum) \
   (int) \
   ((__builtin_constant_p(pinNum))? \
        ((READ(PinToPIN(pinNum), PinToBit(pinNum)))?HIGH:LOW) \
        :digitalRead(pinNum))

#define WritePin(pinNum, value) \
 if (__builtin_constant_p(pinNum)) { \
    WRITE(PinToPort(pinNum), PinToBit(pinNum), value); \
 } else { \
    digitalWrite(pinNum, value);}

#endif  //ends for _READ_WRITE_LIB_H

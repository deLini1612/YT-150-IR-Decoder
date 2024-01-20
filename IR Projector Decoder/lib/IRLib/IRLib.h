/*
IRLib.h
Copyright (C) 2023 deLini1612 (Linh Nguyen Phuong)

- This is a part of final project of System Embedded course at HUST, semester 20231
- This lib is used to detect and decode a NEC protocol frame based on logic change interrupt in a pin of ATmaga328P.
- The decoded frame includes address, command and flag. It will be stored in ir_receive_data variable.
- Provide user isr callback by define USE_USER_ISR_IR_RECEIVER_CB in user's code user_ir_isr_handle()

- NEC Protocol:
    + Reference: https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol
    + 1 burst: 562.5µs
    + Logical 0 and 1:
        * Logical 1: 1000
        * Logical 0: 10 
    + Frame:
        * LBS is transmited first
        * 24 (16 + 8) start bursts + 8 bit address + 8 bit inverted address + 8 bit command + 8 bit inverted command + 1 stop burst.
    + Repeated Frame (Each 110ms):
        * Each 110ms --> Repeat all original frame (NEC2)

- Code conventional:
    + _H:       .h library
    + _S:       state for FSM
    + _FLG:     flag
    + _t:       NPLinh defined struct
    + _cb:      Callback function
*/

#ifndef _IR_LIB_H
#define _IR_LIB_H

//================ Include other libs ================
#include <Arduino.h>
#include <ReadWriteLib.h>
#include <config.h>

#ifdef __cplusplus
extern "C" {
#endif

//============= Check the microcontroller is supported or not =============
#ifndef __AVR_ATmega328P__
#   error "Unsupported microcontroller. This code is intended for ATmega328P."
#endif  //ends for check MCU

//============ Conditional compilation for IR input and output pins ============
#ifndef IR_RECEIVER_PIN
// #warning "Did not define IR received pin. Syntax: #define IR_RECEIVER_PIN xx // [xx is the pin you want to use]. Use default pin (pin 2)"
#error "Did not define IR received pin. Syntax: #define IR_RECEIVER_PIN xx // [xx is the pin you want to use]. Use default pin (pin 2)"
// #define IR_RECEIVER_PIN 2
#endif  //ends for define IR_RECEIVER_PIN

//================== Define NEC param ==================
#ifndef NEC_BURST

#define NEC_BURST               562

#define NEC_ADDRESS_LEN         16
#define NEC_COMMAND_LEN         16
#define NEC_DATA_LEN            (NEC_ADDRESS_LEN + NEC_COMMAND_LEN)

#define NEC_START_PULSE         (16 * NEC_BURST)    // nearly 9000
#define NEC_START_SPACE         (8 * NEC_BURST)     // nearly 4500

#define NEC_BIT_PULSE           NEC_BURST
#define NEC_ONE_SPACE           (3 * NEC_BURST)     // nearly 1690
#define NEC_ZERO_SPACE          NEC_BURST

#define NEC_REPEAT_SPACE        (4 * NEC_BURST)     // nearly 2250
#define NEC_REPEAT_PULSE        (16 * NEC_BURST)    // nearly 9000

#define NEC_REPEAT_INTERVAL     110000
#define NEC_MAX_REPEAT_SPACE    (NEC_REPEAT_INTERVAL - NEC_START_PULSE - NEC_START_SPACE - 65LL*NEC_BURST)

#endif // ends NEC_BURST --> define NEC param


//==================== Using interrupt ====================
#ifdef USE_USER_ISR_IR_RECEIVER_CB
extern void user_ir_isr_handle();   //Called each time a frame (complete or repeat) is received
#endif  //ends for using interrupt pin for receiving IR

//Mapping interrupt source //
#if (IR_RECEIVER_PIN == 2)
#define INT0_VALID
#elif (IR_RECEIVER_PIN == 3)
#define INT1_VALID
#elif ((IR_RECEIVER_PIN == 4) || (IR_RECEIVER_PIN == 5) || (IR_RECEIVER_PIN == 6) || (IR_RECEIVER_PIN == 7))
    //Enable pin change interrupt 20 to 23 for port PD4 to PD7 (Arduino pin 4 to 7)
#define PCINT2_VALID
#elif ((IR_RECEIVER_PIN == 8) || (IR_RECEIVER_PIN == 9) || (IR_RECEIVER_PIN == 10) || (IR_RECEIVER_PIN == 11) || (IR_RECEIVER_PIN == 12) || (IR_RECEIVER_PIN == 13))
    //Enable pin change interrupt 0 to 5 for port PB0 to PB5 (Arduino pin 8 to 13)
#define PCINT0_VALID
#elif ((IR_RECEIVER_PIN == A0) || (IR_RECEIVER_PIN == A1) || (IR_RECEIVER_PIN == A2) || (IR_RECEIVER_PIN == A3) || (IR_RECEIVER_PIN == A4) || (IR_RECEIVER_PIN == A5))
    //Enable pin change interrupt 8 to 13 for port PC0 to PC5 (Arduino pin A0 to A5)
#define PCINT1_VALID
#else
#error "Can't found interrupt mapping for current IR_RECEIVER_PIN. Recommend to use pin 2 as IR_RECEIVER_PIN"
#endif //ends mapping interrupt source



//======================== Define state for FSM ========================
#define RECEIVER_IDLE_S                     0    //IDLE state, wait for the start pulse
#define RECEIVER_WAIT_START_SPACE_S         1    //SYNC state, wait for the start space
#define RECEIVER_WAIT_FIRST_DATA_PULSE_S    2
#define RECEIVER_WAIT_DATA_SPACE_S          3
#define RECEIVER_WAIT_DATA_PULSE_S          4


//======================== Define flags for frame ========================
#define IR_EMPTY_FLG                    0
#define IR_NEW_CODE_FLG                 1
#define IR_REPEAT_CODE_FLG              2
#define IR_PARITY_ERROR_FLG             3


//======================== Struct for IR RECEIVER ========================
typedef union ir_data_t
{
    struct
    {
        uint8_t address;
        uint8_t inv_address;
        uint8_t command;
        uint8_t inv_command;
    } ir_data_field;
    uint32_t data;
}ir_data_t;


typedef struct ir_receiver_t {
    //For FSM
    uint32_t last_sym_change_micro;     //Number of microseconds since the program started running until the last state change of input pin
    uint8_t current_state;              //Store current state of state machine
    uint8_t current_bit_index;          //Number of bits is stored in raw data.
    uint8_t flag;
    ir_data_t raw_data;
}ir_receiver_t;


/**
 * Is filled before calling the user callback to transfer received data to main loop for further processing
 */
typedef struct ir_receiver_data_cb_t {
    uint8_t address;
    uint8_t command;
    uint8_t last_command;
    uint8_t flag;
    bool data_valid; // Is set true if new data is available
}ir_receiver_data_cb_t;

void ir_isr_handler_cb(); 
bool ir_recv_init();
bool isr_enable();
void isr_disable();

#ifdef __cplusplus
}
#endif  //ends __cplusplus

#endif  //ends _IR_LIB_H
#include <IRLib.h>

volatile ir_receiver_data_cb_t ir_receive_data;
ir_receiver_t receiver_controller;

/**
 * Callback function to handle IR ISR
 * To store IR logical input, note that: HIGH = space, LOW = pulse
 * Use micros() function for measuring length of last symbol
    + micros() returns an unsigned long integer representing the number of microseconds that have passed since the program started running
*/
void ir_isr_handler_cb(void) {
    //Use uint_fast8_t instead of uint8_t --> fastest unsigned int with at least 8 bits //TODO: Is it really necessary
    uint_fast8_t ir_input = ReadPin(IR_RECEIVER_PIN);

    uint32_t current_time_micros = micros();
    uint32_t symbol_len_micro = current_time_micros - receiver_controller.last_sym_change_micro; //Length of current symbol (can be a pulse or space) in microsecond
    receiver_controller.last_sym_change_micro = current_time_micros;

    //FSM to decode IR Receiving symbol
    uint8_t next_state = receiver_controller.current_state;

    if(ir_input) {
        //IR_RECEIVER_PIN is in HIGH state --> Have a space
        if (next_state == RECEIVER_WAIT_START_SPACE_S) {
            //If we have a valid start space (start pulse length*3/4 <= actual len <= start pulse length*5/4)
            if (((NEC_START_PULSE*3/4) <= symbol_len_micro) 
                && (symbol_len_micro <= ((NEC_START_PULSE/4)*5))) {
                next_state = RECEIVER_WAIT_FIRST_DATA_PULSE_S;
            }
            //Fail cause wrong last pulse length --> reset FSM
            else {
                next_state = RECEIVER_IDLE_S;
            }
        }

        else if (next_state == RECEIVER_WAIT_DATA_SPACE_S) {
            //Check wether we receive a valid last pulse of bit or not: Valid --> (pulse <= actual len <= pulse*3/2)
            if(((NEC_BIT_PULSE/2) <= symbol_len_micro)
                && (symbol_len_micro <= (NEC_BIT_PULSE*3/2))) {
                //Check if current bit >= data length --> It's stop bit 
                if ((receiver_controller.current_bit_index >= NEC_DATA_LEN) || (receiver_controller.flag == IR_REPEAT_CODE_FLG)) {
                    //Done a frame --> reset for others frame
                    next_state = RECEIVER_IDLE_S;

                    if (receiver_controller.flag != IR_REPEAT_CODE_FLG) {
                        receiver_controller.flag = IR_NEW_CODE_FLG;
                    }
                    
                    if ((receiver_controller.raw_data.ir_data_field.command ^ receiver_controller.raw_data.ir_data_field.inv_command) != 0xFF) {
                      receiver_controller.flag = IR_PARITY_ERROR_FLG;
                    }
                    
                    //Enable interrupt
                    // __asm__ __volatile__ ("sei" ::: "memory");
                    ir_receive_data.address = receiver_controller.raw_data.ir_data_field.inv_command;
                    ir_receive_data.command = receiver_controller.raw_data.ir_data_field.command;
                    ir_receive_data.flag = receiver_controller.flag;
                    ir_receive_data.data_valid = true;

                    //Call user's ISR callback function
#ifdef USE_USER_ISR_IR_RECEIVER_CB
                    user_ir_isr_handle();
#endif
                }
                //Not done a frame
                else {
                    next_state = RECEIVER_WAIT_DATA_PULSE_S;
                }
            }
            //Fail cause wrong last pulse length --> reset FSM
            else {
                next_state = RECEIVER_IDLE_S;
            }
        }

        // Something wrong: not find suitable state --> Reset FSM
        else {
            next_state = RECEIVER_IDLE_S;
        } 
    }

    else {
        //IR_RECEIVER_PIN is in LOW state --> Have a pulse
        if (symbol_len_micro > 2*NEC_START_PULSE) {
            //reset FSM due to time_out
            next_state = RECEIVER_IDLE_S;
            receiver_controller.flag = IR_EMPTY_FLG;
        }


        if (next_state == RECEIVER_IDLE_S) {
            //receive a pulse --> go into AGC burst phase
            next_state = RECEIVER_WAIT_START_SPACE_S;
        }

        else if (next_state == RECEIVER_WAIT_FIRST_DATA_PULSE_S) {
            //In start space phase
            //If we have a valid start space (start space length*3/4 <= actual len <= start space length*5/4)
            if (((NEC_START_SPACE*3/4) <= symbol_len_micro) 
                && (symbol_len_micro <= (NEC_START_SPACE*5/4))) {
                //Init data
                receiver_controller.current_bit_index = 0;
                receiver_controller.raw_data.data = 0;
                next_state = RECEIVER_WAIT_DATA_SPACE_S;
            }
            //Else, if (repeat space length*3/4 <= actual len <= repeat space length*5/4) and number of bit we store is >= total data length of NEC
            //--> repeat frame
            else if (((NEC_REPEAT_SPACE*3/4) <= symbol_len_micro) 
                    && (symbol_len_micro <= (NEC_REPEAT_SPACE*5/4))
                    && (receiver_controller.current_bit_index >= NEC_DATA_LEN)) {
                next_state = RECEIVER_WAIT_DATA_SPACE_S;
                receiver_controller.flag = IR_REPEAT_CODE_FLG;
            }
            //Fail cause wrong last space length --> reset FSM
            else {
                next_state = RECEIVER_IDLE_S;
            }
        }

        else if (next_state == RECEIVER_WAIT_DATA_PULSE_S) {
            //Check wether we receive a valid last space of bit or not: Valid --> (zero space <= actual len <= one space*3/2)
            if(((NEC_ZERO_SPACE/2) <= symbol_len_micro)
                && (symbol_len_micro <= (NEC_ONE_SPACE*3/2))) {
                
                next_state = RECEIVER_WAIT_DATA_SPACE_S;
                receiver_controller.current_bit_index ++;

                // Got logical 1 --> Write to raw_data
                if (symbol_len_micro >= 2*NEC_ZERO_SPACE) {
                    receiver_controller.raw_data.data |= (1UL << (receiver_controller.current_bit_index-1));
                }
            }
            //Fail cause wrong last space length --> reset FSM
            else {
                next_state = RECEIVER_IDLE_S;
            }
        }

        // Something wrong: not find suitable state --> Reset FSM
        else {
            next_state = RECEIVER_IDLE_S;
        }
    }

    receiver_controller.current_state = next_state;
}


//Flow: Init needed GPIO -> enable ISR


/**
 * Init function for IR receiver
 *      + Init GPIO Mode for needed GPIO: Set IR_RECEIVER_PIN as Input and IR_LED_PIN as Output
 *      + Enable ISR
*/
bool ir_recv_init() {
    pinMode(IR_RECEIVER_PIN, INPUT);
    pinMode(IR_LED_PIN, OUTPUT);
    return isr_enable();
}

/**
 * Function to enable ISR (init external interrupts)
 * Return 1 (true) if successfully
*/
bool isr_enable() {
#ifdef INT0_VALID
    // Any logical change on INT0 generates an interrupt request
    SET(EICRA, ISC00);
    // External interrupt flag bit
    SET(EIFR,INTF0);
    // Enable interrupt on next change
    SET(EIMSK, INT0);

#elif defined(INT1_VALID)
    // Any logical change on INT1 generates an interrupt request
    SET(EICRA, ISC10);
    // External interrupt flag bit
    SET(EIFR,INTF1);
    // Enable interrupt on next change
    SET(EIMSK, INT1);

#elif defined(PCINT0_VALID)
    // PCICR |= _BV(PCIE0);
    SET(PCICR, PCIE0);
    PCMSK0 = digitalPinToBitMask(IR_RECEIVE_PIN);
#elif defined(PCINT1_VALID)
    // PCICR |= _BV(PCIE1);
    SET(PCICR, PCIE1);
    PCMSK1 = digitalPinToBitMask(IR_RECEIVE_PIN);
#elif defined(PCINT2_VALID)
    // PCICR |= _BV(PCIE2);
    SET(PCICR, PCIE2);
    PCMSK2 = digitalPinToBitMask(IR_RECEIVE_PIN);
#else
    return false;
#endif  //ends for isr_enable()

    return true;
}

/**
 * Function to disable ISR
*/
void isr_disable() {
#if defined(INT0_VALID)
    // External interrupt flag bit
    SET(EIFR,INTF0);
    // Disable interrupt on next change
    CLEAR(EIMSK, INT0);

#elif defined(INT1_VALID)
    // External interrupt flag bit
    SET(EIFR,INTF1);
    // Disable interrupt on next change
    CLEAR(EIMSK, INT1);

#elif defined(PCINT0_VALID)
    CLEAR(PCICR, PCIE0);
    // PCICR &= ~(_BV(PCIE0));
#elif defined(PCINT1_VALID)
    CLEAR(PCICR, PCIE1);
    // PCICR &= ~(_BV(PCIE1));
#elif defined(PCINT2_VALID)
    CLEAR(PCICR, PCIE2);
    // PCICR &= ~(_BV(PCIE2));
#endif  //ends for isr_disable()
}


//Determine interrupt vector
//ISR() is a standard embedded c method for calling an interrupt vector for atmega 
//--> ISR() is how you tell the compiler to associate a function with a specific interrupt source
/**
 * Determine interrupt vector and calling interrupt vector
*/
#  if defined(INT0_VALID)
ISR(INT0_vect)

#  elif defined(INT1_VALID)
ISR(INT1_vect)

#  elif defined(PCINT0_VALID)
ISR(PCINT0_vect)
#  elif defined(PCINT1_VALID)
ISR(PCINT1_vect)
#  elif defined(PCINT2_VALID)
ISR(PCINT2_vect)
#  else
# error "Interrupt vector error"
#  endif
{
    ir_isr_handler_cb();
}
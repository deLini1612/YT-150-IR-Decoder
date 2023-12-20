// #include <Arduino.h>

// // put function declarations here:

// #include <IRremote.h>

// #define LED_PIN 13
// #define IR_RECEIVER_PIN 5

// IRrecv receiver(IR_RECEIVER_PIN);

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   pinMode(LED_PIN,OUTPUT);
//   receiver.enableIRIn();
//   Serial.println("=============Welcome to our programe===========\n");
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   if (receiver.decode()) {
//     Serial.print("Pushed a button: ");
//     // Serial.println(receiver.decodedIRData.command);
//     Serial.print(receiver.decodedIRData.command);

//     switch (receiver.decodedIRData.command)
//     {
//     case 10:  Serial.println(": INPUT");          break;
//     case 11:  Serial.println(": POWER");          break;
//     case 12:  Serial.println(": MENU");           break;
//     case 14:  Serial.println(": ESC");            break;
//     case 42:  Serial.println(": KEYSTONE +");     break;
//     case 43:  Serial.println(": D-ZOOM +");       break;
//     case 44:  Serial.println(": VOLUME +");       break;
//     case 45:  Serial.println(": KEYSTONE -");     break;
//     case 46:  Serial.println(": D-ZOOM -");       break;
//     case 47:  Serial.println(": VOLUME -");       break;
//     case 58:  Serial.println(": BLANK");          break;
//     case 59:  Serial.println(": FREEZE");         break;
//     case 60:  Serial.println(": ECO");            break;
//     case 74:  Serial.println(": UP");             break;
//     case 75:  Serial.println(": DOWN");           break;
//     case 76:  Serial.println(": ENTER");          break;
//     case 77:  Serial.println(": LEFT");           break;
//     case 78:  Serial.println(": RIGHT");          break;
//     case 90:  Serial.println(": TIMER");          break;
//     case 91:  Serial.println(": AUTO");           break;
//     case 92:  Serial.println(": ASPECT");         break;
//     case 93:  Serial.println(": FUNC");           break;
//     default:
//       Serial.println(": Receive IR Error!"); break;
//     }

//     digitalWrite(LED_PIN, HIGH);

//     switch (receiver.decodedIRData.protocol) {
//       case NEC: Serial.println("NEC"); break ;
//       case SONY: Serial.println("SONY"); break ;
//       case RC5: Serial.println("RC5"); break ;
//       case RC6: Serial.println("RC6"); break ;
//       case SHARP: Serial.println("SHARP"); break ;
//       case JVC: Serial.println("JVC"); break ;
//       case SAMSUNG: Serial.println("SAMSUNG"); break ;
//       case LG: Serial.println("LG"); break ;
//       case WHYNTER: Serial.println("WHYNTER"); break ;
//       case PANASONIC: Serial.println("PANASONIC"); break ;
//       case DENON: Serial.println("DENON"); break ;
//     default:
//       case UNKNOWN: Serial.println("UNKNOWN"); break ;
//     }
//     Serial.println("");
//     delay(100);
//     digitalWrite(LED_PIN, LOW);
//     receiver.resume();
//   }
// }

#include <Arduino.h>
#include <IRLib.h>
#include <config.h>

volatile ir_receiver_data_cb_t ir_receive_data;

void setup() {
    Serial.begin(115200);
    Serial.println(F("START " __FILE__ " from " __DATE__));
    if (!ir_recv_init()) {
        Serial.println("No interrupt available for your IR pin");
    }
    Serial.println("====Welcome to our IR receiver====");
}

void loop() {
    if (ir_receive_data.data_valid) {
        ir_receive_data.data_valid = false;
        Serial.print(F("Address = "));
        Serial.print(ir_receive_data.address);
        Serial.print(F(" Command = "));
        Serial.print(ir_receive_data.command);
        
        if (ir_receive_data.flag == IR_NEW_CODE_FLG) {
            Serial.println(" -N");
        }
        else if (ir_receive_data.flag == IR_REPEAT_CODE_FLG) {
            Serial.println(" -R");
        }
        else if (ir_receive_data.flag == IR_PARITY_ERROR_FLG) {
            Serial.println(" -E");
        }
    }
}


void user_ir_isr_handle()
{
  Serial.print("Push a button: ");
}

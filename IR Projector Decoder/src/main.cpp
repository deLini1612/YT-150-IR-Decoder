#include <Arduino.h>
#include <IRLib.h>
#include <config.h>

#ifdef TEST_IR_LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#endif

volatile ir_receiver_data_cb_t ir_receive_data;

#ifdef TEST_IR_LCD

#define P_NORMAL  0
#define P_BLANK   1
#define P_FREEZE  2

#define SCREEN_MAIN   0
#define SCREEN_ECO    1

String status[3] = {"NORMAL", "BLANK", "FREEZE"};

LiquidCrystal_I2C lcd(0x27,16,2); //0x27 = địa chỉ I2c; 16 và 12 là kích thước LCD 16x2

typedef struct projector_t {
    bool power;
    bool eco;
    uint8_t eco_level;
    uint8_t state;
    uint8_t volumn;
    uint8_t screen;
}projector_t;

projector_t projector;
bool line;

void init_projector(projector_t *p) {
    p->power = 0;
    p->eco = 0;
    p->eco_level = 0;
    p->state = P_NORMAL;
    p->volumn = 100;
    p->screen = SCREEN_MAIN;
}

void projector_display(projector_t p){
  lcd.clear();
  if (p.screen == SCREEN_MAIN) {
    lcd.setCursor(2,0);
    lcd.print("MODE:");
    lcd.print(status[p.state]);
    lcd.setCursor(3,1);
    lcd.print("VOLUMN:");
    lcd.print(p.volumn/2);
  }
  else if (p.screen == SCREEN_ECO) {
    lcd.setCursor(0,0);
    lcd.print("ECO SETTING: ");
    if (p.eco)
    {
        lcd.print("ON");
        lcd.setCursor(3,1);
        for (int i = 0; i < p.eco_level; i++)
            lcd.print("--");
    }
    else lcd.print("OFF");
  }
}

void update_projector(projector_t *p)
{
    if (ir_receive_data.command == 11) {
        if (ir_receive_data.flag == IR_NEW_CODE_FLG) {
            p->power = !(p->power);
            if(p->power) {
                lcd.backlight();
                p->screen = SCREEN_MAIN;
            }
            else {
                p->state = P_NORMAL;
            }
        }
    }
    else if (p->power) {
        if (p->screen == SCREEN_MAIN) {
            switch (ir_receive_data.command)
            {
                case 14:
                    if ((ir_receive_data.flag == IR_NEW_CODE_FLG)&&((p->state == P_BLANK)||(p->state == P_FREEZE))) p->state = P_NORMAL;
                    break;

                case 44:
                    if(p->volumn < 200) p->volumn++;
                    break;

                case 47:
                    if(p->volumn > 0) p->volumn--;
                    break;

                case 58:
                    if (ir_receive_data.flag == IR_NEW_CODE_FLG) {
                        if (p->state == P_BLANK) p->state = P_NORMAL;
                        else p->state = P_BLANK;
                    }
                    break;

                case 59:
                    if (ir_receive_data.flag == IR_NEW_CODE_FLG) {
                        if (p->state == P_FREEZE) p->state = P_NORMAL;
                        else p->state = P_FREEZE;
                    }       
                    break;

                case 60:
                    if (ir_receive_data.flag == IR_NEW_CODE_FLG) {
                        line = 0;
                        p->screen = SCREEN_ECO;
                    }       
                    break;

                default:
                    digitalWrite(IR_LED_PIN, 1);//other
            }
        }

        else if (p->screen == SCREEN_ECO) {
            if (ir_receive_data.command == 14) 
            {
                if (ir_receive_data.flag == IR_NEW_CODE_FLG) p->screen = SCREEN_MAIN;
            }
            else 
            {
                if (!line) {
                    if ((ir_receive_data.command == 77)||(ir_receive_data.command == 78)) {
                            if (ir_receive_data.flag == IR_NEW_CODE_FLG) p->eco = !(p->eco);
                        }
                    else if ((p->eco == 1)&&(ir_receive_data.command == 75)&&(ir_receive_data.flag == IR_NEW_CODE_FLG)) line = 1;
                }
                else {
                    if ((ir_receive_data.command==74)&&(ir_receive_data.flag == IR_NEW_CODE_FLG)) line = 0;
                    else if ((ir_receive_data.command==78)&&(p->eco_level < 5)&&(ir_receive_data.flag == IR_NEW_CODE_FLG)) p->eco_level++;
                    else if ((ir_receive_data.command==77)&&(p->eco_level > 0)&&(ir_receive_data.flag == IR_NEW_CODE_FLG)) p->eco_level--;
                }
            }
        }
    }
}

#endif

void setup() {
    pinMode(IR_LED_PIN, OUTPUT);
    Serial.begin(115200);
    Serial.println(F("START " __FILE__ " from " __DATE__"\n"));
    if (!ir_recv_init()) {
        Serial.println("No interrupt available for your IR pin");
    }

    #ifdef TEST_IR_LCD
    lcd.init();                    
    lcd.backlight(); //Bật đèn nền LCD
    lcd.setCursor(3,0);
    lcd.print("IR Decoder");
    lcd.setCursor(1,1);
    lcd.print("LinhHieuDuong!");
    init_projector(&projector);
    delay(2000);
    lcd.noBacklight();
    lcd.clear();
    #endif
}

void loop() {
  if (ir_receive_data.data_valid) {
    ir_receive_data.data_valid = false;
    if (ir_receive_data.flag == IR_PARITY_ERROR_FLG) digitalWrite(IR_LED_PIN, 1);
    else digitalWrite(IR_LED_PIN, 0);

    #ifdef TEST_IR_SERIAL
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
    #endif

    #ifdef TEST_IR_LCD
    update_projector(&projector);
    if(projector.power == 1) projector_display(projector);
    else 
    {
      lcd.noBacklight();
      lcd.clear();
    }
    #endif
  }
}

void user_ir_isr_handle()
{
  #ifdef TEST_IR_SERIAL
  Serial.print("Push a button: ");
  #endif

  #ifdef TEST_IR_LCD
  //user's code
  #endif
}
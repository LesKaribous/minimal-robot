#ifndef PIN_H
#define PIN_H

const int ihm_init = 38,
    ihm_tirette = 32,
    ihm_couleur = 31,
    ihm_strategy = 36;

const int neopixel = 37;

const int pinRobot = 2;
const int buzzer = 3;

// TFT pinout
const int TFT_CS = 10,
    TFT_DC = 9,
    TFT_RST = 255,
    TFT_MOSI = 11,
    TFT_SCK = 13,
    TFT_MISO = 12;

//const int T_CS_PIN = 51, T_IRQ_PIN = 52;

// Stepper Pinout
const int stepper_en = 24;

const int dir_1 = 4, 
    dir_2 = 25, 
    dir_3 = 26 ;
const int step_1 = 6, 
    step_2 = 27, 
    step_3 = 5 ;

// Actuator Pinout
const int pinServo01 = 21;
const int pinServo02 = 22;
const int pinServo03 = 20;

const int pinServo04 = 23;
const int pinServo05 = 16;
const int pinServo06 = 39;

const int pinServo07 = 15;
const int pinServo08 = 14;
const int pinServo09 = 17;

//  EnableRegulator 
const int pinEnaTraco   = 28;

#endif // PIN_H
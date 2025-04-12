#include <Arduino.h>
#include <Pin.h>
#include <UI/UI.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <SPI.h>
#include <ILI9341_t3.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define NUMPIXELS 2

#define ON true
#define OFF false

void init_pinout();
void init_servo();
void free_servo();
void check_servo();
void initPump();
void testPump();
void moveServo(Servo &servo, int angleA, int angleB, int speed);
void sequence_actuator();
void setOutput(uint8_t pin, bool state);
void startPump(uint8_t pumpPin, uint8_t evPin);
void stopPump(uint8_t pumpPin, uint8_t evPin, uint16_t evPulseDuration = 500);

Servo CA_LeftGripper;
Servo CA_RightGripper;
Servo CA_PlankGripper;

Servo AB_LeftGripper;
Servo AB_RightGripper;
Servo AB_PlankGripper;

Servo CA_Elevator;
Servo AB_Elevator;
Servo BC_Elevator;

int CA_ElevatorHigh = 20;
int CA_ElevatorLow  = 120;

int AB_ElevatorHigh = 20;
int AB_ElevatorLow  = 120;

int BC_ElevatorHigh = 36;
int BC_ElevatorLow  = 17;

int CA_LeftGripperOpen = 0;
int CA_LeftGripperGrab = 180;

int CA_RightGripperOpen = 180;
int CA_RightGripperGrab = 0;

int AB_LeftGripperOpen = 0;
int AB_LeftGripperGrab = 170;

int AB_RightGripperOpen = 180;
int AB_RightGripperGrab = 30;

int CA_PlankGripperGrab = 90;
int CA_PlankGripperRaise = 130;

int AB_PlankGripperGrab = 90;
int AB_PlankGripperRaise = 130;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(115200);
  Serial.println("Hello");
  init_pinout();
  init_servo();
  initPump();
  delay(500);
  init_tft();
  //check_servo();
  //free_servo();
}

void loop() {
  testPump();
  sequence_actuator();
}

void init_pinout(){
  pinMode(ihm_init,INPUT_PULLUP);
  pinMode(ihm_tirette,INPUT_PULLUP);
  pinMode(ihm_couleur,INPUT_PULLUP);
  pinMode(ihm_strategy,INPUT_PULLUP);

  pinMode(ihm_analog,INPUT);

  pinMode(pinEnaTraco,OUTPUT);
  digitalWrite(pinEnaTraco,HIGH);
}

void init_servo(){

  CA_LeftGripper.attach(pinServo01);
  CA_RightGripper.attach(pinServo02);
  CA_PlankGripper.attach(pinServo03);
  
  AB_LeftGripper.attach(pinServo04);
  AB_RightGripper.attach(pinServo05);
  AB_PlankGripper.attach(pinServo06);
  
  CA_Elevator.attach(pinServo07);
  AB_Elevator.attach(pinServo08);
  BC_Elevator.attach(pinServo09);

  CA_LeftGripper.write(CA_LeftGripperOpen);
  CA_RightGripper.write(CA_RightGripperOpen);
  CA_PlankGripper.write(CA_PlankGripperRaise);
  
  AB_LeftGripper.write(AB_LeftGripperOpen);
  AB_RightGripper.write(AB_RightGripperOpen);
  AB_PlankGripper.write(AB_PlankGripperRaise);
  
  CA_Elevator.write(CA_ElevatorLow);
  AB_Elevator.write(AB_ElevatorLow);
  BC_Elevator.write(BC_ElevatorLow);
}

void free_servo(){
  CA_LeftGripper.detach();
  CA_RightGripper.detach();
  CA_PlankGripper.detach();
  
  AB_LeftGripper.detach();
  AB_RightGripper.detach();
  AB_PlankGripper.detach();
  
  CA_Elevator.detach();
  AB_Elevator.detach();
  BC_Elevator.detach();
}

void check_servo(){
  // To be done
}

void moveServo(Servo &servo, int angleA, int angleB, int speed) {
    if (angleA > angleB) {
        for (int pos = angleA; pos >= angleB; pos--) {
            servo.write(pos);
            delay(speed);
        }
    } else {
        for (int pos = angleA; pos <= angleB; pos++) {
            servo.write(pos);
            delay(speed);
        }
    }
}

void initPump(){
  pwm.begin();
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm.setOscillatorFrequency(50000000);
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  //Wire.setClock(400000);

}

void setOutput(uint8_t pin, bool state) {
  if (state) {
    pwm.setPWM(pin, 4096, 0);  // ON
  } else {
    pwm.setPWM(pin, 0, 4096);  // OFF
  }
}

void startPump(uint8_t pumpPin, uint8_t evPin) {
  setOutput(evPin, false);  // Fermer l'électrovanne
  setOutput(pumpPin, true); // Démarrer la pompe
}

void stopPump(uint8_t pumpPin, uint8_t evPin, uint16_t evPulseDuration = 500) {
  setOutput(pumpPin, false); // Stopper la pompe
  setOutput(evPin, true);    // Ouvrir l’EV
  delay(evPulseDuration);    // Maintenir l’EV ouverte
  setOutput(evPin, false);   // Fermer l’EV
}

void testPump() {
  startPump(Pump_AB, Ev_AB);
  delay(2000);               // Pompage
  stopPump(Pump_AB, Ev_AB);  // Relâche pression

  delay(2000);

  startPump(Pump_CA, Ev_CA);
  delay(2000);
  stopPump(Pump_CA, Ev_CA);
}


void sequence_actuator(){
  /*
  Servo01 - 
  Servo02 - 
  Servo03 - 

  Servo04 - Arm Suction Cup AB
  Servo05 - Gripper AB Left
  Servo06 - Gripper AB Right

  Servo07 - Elevator CA
  Servo08 - Elevator AB
  Servo09 - Elevator BC
  */

  /*
  // Test elevator + pump
  pwm.setPWM(0, 4096, 0 ); 
  pwm.setPWM(1, 0 , 4096 );
  delay(500);
  moveServo(Servo04, 45, 5, 5);
  delay(2000);
  moveServo(Servo04, 0, 45, 5); // Monter le bras
  moveServo(Servo08, 0, 45, 20); // Monter l'elevator
  delay(4000);
  moveServo(Servo08, 45, 0, 20); // Descendre l'elevator
  moveServo(Servo04, 45, 5, 5); // Descendre le bras
  delay(2000);
  pwm.setPWM(0, 0, 4096 ); 
  pwm.setPWM(1, 4096 , 0 );
  delay(500);
  pwm.setPWM(1, 0 , 4096 );
  delay(500);
  moveServo(Servo04, 0, 45, 5);
  */

  // Test Elevator
  /*
  Servo04.write(40);
  pwm.setPWM(0, 4096, 0 ); 
  delay(2000);
  moveServo(Servo08, 110, 10, 20);
  delay(2000);
  moveServo(Servo08, 10, 110, 25);
  pwm.setPWM(0, 0, 4096 ); 
  delay(2000);
  */

  /*
  // Test gripper magnetic
  Servo04.write(45);
  moveServo(Servo08, 45, 0, 20); // Descendre l'elevator
  delay(1000);
  moveServo(Servo05, 0, 180, 2); // Take Left Can
  moveServo(Servo06, 160, 10, 2); // Take Right Can
  moveServo(Servo08, 0, 45, 20); // Monter l'elevator
  delay(2000);
  moveServo(Servo08, 45, 0, 20); // Descendre l'elevator
  moveServo(Servo05, 180, 0, 2); // Release Left Can
  moveServo(Servo06, 10, 160, 2); // Release Right Can
  delay(2000);
  moveServo(Servo08, 0, 45, 20); // Monter l'elevator
  */

  /*
  Servo04.write(45);
  moveServo(Servo08, 45, 0, 20); // Descendre l'elevator
  delay(1000);
  pwm.setPWM(0, 4096, 0 ); 
  pwm.setPWM(1, 0 , 4096 );
  delay(500);
  moveServo(Servo04, 45, 5, 5); //Descendre le bras
  moveServo(Servo05, 0, 180, 2); // Take Left Can
  moveServo(Servo06, 160, 10, 2); // Take Right Can
  moveServo(Servo04, 0, 45, 5); // Monter le bras
  moveServo(Servo08, 0, 45, 20); // Monter l'elevator
  delay(2000);
  moveServo(Servo08, 45, 0, 20); // Descendre l'elevator
  moveServo(Servo04, 45, 5, 5); //Descendre le bras
  moveServo(Servo05, 180, 0, 2); // Release Left Can
  moveServo(Servo06, 10, 160, 2); // Release Right Can
  pwm.setPWM(0, 0, 4096 ); 
  pwm.setPWM(1, 4096 , 0 );
  delay(500);
  pwm.setPWM(1, 0 , 4096 );
  delay(500);
  moveServo(Servo04, 0, 45, 5); // Monter le bras
  delay(2000);
  moveServo(Servo08, 0, 45, 20); // Monter l'elevator
  */

  /*
  // Test Pump 1
  delay(1000);
  pwm.setPWM(0, 4096, 0 ); 
  pwm.setPWM(1, 0 , 4096 );
  pwm.setPWM(2, 4096, 0 ); 
  pwm.setPWM(3, 0 , 4096 );
  delay(5000);
  pwm.setPWM(0, 0, 4096 ); 
  pwm.setPWM(1, 4096 , 0 );
  pwm.setPWM(2, 0, 4096 ); 
  pwm.setPWM(3, 4096 , 0 );
  delay(1000);
  pwm.setPWM(1, 0 , 4096 );
  pwm.setPWM(3, 0 , 4096 );
  */
  //delay(3000);
}

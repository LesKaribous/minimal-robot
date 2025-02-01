#include <Arduino.h>
#include <Pin.h>
#include <UI/UI.h>
#include <UI/notes.h>
#include <Adafruit_NeoPixel.h>
#include <TeensyStep.h>
#include <Servo.h>
#include <SPI.h>
#include <ILI9341_t3.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define NUMPIXELS 2

void init_pinout();
void init_servo();
void playTone(int frequency, int duration);
void playMelody(int *notes, int *durations, int length, int tempo);
void playStartupMelody();
void free_servo();
void init_stepper();
void check_neopixel();
void color_neopixel(char R,char G, char B);
void rainbow_neopixel();
void check_stepper_move();
void check_stepper_rotate();
void check_servo();
void updateScore(int uScore);
void testScore();
void initPump();
void testPump();

Stepper stepper_A(step_1, dir_1);
Stepper stepper_B(step_2, dir_2);
Stepper stepper_C(step_3, dir_3);

StepControl StepController;
RotateControl RotController;

Servo Servo01;
Servo Servo02;
Servo Servo03;

Servo Servo04;
Servo Servo05;
Servo Servo06;

Servo Servo07;
Servo Servo08;
Servo Servo09;

int open01 = 90;
int open02 = 90;
int open03 = 90;

int open04 = 90;
int open05 = 90;
int open06 = 90;

int open07 = 90;
int open08 = 90;
int open09 = 90;

int close01 = 0;
int close02 = 0;
int close03 = 0;

int close04 = 0;
int close05 = 0;
int close06 = 0;

int close07 = 0;
int close08 = 0;
int close09 = 0;

Adafruit_NeoPixel pixels(NUMPIXELS, neopixel, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 0
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(115200);
  Serial.println("Hello");
  Serial1.begin(115200); // Lidar
  init_pinout();
  init_stepper();
  pixels.begin();
  init_tft();
  init_servo();
  initPump();
  delay(500);
  //!!!!!!!!
  while(1) testPump(); //!!!!!!!!
  check_servo();
  free_servo();
  drawBackScreenStart();
  playTone(NOTE_B0,400);
  playTone(NOTE_C1,400);
}

void loop() {
  
  while(getTiretteState()!= TIRETTE_GO)
  {
    updateAllStartVar();
    if(initHasPressed())
    {
      updateAllStartVar();
      check_neopixel();
      rainbow_neopixel();
      // Initialize
      check_stepper_move();
      // end Initialize
      delay(2000);
      setInitState(DONE_INIT);
    }
  }
  drawBackScreenMatch();
  updateAllMatchVar();

    // Match
    //init_servo();
    //check_servo();
    //free_servo();
    //check_neopixel();
    //rainbow_neopixel();
    check_stepper_move();
    //check_stepper_rotate();
    //while(1) ihmTurbine();
    // End Match
    if(initHasPressed()) while(!checkRestartRequest());
    delay(1000);
}

void init_pinout(){
  pinMode(ihm_init,INPUT_PULLUP);
  pinMode(ihm_tirette,INPUT_PULLUP);
  pinMode(ihm_couleur,INPUT_PULLUP);
  pinMode(ihm_strategy,INPUT_PULLUP);

  pinMode(neopixel,OUTPUT);

  pinMode(dir_1,OUTPUT);
  pinMode(dir_2,OUTPUT);
  pinMode(dir_3,OUTPUT);

  pinMode(step_1,OUTPUT);
  pinMode(step_2,OUTPUT);
  pinMode(step_3,OUTPUT);

  pinMode(stepper_en,OUTPUT);
  digitalWrite(stepper_en,HIGH);

  pinMode(pinEnaTraco,OUTPUT);
  digitalWrite(pinEnaTraco,HIGH);
}

void init_stepper(){
  digitalWrite(stepper_en,HIGH);

  stepper_A.setMaxSpeed(500);
  stepper_B.setMaxSpeed(500);
  stepper_C.setMaxSpeed(500);

  stepper_A.setAcceleration(500);
  stepper_B.setAcceleration(500);
  stepper_C.setAcceleration(500);
}

void init_servo(){
  Servo01.attach(pinServo01);
  Servo02.attach(pinServo02);
  Servo03.attach(pinServo03);

  Servo04.attach(pinServo04);
  Servo05.attach(pinServo05);
  Servo06.attach(pinServo06);

  Servo07.attach(pinServo07);
  Servo08.attach(pinServo08);
  Servo09.attach(pinServo09);

  Servo01.write(open01);
  Servo02.write(open02);
  Servo03.write(open03);

  Servo04.write(open04);
  Servo05.write(open05);
  Servo06.write(open06);

  Servo07.write(open07);
  Servo08.write(open08);
  Servo09.write(open09);
}

void free_servo(){
  Servo01.detach();
  Servo02.detach();
  Servo03.detach();

  Servo04.detach();
  Servo05.detach();
  Servo06.detach();

  Servo07.detach();
  Servo08.detach();
  Servo09.detach();
}

void check_servo(){
  Servo01.write(open01);
  Servo02.write(open02);
  Servo03.write(open03);

  Servo04.write(open04);
  Servo05.write(open05);
  Servo06.write(open06);

  Servo07.write(open07);
  Servo08.write(open08);
  Servo09.write(open09);

  delay(3000);
  Servo01.write(close01);
  Servo02.write(close02);
  Servo03.write(close03);

  Servo04.write(close04);
  Servo05.write(close05);
  Servo06.write(close06);

  Servo07.write(close07);
  Servo08.write(close08);
  Servo09.write(close09);
  delay(3000);
}

void playTone(int frequency, int duration) {
  if (frequency > 0) {
    tone(buzzer, frequency, duration);
  } else {
    tone(buzzer, 0, duration);
  }
}

// Fonction pour jouer une mélodie avec un tempo spécifique
void playMelody(int *notes, int *durations, int length, int tempo) {
  for (int i = 0; i < length; i++) {
    int noteDuration = (tempo * 4) / durations[i]; // Calculer la durée réelle de la note
    playTone(notes[i], noteDuration);       // Jouer chaque note
    delay(noteDuration * 0.1);              // Petite pause entre les notes (10% de la durée)
  }
  tone(buzzer, 0);
}

void playStartupMelody() {
   // Tempo spécifique pour cette mélodie (durée d'une noire en ms)
  int tempo = 400;

  // Notes du riff "We will rock you"
  int melody[] = {
    NOTE_G4, NOTE_F4, NOTE_REST, NOTE_E4, NOTE_D4, NOTE_REST, NOTE_E4, NOTE_E4, NOTE_REST
  };

  // Durées des notes : 2 = blanche, 4 = noire, 8 = croche
  int noteDurations[] = {
    4, 8, 8, 4, 8, 8, 8, 8, 4 // Dernière note prolongée
  };

  // Longueur de la mélodie
  int length = sizeof(melody) / sizeof(melody[0]);

  // Jouer la mélodie avec le tempo spécifique
  playMelody(melody, noteDurations, length, tempo);
}

void check_neopixel(){
  pixels.clear();
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
    delay(DELAYVAL);
  }
}

void color_neopixel(char R,char G, char B){
  pixels.clear();
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(R, G, B));
    pixels.show();
  }
}

void rainbow_neopixel(){
  pixels.clear();
  int interDelay = 2;
  for(int j = 0; j <= 255; j++){
    color_neopixel(j,j,j);
    delay(interDelay);
  }
  for(int j = 255; j >= 0; j--){
    color_neopixel(j,j,j);
    delay(interDelay);
  }
  for(int r = 0; r <= 255; r++){
    color_neopixel(r,0,0);
    delay(interDelay);
  }
  for(int r = 255; r >= 0; r--){
    color_neopixel(r,0,0);
    delay(interDelay);
  }
  for(int g = 0; g <= 255; g++){
    color_neopixel(0,g,0);
    delay(interDelay);
  }
  for(int g = 255; g >= 0; g--){
    color_neopixel(0,g,0);
    delay(interDelay);
  }
  for(int b = 0; b <= 255; b++){
    color_neopixel(0,0,b);
    delay(interDelay);
  }
  for(int b = 255; b >= 0; b--){
    color_neopixel(0,0,b);
    delay(interDelay);
  }

}

void check_stepper_move(){
  digitalWrite(stepper_en,LOW);

  stepper_A.setTargetAbs(0);
  stepper_B.setTargetAbs(0);
  stepper_C.setTargetAbs(0);

  StepController.moveAsync(stepper_A, stepper_B, stepper_C);
  while(StepController.isRunning()){
    //Serial.print("|");
    //delay(500);
    delay(1);
  }

  stepper_A.setTargetAbs(500);
  stepper_B.setTargetAbs(500);
  stepper_C.setTargetAbs(500);

  StepController.moveAsync(stepper_A, stepper_B, stepper_C);

  while(StepController.isRunning()){
    //Serial.print("|");
    //delay(500);
    delay(1);
  }

  stepper_A.setTargetAbs(-500);
  stepper_B.setTargetAbs(-500);
  stepper_C.setTargetAbs(-500);

  StepController.moveAsync(stepper_A, stepper_B, stepper_C);

  while(StepController.isRunning()){
    //Serial.print("|");
    //delay(500);
    delay(1);
  }

  digitalWrite(stepper_en,HIGH);
}

void check_stepper_rotate(){
  digitalWrite(stepper_en,LOW);
  RotController.rotateAsync(stepper_A, stepper_B, stepper_C);
  delay(2000);
  RotController.stop();
  digitalWrite(stepper_en,HIGH);
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

void testPump(){

  pwm.setPWM(0, 4096, 0 ); 
  pwm.setPWM(1, 0 , 4096 ); 
  delay(2000);
  pwm.setPWM(0, 0, 4096 ); 
  pwm.setPWM(1, 4096 , 0 );
  delay(500);
  pwm.setPWM(1, 0 , 4096 ); 
  delay(2000);

}
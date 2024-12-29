#include <Arduino.h>
#include <Pin.h>
#include <UI/UI.h>
#include <UI/notes.h>
#include <Adafruit_NeoPixel.h>
#include <TeensyStep.h>
#include <Servo.h>
#include <SPI.h>
#include <ILI9341_t3.h>

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

int open01 = 60;
int open02 = 100;
int open03 = 100;

int open04 = 60;
int open05 = 100;
int open06 = 100;

int open07 = 60;
int open08 = 100;
int open09 = 100;

int close01 = 100;
int close02 = 80;
int close03 = 80;

int close04 = 100;
int close05 = 80;
int close06 = 80;

int close07 = 100;
int close08 = 80;
int close09 = 80;

Adafruit_NeoPixel pixels(NUMPIXELS, neopixel, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 0

void setup() {
  Serial.begin(115200);
  Serial.println("Hello");
  Serial1.begin(115200); // Lidar
  init_pinout();
  init_stepper();
  pixels.begin();
  init_tft();
  init_servo();
  delay(500);
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

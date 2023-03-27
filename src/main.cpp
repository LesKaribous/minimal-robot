#include <Arduino.h>
#include <pinout.h>
#include <Adafruit_NeoPixel.h>
#include <TeensyStep.h>
#include <Servo.h>
#include <SPI.h>
#include <ILI9341_t3n.h>

#define NUMPIXELS 4

void init_pinout();
void init_servo();
void free_servo();
void init_tft();
void init_stepper();
void check_ihm();
void check_neopixel();
void color_neopixel(char R,char G, char B);
void rainbow_neopixel();
void check_stepper_move();
void check_stepper_rotate();
void check_servo();
void tft_test();
void check_ihm_tft();
void setTurbineSpeed(int speed);
void checkTurbine();
void testLidar();
void checkLidar();

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

Servo ServoTrap;
int turbineSpeed = 0;

int objAngle = 0, objDistance = 0;
bool newLidarData = false ;

Adafruit_NeoPixel pixels(NUMPIXELS, neopixel, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 0

ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);

void setup() {
  Serial.begin(115200);
  Serial.println("Hello");
  Serial1.begin(115200); // Lidar
  init_pinout();
  init_stepper();
  init_tft();
  pixels.begin();
  init_servo();
  delay(500);
  free_servo();
  testLidar();
}

void loop() {
  //checkTurbine();
  //tft_test();
  //check_ihm_tft();
  //check_ihm();
  //check_servo();
  //check_neopixel();
  //rainbow_neopixel();
  //check_stepper_rotate();
  //check_stepper_move();
  checkLidar();
  delay(200);
}

void init_pinout(){
  pinMode(ihm_init,INPUT_PULLUP);
  pinMode(ihm_tirette,INPUT_PULLUP);
  pinMode(ihm_couleur,INPUT_PULLUP);
  pinMode(ihm_detection,INPUT_PULLUP);

  pinMode(neopixel,OUTPUT);

  pinMode(dir_1,OUTPUT);
  pinMode(dir_2,OUTPUT);
  pinMode(dir_3,OUTPUT);

  pinMode(step_1,OUTPUT);
  pinMode(step_2,OUTPUT);
  pinMode(step_3,OUTPUT);

  pinMode(stepper_en,OUTPUT);

  pinMode(pinTurbine,OUTPUT);
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

  ServoTrap.attach(pinServoTrap);

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

void check_ihm(){
  Serial.print("Init : ");
  Serial.println(digitalRead(ihm_init));
  Serial.print("Tirette : ");
  Serial.println(digitalRead(ihm_tirette));
  Serial.print("Couleur : ");
  Serial.println(digitalRead(ihm_couleur));
  Serial.print("Detection : ");
  Serial.println(digitalRead(ihm_detection));
}

void check_ihm_tft(){
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);

  tft.fillRect(150,0,20,20, ILI9341_BLACK);
  tft.fillRect(150,20,20,20, ILI9341_BLACK);
  tft.fillRect(150,40,20,20, ILI9341_BLACK);
  tft.fillRect(150,60,20,20, ILI9341_BLACK);

  tft.drawString("Init : ",0,0);      tft.drawNumber(digitalRead(ihm_init),150,0);
  tft.drawString("Tirette : ",0,20);   tft.drawNumber(digitalRead(ihm_tirette),150,20);
  tft.drawString("Strategie : ",0,40); tft.drawNumber(digitalRead(ihm_detection),150,40);

  bool team_color = digitalRead(ihm_couleur);
  if (team_color) tft.setTextColor(ILI9341_BLUE);
  else tft.setTextColor(ILI9341_GREEN);
  tft.drawString("Couleur : ",0,60);   tft.drawNumber(team_color,150,60);
  
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
    Serial.print("|");
    delay(500);
  }

  stepper_A.setTargetAbs(500);
  stepper_B.setTargetAbs(500);
  stepper_C.setTargetAbs(500);

  StepController.moveAsync(stepper_A, stepper_B, stepper_C);

  while(StepController.isRunning()){
    Serial.print("|");
    delay(500);
  }

  stepper_A.setTargetAbs(-500);
  stepper_B.setTargetAbs(-500);
  stepper_C.setTargetAbs(-500);

  StepController.moveAsync(stepper_A, stepper_B, stepper_C);

  while(StepController.isRunning()){
    Serial.print("|");
    delay(500);
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

void init_tft(){
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
}

void tft_test(){
  tft.println(F("Hello World"));
}

void setTurbineSpeed(int speed){
  speed = constrain(speed,0,255);
  if(speed>turbineSpeed)
  {
    for(int i=turbineSpeed; i<=speed; i++)
    {
      analogWrite(pinTurbine,i);
      delay(5);
    }
  }
  else if (speed<turbineSpeed)
  {
    for(int i=turbineSpeed; i>=speed; i--)
    {
      analogWrite(pinTurbine,i);
      delay(5);
    }
  }
  turbineSpeed = speed;
}

void checkTurbine(){
  ServoTrap.attach(pinServoTrap);
  ServoTrap.write(50);
  setTurbineSpeed(250);
  delay(15000);
  setTurbineSpeed(0);
  delay(2000);
  ServoTrap.write(140);
  delay(3000);
  ServoTrap.write(50);
  ServoTrap.detach();
}

void testLidar(){
  // Envoi une demande de detection
  Serial1.println("G90,400;");
}

void checkLidar(){
  if(newLidarData){
    Serial.print("M");
    Serial.print(objAngle);
    Serial.print(",");
    Serial.println(objDistance);
    newLidarData = false;
  }
}

void serialEvent1() {
  // Serial.println("Input from lidar...");
  // Lit les données jusqu'à la fin de la trame
  String inputString = Serial1.readStringUntil('\n');
  Serial.println(inputString);
  if (inputString.charAt(0) == 'M')
  {
    // Extraction des données à partir de la trame
    int commaIndex = inputString.indexOf(',');
    int xIndex = 1;
    int yIndex = commaIndex + 1;
    if (commaIndex != -1 && inputString.charAt(inputString.length() - 2) == ';') // -2 pour le serial hardware
    {
      // Conversion des données en entiers
      String xString = inputString.substring(xIndex, commaIndex);
      String yString = inputString.substring(yIndex, inputString.length() - 1);
      objAngle = xString.toInt();
      objDistance   = yString.toInt();
      newLidarData = true; // Active le flag newData lorsque des données sont disponibles
    } else {
      Serial.println("Invalid data format on serial 1 (; missing)");
    }
  } 
  else 
  {
    Serial.println("Invalid data format on serial 1 (M missing)");
  }
}
// UI.cpp
#include "UI.h"
#include "Pin.h"
#include <ILI9341_t3n.h> // Assurez-vous d'inclure la bibliothèque ILI9341_t3n dans platformio.ini

// Créez l'objet ILI9341_t3n avec les broches définies dans Pin.h
ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);

bool
    ROBOT       = PRIMARY,
    LIDAR_STATE = LIDAR_ERROR,
    TEAM_COLOR  = GREEN,
    STRATEGY    = CAKE_FIRST;

bool PREVIOUS_LIDAR_STATE = LIDAR_STATE;
bool firstUpdate = true;

bool getRobot(){return ROBOT = digitalRead(pinRobot);}
bool getTeamColor(){return TEAM_COLOR = digitalRead(ihm_couleur);}
bool getStrategy(){return STRATEGY = digitalRead(ihm_strategy);}
bool getLidarState(){return LIDAR_STATE;}

void setRobot(bool state)       {ROBOT       = state;}
void setTeamColor(bool state)   {TEAM_COLOR  = state;}
void setStrategy(bool state)    {STRATEGY    = state;}
void setLidarState(bool state){
    if(state != LIDAR_STATE){LIDAR_STATE = state;}
}

bool teamHasChanged(){
    if(TEAM_COLOR != digitalRead(ihm_couleur)) return true;
    else return false;
}

bool strategyHasChanged(){
    if(STRATEGY != digitalRead(ihm_strategy)) return true;
    else return false;
}

bool lidarHasChanged(){
    if(LIDAR_STATE != PREVIOUS_LIDAR_STATE) 
    {
        PREVIOUS_LIDAR_STATE = LIDAR_STATE;
        return true;
    }
    else return false;
}

void init_tft() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
}

void drawBackScreenStart() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_ORANGE);
  tft.setTextSize(2);
  // Texte "Etats"
  tft.setCursor(10,10);
  tft.println("Strat : ");
  tft.setCursor(10,30);
  tft.println("    X :        mm");
  tft.setCursor(10,50);
  tft.println("    Y :        mm");
  tft.setCursor(10,70);
  tft.println("Theta :        deg");
  tft.setCursor(10,90);
  tft.println("Lidar : ");
  tft.setCursor(10,110);
  tft.println(" Init : ");
  tft.setCursor(10,130);
  tft.println("Tirette - ");
  tft.setCursor(10,300);
  tft.println("Upload : ");
  tft.drawFastHLine(10, 155, 220, ILI9341_WHITE);
  tft.setTextSize(4);
  tft.setCursor(10,170);
  tft.println("Team");
}

void updateAllStartVar() {
    if(firstUpdate){
        updateStrategyState(getStrategy());
        updateTeamColor(getTeamColor());
        updateLidarState(getLidarState());
        firstUpdate = false;
    } 
    else 
    {
        if(strategyHasChanged()) updateStrategyState(getStrategy());
        if(teamHasChanged()) updateTeamColor(getTeamColor());
        if(lidarHasChanged()) updateLidarState(getLidarState());
        //updatePosition(1780.0, 1250.0, 180.0);
        //updateInitState(2);
    }
}

void updateTeamColor(bool team)
{
  tft.fillRect(10, 200, 220, 98, ILI9341_BLACK);
  tft.setTextSize(4);
  tft.setCursor(60,230);
  tft.setTextColor(ILI9341_WHITE);
  if(team)
  {
    tft.fillRoundRect(10, 213, 220, 67, 20, ILI9341_GREEN);
    tft.fillRoundRect(15, 218, 210, 57, 15, ILI9341_BLACK);
    tft.println("Green");
  }
  else
  {
    tft.fillRoundRect(10, 213, 220, 67, 20, ILI9341_BLUE);
    tft.fillRoundRect(15, 218, 210, 57, 15, ILI9341_BLACK);
    tft.println("Blue");
  }
  
}

void updateInitState(int initState) {
  tft.fillRect(100, 110, 140, 15, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(100, 110);
  switch (initState) {
    case 0:
      tft.setTextColor(ILI9341_RED);
      tft.println("No init !");
      break;
    case 1:
      tft.setTextColor(ILI9341_BLUE);
      tft.println("Init ...");
      break;
    case 2:
      tft.setTextColor(ILI9341_GREEN);
      tft.println("Init done !");
      break;
    default:
      tft.setTextColor(ILI9341_RED);
      tft.println("Unknow init");
  }
}

void drawBackScreenMatch() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_ORANGE);
  tft.setTextSize(2);
  // Texte "Etats"
  tft.setCursor(10, 10);
  tft.println("Strat : ");
  tft.setCursor(10, 30);
  tft.println("    X :        mm");
  tft.setCursor(10, 50);
  tft.println("    Y :        mm");
  tft.setCursor(10, 70);
  tft.println("Theta :        deg");
  tft.setCursor(10, 90);
  tft.println("Lidar : ");
  tft.drawFastHLine(10, 112, 220, ILI9341_WHITE);
  // Texte "Temps"
  tft.setCursor(10, 120);
  tft.println("Temps Restant : ");
  tft.setCursor(125, 172);
  tft.println("Sec.");
  // Texte "Score"
  tft.setCursor(10, 190);
  tft.println("Score :");
  // Texte "Points"
  tft.setCursor(150, 300);
  tft.println("points");
}

void updateAllMatchVar() {
  updateMatchTime(100);
  updateScore(9);
  updatePosition(1780.0, 1250.0, 180.0);
  updateLidarState(LIDAR_STATE);
  updateStrategyState(STRATEGY);
}

void updateStrategyState(bool stratState) {
  tft.fillRect(100, 10, 140, 15, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(100, 10);
  if(stratState == CAKE_FIRST) tft.println("CAKE");
  else if(stratState == CHERRY_FIRST) tft.println("CHERRY");
}

void updateLidarState(bool lidarState) {
  tft.fillRect(100, 90, 140, 15, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(100, 90);

  if (lidarState == LIDAR_ERROR) {
    tft.setTextColor(ILI9341_RED);
    tft.println("Waiting...");
  } else {
    tft.setTextColor(ILI9341_GREEN);
    tft.println("Connected");
  }
}

void updatePosition(float Xpos, float Ypos, float Tpos) {
  tft.fillRect(100, 30, 82, 55, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(100, 30);
  tft.println(Xpos);
  tft.setCursor(100, 50);
  tft.println(Ypos);
  tft.setCursor(100, 70);
  tft.println(Tpos);
}

void updateMatchTime(int tMatch) {
  tft.fillRect(10, 140, 110, 45, ILI9341_BLACK);
  tft.setCursor(10, 142);
  tft.setTextSize(6);
  // Change color of the timer
  if (tMatch < 10) {
    tft.setTextColor(ILI9341_RED);
  } else if (tMatch < 50) {
    tft.setTextColor(ILI9341_ORANGE);
  } else {
    tft.setTextColor(ILI9341_GREEN);
  }
  // Add "0"
  if (tMatch < 100) tft.print("0");
  if (tMatch < 10) tft.print("0");
  // Print time
  tft.println(tMatch);
}

void updateScore(int uScore) {
  tft.fillRect(10, 210, 220, 85, ILI9341_BLACK);
  tft.setCursor(10, 210);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(12);
  if (uScore < 100) tft.print("0");
  if (uScore < 10) tft.print("0");
  tft.println(uScore);
}


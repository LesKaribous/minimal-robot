#ifndef UI_H
#define UI_H

#define TIRETTE_UNARMED 0
#define TIRETTE_ARMED 1
#define TIRETTE_GO 2
#define NO_INIT 0
#define PENDING_INIT 1
#define DONE_INIT 2

const bool
    ADVERSAIRE_NON  = 1,
    ADVERSAIRE_OUI  = 0,
    CAKE_FIRST      = 1, // Stratégie gateau en premier
    CHERRY_FIRST    = 0, // Stratégie cerise en premier
    PRIMARY   = 1,
    SECONDARY = 0,
    LIDAR_ERROR = 0,
    LIDAR_CONNECTED = 1,

    GREEN   = true,
    BLUE    = false;

// Fonctions d'init et reset
void init_tft();
void restartDisplay();
bool checkRestartRequest();

// Fonctions de récupération des données
bool getRobot();
bool getTeamColor();
bool getStrategy();
bool getLidarState();
int getInitState();
int getTiretteState();

void setRobot(bool state);
void setTeamColor(bool state);
void setStrategy(bool state);
void setLidarState(bool state);
void setInitState(int state);
void setTiretteState(int state);

bool teamHasChanged();
bool strategyHasChanged();
bool lidarHasChanged();
bool tiretteHasChanged();
bool initHasChanged();
bool initHasPressed();

// Fonctions d'affichage
void drawBackScreenStart();
void drawBackScreenMatch();
void updateScore(int uScore);
void updateMatchTime(int tMatch);
void updatePosition(float Xpos, float Ypos, float Tpos);
void updateLidarState(bool lidarState);
void updateTiretteState(int tiretteState);
void updateStrategyState(bool stratState);
void updateAllMatchVar();
void updateInitState(int initState);
void updateTeamColor(bool team);
void updateAllStartVar();

#endif // UI_H

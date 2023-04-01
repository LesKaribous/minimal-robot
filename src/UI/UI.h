#ifndef UI_H
#define UI_H

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


// Fonction de récupération des données
bool getRobot();
bool getTeamColor();
bool getStrategy();
bool getLidarState();

void setRobot(bool state);
void setTeamColor(bool state);
void setStrategy(bool state);
void setLidarState(bool state);

bool teamHasChanged();
bool strategyHasChanged();
bool lidarHasChanged();

// Fonctions d'affichage
void init_tft();
void drawBackScreenStart();
void drawBackScreenMatch();
void updateScore(int uScore);
void updateMatchTime(int tMatch);
void updatePosition(float Xpos, float Ypos, float Tpos);
void updateLidarState(bool lidarState);
void updateStrategyState(bool stratState);
void updateAllMatchVar();
void updateInitState(int initState);
void updateTeamColor(bool team);
void updateAllStartVar();

#endif // UI_H

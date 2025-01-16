#include <Arduino.h>

byte xPosition = 0;
byte yPosition = 0;

enum Mode{
    FORWARD,
    BACKWARD,
    LEFT_DIRECTION,
    RIGHT_DIRECTION
};

Mode currentMode = FORWARD;

char maps[5][5];
Mode mapsOrientation[5][5];
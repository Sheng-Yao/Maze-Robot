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

String maps[5][5] = {
    {"0","0","0","0","0"},
    {"0","0","0","0","0"},
    {"0","0","0","0","0"},
    {"0","0","0","0","0"},
    {"0","0","0","0","0"}
};
Mode mapsOrientation[5][5];

void printMaps(){
    Serial.println();
    for(int j = 4; j >= 0; j--){
        Serial.print("[");
        for(byte i = 0; i < 5; i++){
            Serial.print(maps[i][j]);
            if(i != 4){
                Serial.print(",");
            }
        }
        Serial.print("]");
        Serial.println();
    }
    Serial.println();
}
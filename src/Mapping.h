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

const byte PUZZLE_X = 5;
const byte PUZZLE_Y = 5;

String maps[PUZZLE_X][PUZZLE_Y] = {
    {"0","0","0","0","0"},
    {"0","0","0","0","0"},
    {"0","0","0","0","0"},
    {"0","0","0","0","0"},
    {"0","0","0","0","0"}
};
Mode mapsOrientation[PUZZLE_X][PUZZLE_Y];

void printMaps(){
    Serial.println();
    for(int j = PUZZLE_Y - 1; j >= 0; j--){
        Serial.print("[");
        for(byte i = 0; i < PUZZLE_X; i++){
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
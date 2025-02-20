#include <Arduino.h>

// Car position variable selections (Can be view based on fixed positions)
enum Mode{
    FORWARD,
    BACKWARD,
    LEFT_DIRECTION,
    RIGHT_DIRECTION
};

// Current car orientation; initial = FORWARD
Mode currentMode = FORWARD;// initial = FORWARD

// Current car position; initial at (0,0)
byte xPosition = 0;
byte yPosition = 0;

// Maze sizing (x,y)
const byte PUZZLE_X = 4;
const byte PUZZLE_Y = 4;

String maps[PUZZLE_X][PUZZLE_Y] = {
    {"0","0","0","0"},
    {"0","0","0","0"},
    {"0","0","0","0"},
    {"0","0","0","0"}
};

// Print the 3x3 matrix maps (ease of trobleshooting)
void printMaps(){
    Serial.println();
    // Print line by line (y-axis)
    for(int j = PUZZLE_Y - 1; j >= 0; j--){
        Serial.print("[");
        // Print elements in a line (x-axis)
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

String finalSolution = "";
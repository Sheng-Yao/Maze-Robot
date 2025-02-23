#include <Arduino.h>

// Car position values (Can be view based on fixed positions)
enum Mode{
    FORWARD,
    BACKWARD,
    LEFT_DIRECTION,
    RIGHT_DIRECTION
};

// Current car orientation; initial = FORWARD
Mode currentMode = FORWARD;

// Current car position; initial at (0,0)
byte xPosition = 0;
byte yPosition = 0;

// Blocks in each side
const byte side = 4;

// Maze sizing (x,y)
const byte PUZZLE_X = side;
const byte PUZZLE_Y = side;

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
            if(i != side - 1){
                Serial.print(",");
            }
        }
        Serial.print("]");
        Serial.println();
    }
    Serial.println();
}

// String message that used in solving maze in second part of the program
String finalSolution = "";
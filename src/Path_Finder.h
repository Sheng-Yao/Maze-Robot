#include <Arduino.h>
#include <Mapping.h>
#include <EEPROM_Fun.h>

// Initial point of obtaining the solution
byte xPos = 0;
byte yPos = 0;
// Initial orientation
Mode solvingMode = FORWARD;

String maze_Solving(String str[PUZZLE_X][PUZZLE_Y]){
    // Reset variable data
    finalSolution = "";

    Serial.println();
    Serial.print("In Progress of Solving the maze...");

    while(true){
        Serial.print(".");
        delay(50);
        if(str[xPos][yPos] == "F"){
            if(solvingMode == FORWARD){
                yPos++;
                solvingMode = FORWARD;
            }else if(solvingMode == BACKWARD){
                yPos--;
                solvingMode = BACKWARD;
            }else if(solvingMode == LEFT_DIRECTION){
                xPos--;
                solvingMode = LEFT_DIRECTION;
            }else if(solvingMode == RIGHT_DIRECTION){
                xPos++;
                solvingMode = RIGHT_DIRECTION;
            }
            finalSolution.concat("F");
            continue;
        }else if(str[xPos][yPos] == "L"){
            if(solvingMode == FORWARD){
                xPos--;
                solvingMode = LEFT_DIRECTION;
            }else if(solvingMode == BACKWARD){
                xPos++;
                solvingMode = RIGHT_DIRECTION;
            }else if(solvingMode == LEFT_DIRECTION){
                yPos--;
                solvingMode = BACKWARD;
            }else if(solvingMode == RIGHT_DIRECTION){
                yPos++;
                solvingMode = FORWARD;
            }
            finalSolution.concat("L");
            continue;
        }else if(str[xPos][yPos] == "R"){
            if(solvingMode == FORWARD){
                xPos++;
                solvingMode = RIGHT_DIRECTION;
            }else if(solvingMode == BACKWARD){
                xPos--;
                solvingMode = LEFT_DIRECTION;
            }else if(solvingMode == LEFT_DIRECTION){
                yPos++;
                solvingMode = FORWARD;
            }else if(solvingMode == RIGHT_DIRECTION){
                yPos--;
                solvingMode = BACKWARD;
            }
            finalSolution.concat("R");
            continue;
        }
        // 2 branches (Need to eliminate to 1)
        else if(str[xPos][yPos] == "LR"){
            if(solvingMode == FORWARD){
                // Check if Left branch is not discovered or is not a solution
                if(str[xPos - 1][yPos] == "0" || str[xPos - 1][yPos] == "X"){
                    str[xPos][yPos] = "R";
                    xPos++;
                    solvingMode = RIGHT_DIRECTION;
                    finalSolution.concat("R");
                }
                // Check if right branch is not discovered or is not a solution
                else if(str[xPos + 1][yPos] == "0" || str[xPos + 1][yPos] == "X"){
                    str[xPos][yPos] = "L";
                    xPos--;
                    solvingMode = LEFT_DIRECTION;
                    finalSolution.concat("L");
                }
            }else if(solvingMode == BACKWARD){
                // Check if Right branch is not discovered or is not a solution
                if(str[xPos - 1][yPos] == "0" || str[xPos - 1][yPos] == "X"){
                    str[xPos][yPos] = "L";
                    xPos--;
                    solvingMode = LEFT_DIRECTION;
                    finalSolution.concat("L");
                }
                // Check if Left branch is not discovered or is not a solution
                else if(str[xPos + 1][yPos] == "0" || str[xPos + 1][yPos] == "X"){
                    str[xPos][yPos] = "R";
                    xPos++;
                    solvingMode = RIGHT_DIRECTION;
                    finalSolution.concat("R");
                }
            }else if(solvingMode == LEFT_DIRECTION){
                // Check if Left branch is not discovered or is not a solution
                if(str[xPos][yPos - 1] == "0" || str[xPos][yPos - 1] == "X"){
                    str[xPos][yPos] = "R";
                    yPos++;
                    solvingMode = FORWARD;
                    finalSolution.concat("R");
                }
                // Check if right branch is not discovered or is not a solution
                else if(str[xPos][yPos + 1] == "0" || str[xPos][yPos + 1] == "X"){
                    str[xPos][yPos] = "L";
                    yPos--;
                    solvingMode = BACKWARD;
                    finalSolution.concat("L");
                }
            }else if(solvingMode == RIGHT_DIRECTION){
                // Check if Left branch is not discovered or is not a solution
                if(str[xPos][yPos + 1] == "0" || str[xPos][yPos + 1] == "X"){
                    str[xPos][yPos] = "R";
                    yPos--;
                    solvingMode = BACKWARD;
                    finalSolution.concat("R");
                }
                // Check if right branch is not discovered or is not a solution
                else if(str[xPos][yPos - 1] == "0" || str[xPos][yPos - 1] == "X"){
                    str[xPos][yPos] = "L";
                    yPos++;
                    solvingMode = FORWARD;
                    finalSolution.concat("L");
                }
            }
            continue;
        }else if(str[xPos][yPos] == "FL"){
            if(solvingMode == FORWARD){
                // Check if Left branch is not discovered or is not a solution
                if(str[xPos][yPos + 1] == "0" || str[xPos][yPos + 1] == "X"){
                    str[xPos][yPos] = "L";
                    xPos--;
                    solvingMode = LEFT_DIRECTION;
                    finalSolution.concat("L");
                }
                // Check if right branch is not discovered or is not a solution
                else if(str[xPos - 1][yPos] == "0" || str[xPos - 1][yPos] == "X"){
                    str[xPos][yPos] = "F";
                    yPos++;
                    solvingMode = FORWARD;
                    finalSolution.concat("F");
                }
            }else if(solvingMode == BACKWARD){
                // Check if Right branch is not discovered or is not a solution
                if(str[xPos][yPos - 1] == "0" || str[xPos][yPos - 1] == "X"){
                    str[xPos][yPos] = "L";
                    xPos++;
                    solvingMode = RIGHT_DIRECTION;
                    finalSolution.concat("L");
                }
                // Check if Left branch is not discovered or is not a solution
                else if(str[xPos + 1][yPos] == "0" || str[xPos + 1][yPos] == "X"){
                    str[xPos][yPos] = "F";
                    yPos--;
                    solvingMode = BACKWARD;
                    finalSolution.concat("F");
                }
            }else if(solvingMode == LEFT_DIRECTION){
                // Check if Left branch is not discovered or is not a solution
                if(str[xPos - 1][yPos] == "0" || str[xPos - 1][yPos] == "X"){
                    str[xPos][yPos] = "L";
                    yPos--;
                    solvingMode = BACKWARD;
                    finalSolution.concat("L");
                }
                // Check if right branch is not discovered or is not a solution
                else if(str[xPos][yPos - 1] == "0" || str[xPos][yPos - 1] == "X"){
                    str[xPos][yPos] = "F";
                    xPos--;
                    solvingMode = LEFT_DIRECTION;
                    finalSolution.concat("F");
                }
            }else if(solvingMode == RIGHT_DIRECTION){
                // Check if Left branch is not discovered or is not a solution
                if(str[xPos + 1][yPos] == "0" || str[xPos + 1][yPos] == "X"){
                    str[xPos][yPos] = "L";
                    yPos++;
                    solvingMode = FORWARD;
                    finalSolution.concat("L");
                }
                // Check if right branch is not discovered or is not a solution
                else if(str[xPos][yPos + 1] == "0" || str[xPos][yPos + 1] == "X"){
                    str[xPos][yPos] = "F";
                    xPos++;
                    solvingMode = RIGHT_DIRECTION;
                    finalSolution.concat("F");
                }
            }
            continue;
        }else if(str[xPos][yPos] == "FR"){
            if(solvingMode == FORWARD){
                // Check if Left branch is not discovered or is not a solution
                if(str[xPos][yPos + 1] == "0" || str[xPos][yPos + 1] == "X"){
                    str[xPos][yPos] = "R";
                    xPos++;
                    solvingMode = RIGHT_DIRECTION;
                    finalSolution.concat("R");
                }
                // Check if right branch is not discovered or is not a solution
                else if(str[xPos + 1][yPos] == "0" || str[xPos + 1][yPos] == "X"){
                    str[xPos][yPos] = "F";
                    yPos++;
                    solvingMode = FORWARD;
                    finalSolution.concat("F");
                }
            }else if(solvingMode == BACKWARD){
                // Check if Right branch is not discovered or is not a solution
                if(str[xPos][yPos - 1] == "0" || str[xPos][yPos - 1] == "X"){
                    str[xPos][yPos] = "R";
                    xPos--;
                    solvingMode = LEFT_DIRECTION;
                    finalSolution.concat("R");
                }
                // Check if Left branch is not discovered or is not a solution
                else if(str[xPos - 1][yPos] == "0" || str[xPos - 1][yPos] == "X"){
                    str[xPos][yPos] = "F";
                    yPos--;
                    solvingMode = BACKWARD;
                    finalSolution.concat("F");
                }
            }else if(solvingMode == LEFT_DIRECTION){
                // Check if Left branch is not discovered or is not a solution
                if(str[xPos - 1][yPos] == "0" || str[xPos - 1][yPos] == "X"){
                    str[xPos][yPos] = "R";
                    yPos++;
                    solvingMode = FORWARD;
                    finalSolution.concat("R");
                }
                // Check if right branch is not discovered or is not a solution
                else if(str[xPos][yPos + 1] == "0" || str[xPos][yPos + 1] == "X"){
                    str[xPos][yPos] = "F";
                    xPos--;
                    solvingMode = LEFT_DIRECTION;
                    finalSolution.concat("F");
                }
            }else if(solvingMode == RIGHT_DIRECTION){
                // Check if Left branch is not discovered or is not a solution
                if(str[xPos + 1][yPos] == "0" || str[xPos + 1][yPos] == "X"){
                    str[xPos][yPos] = "R";
                    yPos--;
                    solvingMode = BACKWARD;
                    finalSolution.concat("R");
                }
                // Check if right branch is not discovered or is not a solution
                else if(str[xPos][yPos - 1] == "0" || str[xPos][yPos - 1] == "X"){
                    str[xPos][yPos] = "F";
                    xPos++;
                    solvingMode = RIGHT_DIRECTION;
                    finalSolution.concat("F");
                }
            }
            continue;
        }else if(str[xPos][yPos] == "E"){
            memoryReset();
            memoryWrite(finalSolution);
            Serial.println();
            break;
        }
    }
    return finalSolution;
}
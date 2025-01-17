#include <Motor.h>
#include <Mapping.h>

void setup(){

  // Initialize serial monitor
  Serial.begin(9600);
  
  encoderSetup();
  
  motorSetup();

  mpuSetup();

  ultrasonicSetup();

  Serial.println("Done setup..");

  delay(1000);
}

bool isMoving = false;


const byte mazeWidth = 10;

unsigned long current = millis();


void loop(){
  if(getDistance(FRONT) > 2){
    if(!isMoving){
      if(isTurnLeft || isTurnRight || isUTurn){
        if(isTurnRight){
          turnRight();
          moveForwardAfterTurn();
          isTurnRight = false;
        }else if(isTurnLeft){
          turnLeft();
          moveForwardAfterTurn();
          isTurnLeft = false;
        }else if(isUTurn){
          uTurn();
          moveForwardAfterTurn();
          isUTurn = false;
        }
      }
      else{
        moveForward();
        isMoving = true;
        resetDistance();
      }
    }else{
      float ultrasonicResult = getDistance(FRONT);
      if((getMovingDistance() > oneBlockSize + 0.5 && (int(ultrasonicResult) % 20 > 6 && int(ultrasonicResult) % 20 < 9) ) || ultrasonicResult < 8){
        // Serial.print(getMovingDistance());
        // Serial.print("  " + String(int(ultrasonicResult) % 20));
        // Serial.print("  " + String(ultrasonicResult));
        // Serial.println();
        stop();
        isMoving = false;
        isReachPoint = true;
      }else{
        //Moving forward + Align
        if(millis() - current > 25){
          distance[0] = getDistance(LEFT);
          distance[1] = getDistance(RIGHT);
          float distanceDifference = distance[0] - distance[1];
          if(distanceDifference <= -2){
            if(distanceDifference <= -15.0){
              if(distance[0] < 6){
                alignRight();
              }else if(distance[0] > 8 && distance[0] < 12){
                alignLeft();
              }else{
                moveForward();
              }
            }else{
              if(distance[0] > mazeWidth + 10 && distance[1] > mazeWidth + 10){
                moveForward();
              }else{
                alignRight();
              }
            }
          }else if(distanceDifference >= 2){
            if(distanceDifference >= 15.0){
              if(distance[1] < 6){
                alignLeft();
              }else if(distance[1] > 8 && distance[1] < 12){
                alignRight();
              }else{
                moveForward();
              }
            }else{
              if(distance[0] > mazeWidth + 10 && distance[1] > mazeWidth + 10){
                update();
                if(angle < targetAngle - 4){
                  alignLeft();
                }else if(angle > targetAngle + 4){
                  alignRight();
                }else{
                  moveForwardSlow();
                }
              }else{
                alignLeft();
              }

            }
          }
          else{
            moveForward();
          }
          current = millis();
        }
      }
    }
  }else{
    Serial.println(String(pulsesLeft - pulsesRight));
    Serial.println(String(pulsesLeft + pulsesRight));
    Serial.println(String(getMovingDistance()));
    stop();
    while(1){;}
  }

  if(isReachPoint){

    delay(500);

    if(maps[xPosition][yPosition] == "0"){

      distance[0] = getDistance(LEFT);
      distance[1] = getDistance(RIGHT);
      distance[2] = getDistance(FRONT);

      if(distance[0] < mazeWidth && distance[1] < mazeWidth && distance[2] < mazeWidth){
        Serial.println("Dead End (X)");
        moveCloseToWall();
        isUTurn = true;
        isTurnLeft = false;
        isTurnRight = false;
        maps[xPosition][yPosition] = 'X';
        Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
        if(currentMode == FORWARD){
          currentMode = BACKWARD;
          yPosition--;
        }else if(currentMode == BACKWARD){
          currentMode = FORWARD;
          yPosition++;
        }else if(currentMode == LEFT_DIRECTION){
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }else if(currentMode == RIGHT_DIRECTION){
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }
      }else if(distance[0] < mazeWidth && distance[1] < mazeWidth && distance[2] > mazeWidth){
        Serial.println("Front (F)");
        isTurnLeft = false;
        isTurnRight = false;
        isUTurn = false;
        maps[xPosition][yPosition] = 'F';
        Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
        if(currentMode == FORWARD){
          yPosition++;
        }else if(currentMode == BACKWARD){
          yPosition--;
        }else if(currentMode == LEFT_DIRECTION){
          xPosition--;
        }else if(currentMode == RIGHT_DIRECTION){
          xPosition++;
        }
      }else if(distance[0] > mazeWidth && distance[1] < mazeWidth && distance[2] < mazeWidth){
        Serial.println("Left (L)");
        moveCloseToWall();
        isTurnLeft = true;
        isTurnRight = false;
        isUTurn = false;
        maps[xPosition][yPosition] = 'L';
        Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
        if(currentMode == FORWARD){
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }else if(currentMode == BACKWARD){
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }else if(currentMode == LEFT_DIRECTION){
          currentMode = BACKWARD;
          yPosition--;
        }else if(currentMode == RIGHT_DIRECTION){
          currentMode = FORWARD;
          yPosition++;
        }
      }else if(distance[0] < mazeWidth && distance[1] > mazeWidth && distance[2] < mazeWidth){
        Serial.println("Right (R)");
        moveCloseToWall();
        isTurnRight = true;
        isTurnLeft = false;
        isUTurn = false;
        maps[xPosition][yPosition] = 'R';
        Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
        if(currentMode == FORWARD){
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }else if(currentMode == BACKWARD){
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }else if(currentMode == LEFT_DIRECTION){
          currentMode = FORWARD;
          yPosition++;
        }else if(currentMode == RIGHT_DIRECTION){
          currentMode = BACKWARD;
          yPosition--;
        }
      }else if(distance[0] > mazeWidth && distance[1] > mazeWidth && distance[2] < mazeWidth){
        Serial.println("Left and Right (LR)");
        moveCloseToWall();
        isTurnLeft = true;
        isTurnRight = false;
        isUTurn = false;
        maps[xPosition][yPosition] = "LR";
        Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
        if(currentMode == FORWARD){
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }else if(currentMode == BACKWARD){
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }else if(currentMode == LEFT_DIRECTION){
          currentMode = BACKWARD;
          yPosition--;
        }else if(currentMode == RIGHT_DIRECTION){
          currentMode = FORWARD;
          yPosition++;
        }
      }else if(distance[0] > mazeWidth && distance[1] < mazeWidth && distance[2] > mazeWidth){
        Serial.println("Front and Left (FL)");
        isTurnLeft = false;
        isTurnRight = false;
        isUTurn = false;
        maps[xPosition][yPosition] = "FL";
        Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
        if(currentMode == FORWARD){
          currentMode = FORWARD;
          yPosition++;
        }else if(currentMode == BACKWARD){
          currentMode = BACKWARD;
          yPosition--;
        }else if(currentMode == LEFT_DIRECTION){
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }else if(currentMode == RIGHT_DIRECTION){
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }
      }else if(distance[0] < mazeWidth && distance[1] > mazeWidth && distance[2] > mazeWidth){
        Serial.println("Front and Right (FR)");
        isTurnLeft = false;
        isTurnRight = false;
        isUTurn = false;
        maps[xPosition][yPosition] = "FR";
        Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
        if(currentMode == FORWARD){
          currentMode = FORWARD;
          yPosition++;
        }else if(currentMode == BACKWARD){
          currentMode = BACKWARD;
          yPosition--;
        }else if(currentMode == LEFT_DIRECTION){
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }else if(currentMode == RIGHT_DIRECTION){
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }
      }
      // else if(distance[0] > mazeWidth && distance[1] > mazeWidth && distance[2] > mazeWidth){
      //   Serial.println("Front, Left and Right branches");
      //   isTurnLeft = false;
      //   isTurnRight = false;
      //   isUTurn = false;
      //   maps[xPosition][yPosition] = "FLR";
      //   Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
      //   if(currentMode == FORWARD){
      //     currentMode = FORWARD;
      //     yPosition++;
      //   }else if(currentMode == BACKWARD){
      //     currentMode = BACKWARD;
      //     yPosition--;
      //   }else if(currentMode == LEFT_DIRECTION){
      //     currentMode = LEFT_DIRECTION;
      //     xPosition--;
      //   }else if(currentMode == RIGHT_DIRECTION){
      //     currentMode = RIGHT_DIRECTION;
      //     xPosition++;
      //   }
      // }
    }else{
      if(maps[xPosition][yPosition] == "F"){
        Serial.println("Front (F) [Written]");
        isTurnLeft = false;
        isTurnRight = false;
        isUTurn = false;
        if(currentMode == FORWARD){
          if(maps[xPosition][yPosition - 1] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          currentMode = FORWARD;
          yPosition++;
        }else if(currentMode == BACKWARD){
          if(maps[xPosition][yPosition + 1] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          currentMode = BACKWARD;
          yPosition--;
        }else if(currentMode == LEFT_DIRECTION){
          if(maps[xPosition + 1][yPosition] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }else if(currentMode == RIGHT_DIRECTION){
          if(maps[xPosition - 1][yPosition] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }
      }else if(maps[xPosition][yPosition] == "L"){
        Serial.println("Left (L) [Written]");
        moveCloseToWall();
        if(currentMode == FORWARD){
          if(maps[xPosition][yPosition - 1] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          isTurnLeft = true;
          isTurnRight = false;
          isUTurn = false;
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }else if(currentMode == BACKWARD){
          if(maps[xPosition][yPosition + 1] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          isTurnRight = true;
          isTurnLeft = false;
          isUTurn = false;
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }else if(currentMode == LEFT_DIRECTION){
          if(maps[xPosition + 1][yPosition] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          isTurnLeft = false;
          isTurnRight = false;
          isUTurn = false;
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }else if(currentMode == RIGHT_DIRECTION){
          if(maps[xPosition - 1][yPosition] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          isTurnRight = true;
          isTurnLeft = false;
          isUTurn = false;
          currentMode = BACKWARD;
          yPosition--;
        }
      }else if(maps[xPosition][yPosition] == "R"){
        Serial.println("Right (R) [Written]");
        if(currentMode == FORWARD){
          if(maps[xPosition][yPosition - 1] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          isTurnRight = false;
          isTurnLeft = true;
          isUTurn = false;
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }else if(currentMode == BACKWARD){
          if(maps[xPosition][yPosition + 1] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          isTurnLeft = true;
          isTurnRight = false;
          isUTurn = false;
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }else if(currentMode == RIGHT_DIRECTION){
          if(maps[xPosition - 1][yPosition] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          isTurnLeft = true;
          isTurnRight = false;
          isUTurn = false;
          currentMode = FORWARD;
          yPosition++;
        }else if(currentMode == LEFT_DIRECTION){
          if(maps[xPosition + 1][yPosition] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          isTurnLeft = true;
          isTurnRight = false;
          isUTurn = false;
          currentMode = BACKWARD;
          yPosition--;
        }
      }else if(maps[xPosition][yPosition] == "LR"){
        Serial.println("Left and Right (LR) [Written]");
        if(currentMode == BACKWARD){
          if(maps[xPosition][yPosition + 1] == "X"){
            maps[xPosition][yPosition] = "R";
          }
          isTurnRight = false;
          isTurnLeft = false;
          isUTurn = false;
          currentMode = BACKWARD;
          yPosition--;
        }else if(currentMode == RIGHT_DIRECTION){
          if(maps[xPosition - 1][yPosition] == "X"){
            maps[xPosition][yPosition] = "R";
          }
          isTurnLeft = false;
          isTurnRight = false;
          isUTurn = false;
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }
      }else if(maps[xPosition][yPosition] == "FL"){
        Serial.println("Front and Left (FL) [Written]");
        if(currentMode == BACKWARD){
          if(maps[xPosition][yPosition - 1] == "X"){
            maps[xPosition][yPosition] = "L";
          }
          isTurnRight = true;
          isTurnLeft = false;
          isUTurn = false;
          currentMode = LEFT_DIRECTION;
          xPosition--;
        }
      }else if(maps[xPosition][yPosition] == "FR"){
        Serial.println("Front and Right (FR) [Written]");
        if(currentMode == BACKWARD){
          if(maps[xPosition][yPosition - 1] == "X"){
            maps[xPosition][yPosition] = "R";
          }
          isTurnLeft = true;
          isTurnRight = false;
          isUTurn = false;
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }
      }
      // }else if(maps[xPosition][yPosition] == "FLR"){
      //   if(currentMode == BACKWARD){
      //     if(maps[xPosition][yPosition - 1] == "X"){
      //       maps[xPosition][yPosition] = "LR";
      //     }
      //     isTurnRight = true;
      //     isTurnLeft = false;
      //     isUTurn = false;
      //     currentMode = LEFT_DIRECTION;
      //     xPosition--;
      //   }
      // }
    }

    printMaps();

    isMoving = false;
    
    isReachPoint = false;

    delay(2500);

    currentTime = millis();

    // ending point
    if(xPosition == PUZZLE_X - 1 && yPosition == PUZZLE_Y - 1){
      Serial.println("Reached End Point");
      while(1){;}
    }

    if(xPosition < 0 || yPosition < 0 || xPosition == PUZZLE_X || yPosition == PUZZLE_Y){
      Serial.println("Array out of range.");
      while(1){;}
    }
  }
}
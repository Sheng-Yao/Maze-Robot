#include <Motor.h>
#include <Path_Finder.h>

void setup(){

  // Initialize serial monitor
  Serial.begin(115200);
  
  encoderSetup();
  motorSetup();
  mpuSetup();
  ultrasonicSetup();

  currentTime = millis();

  Serial.println("Done setup..");

  delay(1000);

  while(1){;}
}


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
      if((getMovingDistance() > 30 && int(ultrasonicResult) % 27 <= 4) || ultrasonicResult < 6){
        Serial.print(getMovingDistance());
        Serial.print("  " + String(int(ultrasonicResult) % 27));
        Serial.print("  " + String(ultrasonicResult));
        Serial.println();
        stop();
        isMoving = false;
        isReachPoint = true;
      }else{
        //Moving forward + Align
        if(millis() - current > 30){
          distance[0] = getDistance(LEFT);
          distance[1] = getDistance(RIGHT);

          Serial.print("FRONT: " + String(ultrasonicResult) + " LEFT: " + String(distance[0]) + " RIGHT: " + String(distance[1]) + " ");

          if(distance[0] < mazeWidth && distance[1] < mazeWidth){
            if(distance[0] - distance[1] < -3){
              Serial.println("Align Right");
              alignRight();
            }else if(distance[0] - distance[1] > 3){
              Serial.println("Align Left");
              alignLeft();
            }else{
              Serial.println("Move Forward");
              moveForward();
            }
          }else if(distance[0] < mazeWidth && distance[1] > mazeWidth){
            if(distance[0] < 6){
              Serial.println("Align Right");
              alignRight();
            }else if(distance[0] > 8){
              Serial.println("Align Left");
              alignLeft();
            }else{
              Serial.println("Move Forward");
              moveForward();
            }
          }else if(distance[0] > mazeWidth && distance[1] < mazeWidth){
            if(distance[1] < 6){
              Serial.println("Align Left");
              alignLeft();
            }else if(distance[1] > 8){
              Serial.println("Align Right");
              alignRight();
            }else{
              Serial.println("Move Forward");
              moveForward();
            }
          }else{
            Serial.println("Enter 2");
            update();
            Serial.print(String(angle) + " ");
            if(angle < 0){
              if(angle - targetAngle < -10){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                  Serial.println("Align Left");
                  alignLeft();
              }else if(angle - targetAngle > 10){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                  Serial.println("Align Right");
                  alignRight();
              }else{
                  Serial.println("Move Forward");
                  moveForward();
              }
            }else{
              if(angle - targetAngle > 5){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                  Serial.println("Align Right");
                  alignRight();
              }else if(angle - targetAngle  < -5){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                  Serial.println("Align Left");
                  alignLeft();
              }else{
                  Serial.println("Move Forward");
                  moveForward();
              }
            }
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

    // Check whether reach ending point but let the robot move until the ending point
    bool isEnded = false;
    if(xPosition == PUZZLE_X - 1 && yPosition == PUZZLE_Y - 1){
      isEnded = true;
    }

    delay(250);

    if(maps[xPosition][yPosition] == "0"){

      distance[0] = getDistance(LEFT);
      distance[1] = getDistance(RIGHT);
      distance[2] = getDistance(FRONT);

      if(distance[0] < mazeWidth && distance[1] < mazeWidth && distance[2] < mazeWidth){// Dead End (X)
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
      }else if(distance[0] < mazeWidth && distance[1] < mazeWidth && distance[2] > mazeWidth){// Front (F)
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
      }else if(distance[0] > mazeWidth && distance[1] < mazeWidth && distance[2] < mazeWidth){// Left (L)
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
      }else if(distance[0] < mazeWidth && distance[1] > mazeWidth && distance[2] < mazeWidth){// Right (R)
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
      }else if(distance[0] > mazeWidth && distance[1] > mazeWidth && distance[2] < mazeWidth){// Left and Right (LR)
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
      }else if(distance[0] > mazeWidth && distance[1] < mazeWidth && distance[2] > mazeWidth){// Front and Left (FL)
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
      }else if(distance[0] < mazeWidth && distance[1] > mazeWidth && distance[2] > mazeWidth){// Front and Right (FR)
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
    }
    // Have value (Been here before)
    else{
      Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
      if(maps[xPosition][yPosition] == "F"){// Front (F) [Written]
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
      }else if(maps[xPosition][yPosition] == "L"){// Left (L) [Written]
        Serial.println("Left (L) [Written]");
        moveCloseToWall();
        if(currentMode == FORWARD){
          if(maps[xPosition][yPosition - 1] == "X"){
            maps[xPosition][yPosition] = "X";
          }
          isTurnLeft = false;
          isTurnRight = true;
          isUTurn = false;
          currentMode = RIGHT_DIRECTION;
          xPosition++;
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
      }else if(maps[xPosition][yPosition] == "R"){// Right (R) [Written]
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
      }else if(maps[xPosition][yPosition] == "LR"){// Left and Right (LR) [Written]
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
      }else if(maps[xPosition][yPosition] == "FL"){// Front and Left (FL) [Written]
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
        }else if(currentMode == LEFT_DIRECTION){
          if(maps[xPosition + 1][yPosition] == "X"){
            maps[xPosition][yPosition] = "L";
          }
          isTurnRight = true;
          isTurnLeft = false;
          isUTurn = false;
          currentMode = FORWARD;
          yPosition++;
        }else if(currentMode == RIGHT_DIRECTION){
          if(maps[xPosition - 1][yPosition] == "X"){
            maps[xPosition][yPosition] = "L";
          }
          isTurnRight = true;
          isTurnLeft = false;
          isUTurn = false;
          currentMode = BACKWARD;
          yPosition--;
        }
      }else if(maps[xPosition][yPosition] == "FR"){// Front and Right (FR) [Written]
        Serial.println("Front and Right (FR) [Written]");
        if(currentMode == BACKWARD){
          if(maps[xPosition][yPosition + 1] == "X"){
            maps[xPosition][yPosition] = "R";
          }
          isTurnLeft = true;
          isTurnRight = false;
          isUTurn = false;
          currentMode = RIGHT_DIRECTION;
          xPosition++;
        }else if(currentMode == LEFT_DIRECTION){
          if(maps[xPosition + 1][yPosition] == "X"){
            maps[xPosition][yPosition] = "R";
          }
          isTurnLeft = true;
          isTurnRight = false;
          isUTurn = false;
          currentMode = BACKWARD;
          yPosition--;
        }
      }
    }


    // ending point
    if(isEnded){
      maps[PUZZLE_X - 1][PUZZLE_Y - 1] = "E";
      Serial.println("Reached End Point");
      Serial.println(maze_Solving(maps));
      while(1){;}
    }

    if(xPosition < 0 || yPosition < 0 || xPosition == PUZZLE_X || yPosition == PUZZLE_Y){
      Serial.println("Array out of range.");
      while(1){;}
    }

    
    printMaps();

    isMoving = false;
    isReachPoint = false;

    delay(500);

    currentTime = millis();
    
  }
}
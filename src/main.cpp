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
        Serial.print(getMovingDistance());
        Serial.print("  " + String(int(ultrasonicResult) % 20));
        Serial.print("  " + String(ultrasonicResult));
        Serial.println();
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
              alignRight();
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
              alignLeft();
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

    // update();
    // if(angle < targetAngle - 4 || angle > targetAngle + 4){
    //   while(true){
    //     if(angle < targetAngle - 4){
    //       alignLeft();
    //     }else if(angle > targetAngle + 4){
    //       alignRight();
    //     }else{
    //       stop();
    //       break;
    //     }
    //   }
    // }

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
      isReturning = true;
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
      if(isReturning){
        maps[xPosition][yPosition] = 'X';
      }else{
        maps[xPosition][yPosition] = 'F';
      }
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
      Serial.println("Left branch (F)");
      moveCloseToWall();
      isTurnLeft = true;
      isTurnRight = false;
      isUTurn = false;
      if(isReturning){
        maps[xPosition][yPosition] = 'X';
      }else{
        maps[xPosition][yPosition] = 'L';       
      }
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
      Serial.println("Right branch (E)");
      moveCloseToWall();
      isTurnRight = true;
      isTurnLeft = false;
      isUTurn = false;
      if(isReturning){
        maps[xPosition][yPosition] = 'X';
      }else{
        maps[xPosition][yPosition] = 'R';
      }
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
      Serial.println("Left and Right branches");
      moveCloseToWall();
      if(maps[xPosition][yPosition] == "0"){
        maps[xPosition][yPosition] = "LR";
        isReturning = false;
        isTurnLeft = true;
        isTurnRight = false;
        isUTurn = false;
      }else{
        byte tempXPos1, tempYPos1;
        if(currentMode == FORWARD){
          tempXPos1 = xPosition - 1;
          tempYPos1 = yPosition;
          if(maps[tempXPos1][tempYPos1] == "0" && maps[tempXPos1 + 2][tempYPos1] == "X"){
            if(maps[tempXPos1][tempYPos1 - 1] == "X"){
              maps[xPosition][yPosition] = 'X';
              isReturning = true;
            }else{
              maps[xPosition][yPosition] = 'L';
              isReturning = false;
            }
            isTurnLeft = true;
            isTurnRight = false;
            isUTurn = false;
          }else if(maps[tempXPos1][tempYPos1] == "X" && maps[tempXPos1 + 2][tempYPos1] == "0"){
            if(maps[tempXPos1][tempYPos1 - 1] == "X"){
              maps[xPosition][yPosition] = 'X';
              isReturning = true;
            }else{
              maps[xPosition][yPosition] = 'R';
              isReturning = false;
            }
            isTurnRight = true;
            isTurnLeft = false;
            isUTurn = false;
          }
        }else if(currentMode == BACKWARD){
          tempXPos1 = xPosition + 1;
          tempYPos1 = yPosition;
          if(maps[tempXPos1][tempYPos1] == "0" && maps[tempXPos1 - 2][tempYPos1] == "X"){
            if(maps[tempXPos1][tempYPos1 + 1] == "X"){
              maps[xPosition][yPosition] = 'X';
              isReturning = true;
            }else{
              maps[xPosition][yPosition] = 'L';
              isReturning = false;
            }
            isTurnLeft = true;
            isTurnRight = false;
            isUTurn = false;
          }else if(maps[tempXPos1][tempYPos1] == "X" && maps[tempXPos1 - 2][tempYPos1] == "0"){
            if(maps[tempXPos1][tempYPos1 + 1] == "X"){
              maps[xPosition][yPosition] = 'X';
              isReturning = true;
            }else{
              maps[xPosition][yPosition] = 'R';
              isReturning = false;
            }
            isTurnRight = true;
            isTurnLeft = false;
            isUTurn = false;
          }
        }else if(currentMode == LEFT_DIRECTION){
          tempXPos1 = xPosition;
          tempYPos1 = yPosition - 1;
          if(maps[tempXPos1][tempYPos1] == "0" && maps[tempXPos1][tempYPos1 + 2] == "X"){
            if(maps[tempXPos1 + 1][tempYPos1] == "X"){
              maps[xPosition][yPosition] = 'X';
              isReturning = true;
            }else{
              maps[xPosition][yPosition] = 'L';
              isReturning = false;
            }
            isTurnLeft = true;
            isTurnRight = false;
            isUTurn = false;
          }else if(maps[tempXPos1][tempYPos1] == "X" && maps[tempXPos1][tempYPos1 + 2] == "0"){
            if(maps[tempXPos1 + 1][tempYPos1] == "X"){
              maps[xPosition][yPosition] = 'X';
              isReturning = true;
            }else{
              maps[xPosition][yPosition] = 'R';
              isReturning = false;
            }
            isTurnRight = true;
            isTurnLeft = false;
            isUTurn = false;
          }
        }else if(currentMode == RIGHT_DIRECTION){
          tempXPos1 = xPosition;
          tempYPos1 = yPosition + 1;
          if(maps[tempXPos1][tempYPos1] == "0" && maps[tempXPos1][tempYPos1 - 2] == "X"){
            if(maps[tempXPos1 - 1][tempYPos1] == "X"){
              maps[xPosition][yPosition] = 'X';
              isReturning = true;
            }else{
              maps[xPosition][yPosition] = 'L';
              isReturning = false;
            }
            isTurnLeft = true;
            isTurnRight = false;
            isUTurn = false;
          }else if(maps[tempXPos1][tempYPos1] == "X" && maps[tempXPos1][tempYPos1 - 2] == "0"){
            if(maps[tempXPos1 - 1][tempYPos1] == "X"){
              maps[xPosition][yPosition] = 'X';
              isReturning = true;
            }else{
              maps[xPosition][yPosition] = 'R';
              isReturning = false;
            }
            isTurnRight = true;
            isTurnLeft = false;
            isUTurn = false;
          }
        }
      }
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
      // turnLeft();
    }else if(distance[0] > mazeWidth && distance[1] < mazeWidth && distance[2] > mazeWidth){
      Serial.println("Left and front branches (D)");
      isTurnLeft = false;
      isTurnRight = false;
      isUTurn = false;
      // turnLeft();
    }else if(distance[0] < mazeWidth && distance[1] > mazeWidth && distance[2] > mazeWidth){
      Serial.println("Right and front branches (B)");
      isTurnLeft = false;
      isTurnRight = false;
      isUTurn = false;
      // turnRight();
    }else if(distance[0] > mazeWidth && distance[1] > mazeWidth && distance[2] > mazeWidth){
      Serial.println("Left, Right and front branches");
      isTurnLeft = false;
      isTurnRight = false;
      isUTurn = false;
      // turnLeft();
      // Possible reach the end point need to add in with end point validation
    }

    printMaps();

    isMoving = false;
    
    isReachPoint = false;

    delay(2500);

    currentTime = millis();
  }
}
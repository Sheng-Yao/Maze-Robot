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

float distance[3] = {0,0,0};

const byte mazeWidth = 10;

unsigned long current = millis();

void loop(){
  if(getDistance(FRONT) > 3){
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
        }
      }else{
        moveForward();
        isMoving = true;
        resetDistance();
      }
    }else{
      float ultrasonicResult = getDistance(FRONT);
      if((getMovingDistance() >= 25) && (round(ultrasonicResult) % 23 <= 2 || ultrasonicResult <= 7)){
        stop();
        isMoving = false;
        isReachPoint = true;
      }else{
        // Moving forward + Align
        if(millis() - current > 25){
          distance[0] = getDistance(LEFT);
          distance[1] = getDistance(RIGHT);
          float distanceDifference = distance[0] - distance[1];
          if(distanceDifference <= -2){
            if(distanceDifference <= -15.0){
              if(distance[0] < 5.5){
                alignRight();
              }else if(distance[0] > 7){
                alignLeft();
              }else{
                moveForward();
              }
            }else{
              alignRight();
            }
          }else if(distanceDifference >= 2){
            if(distanceDifference >= 15.0){
              if(distance[1] < 5.5){
                alignLeft();
              }else if(distance[1] > 7){
                alignRight();
              }else{
                moveForward();
              }
            }else{
              alignLeft();
            }
          }
          else{
            attachInterrupt(digitalPinToInterrupt(encoderPinA), counterLeftUpdate, RISING);
            attachInterrupt(digitalPinToInterrupt(encoderPinB), counterRightUpdate, RISING);
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

    delay(100);

    distance[0] = getDistance(LEFT);
    distance[1] = getDistance(RIGHT);
    distance[2] = getDistance(FRONT);

    if(distance[0] < mazeWidth && distance[1] < mazeWidth && distance[2] < mazeWidth){
      Serial.println("Dead End (G)");
      moveCloseToWall();
      isUTurn = true;
      isTurnLeft = false;
      isTurnRight = false;
      // Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
      // if(currentMode == FORWARD){
      //   currentMode = BACKWARD;
      // }else if(currentMode == BACKWARD){
      //   currentMode = FORWARD;
      // }else if(currentMode == LEFT_DIRECTION){
      //   currentMode = RIGHT_DIRECTION;
      // }else if(currentMode == RIGHT_DIRECTION){
      //   currentMode = LEFT_DIRECTION;
      // }
    }else if(distance[0] < mazeWidth && distance[1] < mazeWidth && distance[2] > mazeWidth){
      Serial.println("Front (C)");
      isTurnLeft = false;
      isTurnRight = false;
      isUTurn = false;
      // Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
      // if(currentMode == FORWARD){
      //   yPosition++;
      // }else if(currentMode == BACKWARD){
      //   yPosition--;
      // }else if(currentMode == LEFT_DIRECTION){
      //   xPosition--;
      // }else if(currentMode == RIGHT_DIRECTION){
      //   xPosition++;
      // }
    }else if(distance[0] > mazeWidth && distance[1] < mazeWidth && distance[2] < mazeWidth){
      Serial.println("Left branch (F)");
      moveCloseToWall();
      isTurnLeft = true;
      isTurnRight = false;
      isUTurn = false;
      // Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
      // if(currentMode == FORWARD){
      //   currentMode = LEFT_DIRECTION;
      //   xPosition--;
      // }else if(currentMode == BACKWARD){
      //   currentMode = RIGHT_DIRECTION;
      //   xPosition++;
      // }else if(currentMode == LEFT_DIRECTION){
      //   currentMode = BACKWARD;
      //   yPosition--;
      // }else if(currentMode == RIGHT_DIRECTION){
      //   currentMode = FORWARD;
      //   yPosition++;
      // }
    }else if(distance[0] < mazeWidth && distance[1] > mazeWidth && distance[2] < mazeWidth){
      Serial.println("Right branch (E)");
      moveCloseToWall();
      isTurnRight = true;
      isTurnLeft = false;
      isUTurn = false;
      // Serial.println("(" + String(xPosition) + "," + String(yPosition) + ")");
      // if(currentMode == FORWARD){
      //   currentMode = RIGHT_DIRECTION;
      //   xPosition++;
      // }else if(currentMode == BACKWARD){
      //   currentMode = LEFT_DIRECTION;
      //   xPosition--;
      // }else if(currentMode == LEFT_DIRECTION){
      //   currentMode = FORWARD;
      //   yPosition++;
      // }else if(currentMode == RIGHT_DIRECTION){
      //   currentMode = BACKWARD;
      //   yPosition--;
      // }
    }else if(distance[0] > mazeWidth && distance[1] > mazeWidth && distance[2] < mazeWidth){
      Serial.println("Left and Right branches");
      moveCloseToWall();
      isTurnLeft = true;
      isTurnRight = false;
      isUTurn = false;
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

    isMoving = false;
    
    isReachPoint = false;

    delay(2500);

    currentTime = millis();
  }
}
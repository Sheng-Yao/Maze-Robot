#include <Motor.h>
#include <Ultrasonic.h>

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
bool isReachPoint = false;

float distance[3] = {0,0,0};

const byte mazeWidth = 15;

unsigned long current = millis();

void loop(){
  if(getDistance(frontTrig,frontEcho) > 5){
    if(!isMoving){
      if(isTurnLeft || isTurnRight || isUTurn){
        if(isTurnRight){
          turnRight();
          while(true){
            update();
            if(isTurnRight){
              if(abs(angle) < targetAngle){
                continue;
              }else{
                moveForward();
                resetDistance();
                detachInterrupt(digitalPinToInterrupt(encoderPinA));
                detachInterrupt(digitalPinToInterrupt(encoderPinB));
                attachInterrupt(digitalPinToInterrupt(encoderPinA), counterLeftUpdate, RISING);
                attachInterrupt(digitalPinToInterrupt(encoderPinB), counterRightUpdate, RISING);
                while(true){
                  float result = getMovingDistance();
                  Serial.println(result);
                  if(result < 25){
                    continue;
                  }else{
                    stop();
                    break;
                  }
                }
                isReachPoint = true;
                isTurnRight = false;
                break;
              }
            }
          }
          delay(5000);
        }

      }else{
        moveForward();
        isMoving = true;
        resetDistance();
      }
    }else{
      float ultrasonicResult = getDistance(frontTrig,frontEcho);
      if((getMovingDistance() >= 25) && (round(ultrasonicResult) % 25 <= 3 || ultrasonicResult <= 7)){
        stop();
        isMoving = false;
        isReachPoint = true;
      }else{
        // Moving forward + Align
        if(millis() - current > 75){
          distance[0] = getDistance(leftTrig,leftEcho);
          distance[1] = getDistance(rightTrig,rightEcho);
          float distanceDifference = distance[0] - distance[1];
          if(distanceDifference <= -2.5){
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

          }else if(distanceDifference >= 2.5){
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

    distance[0] = getDistance(leftTrig,leftEcho);
    distance[1] = getDistance(rightTrig,rightEcho);
    distance[2] = getDistance(frontTrig,frontEcho);

    Serial.println("Distance (Left): " + String(distance[0]));

    delay(5);

    Serial.println("Distance (Right): " + String(distance[1]));

    delay(5);

    Serial.println("Distance (Front): " + String(distance[2]));

    if(distance[0] < mazeWidth && distance[1] < mazeWidth && distance[2] < mazeWidth){
      Serial.println("Dead End");
      isUTurn = true;
      isTurnLeft = false;
      isTurnRight = false;
    }else if(distance[0] < mazeWidth && distance[1] < mazeWidth && distance[2] > mazeWidth){
      Serial.println("Front");
      isTurnLeft = false;
      isTurnRight = false;
      isUTurn = false;
    }else if(distance[0] > mazeWidth && distance[1] < mazeWidth && distance[2] < mazeWidth){
      Serial.println("Left branch");
      moveForward();
      while(true){
        if(getDistance(frontTrig,frontEcho) < 7){
          stop();
          break;
        }else{
          delay(50);
          continue;
        }
      }
      isTurnLeft = true;
      isTurnRight = false;
      isUTurn = false;
    }else if(distance[0] < mazeWidth && distance[1] > mazeWidth && distance[2] < mazeWidth){
      Serial.println("Right branch");
      moveForward();
      while(true){
        if(getDistance(frontTrig,frontEcho) < 7){
          stop();
          break;
        }else{
          delay(50);
          continue;
        }
      }
      isTurnRight = true;
      isTurnLeft = false;
      isUTurn = false;
    }else if(distance[0] > mazeWidth && distance[1] > mazeWidth && distance[2] < mazeWidth){
      Serial.println("Left and Right branches");
      //
    }else if(distance[0] > mazeWidth && distance[1] < mazeWidth && distance[2] > mazeWidth){
      Serial.println("Left and front branches");
      //
    }else if(distance[0] < mazeWidth && distance[1] > mazeWidth && distance[2] > mazeWidth){
      Serial.println("Right and front branches");
      // turnRight();
    }else if(distance[0] > mazeWidth && distance[1] > mazeWidth && distance[2] > mazeWidth){
      Serial.println("Left, Right and front branches");
      // turnLeft(); // Possible reach the end point need to add in with end point validation
    }
    
    isReachPoint = false;

    delay(5000);

    currentTime = millis();
  }
}
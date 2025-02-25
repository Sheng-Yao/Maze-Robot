#include <Arduino.h>
#include <math.h>
#include <MPU6050.h>
#include <Encoder.h>
#include <Ultrasonic.h>
#include <MotorBasic.h>

bool isInterruptOn = false;

// Motor movement control
void moveForward(){
    if(!isInterruptOn){
        interrupts();
        isInterruptOn = true;
    }
    analogWrite(motor1Speed,equilibriumSpeed);
    analogWrite(motor2Speed,equilibriumSpeed);
    goForwardMotor1();
    goForwardMotor2();
}

// Motor movement control
void moveForwardSlow(){
    if(!isInterruptOn){
        interrupts();
        isInterruptOn = true;
    }
    analogWrite(motor1Speed,equilibriumSpeed-15);
    analogWrite(motor2Speed,equilibriumSpeed-15);
    goForwardMotor1();
    goForwardMotor2();
}

void stop(){
    analogWrite(motor1Speed,equilibriumSpeed);
    analogWrite(motor2Speed,equilibriumSpeed);
    resetMotor1();
    resetMotor2();
}

void alignLeft(){
    stop();
    if(isInterruptOn){
        noInterrupts();
        isInterruptOn = false;
    }
    leftSpeedVal = equilibriumSpeed;
    rightSpeedVal = equilibriumSpeed;
    analogWrite(motor1Speed,rightSpeedVal);
    analogWrite(motor2Speed,leftSpeedVal);
    goForwardMotor1();
    goBackwardMotor2();
}


void alignRight(){
    stop();
    if(isInterruptOn){
        noInterrupts();
        isInterruptOn = false;
    }
    leftSpeedVal = equilibriumSpeed;
    rightSpeedVal = equilibriumSpeed;
    analogWrite(motor1Speed,rightSpeedVal);
    analogWrite(motor2Speed,leftSpeedVal);
    goBackwardMotor1();
    goForwardMotor2();
}


bool isTurnLeft = false;
bool isTurnRight = false;
bool isUTurn = false;

void turnLeft(){
    // update();
    analogWrite(motor1Speed,turningSpeed);
    analogWrite(motor2Speed,turningSpeed);
    goForwardMotor1();
    goBackwardMotor2();
    targetAngle = angle + 90;

}

void turnRight(){
    // update();
    analogWrite(motor1Speed,turningSpeed);
    analogWrite(motor2Speed,turningSpeed);
    goBackwardMotor1();
    goForwardMotor2();
    targetAngle = angle - 90;
}

void uTurn(){
    // update();
    analogWrite(motor1Speed,turningSpeed);
    analogWrite(motor2Speed,turningSpeed);
    goForwardMotor1();
    goBackwardMotor2();
    targetAngle = angle + 180;
}

void moveCloseToWall(){
    while(true){
        if(getDistance(FRONT) <= 5.5){
            stop();
            break;
        }else{
            moveForwardSlow();
            continue;
        }
    }
}

const byte mazeWidth = 20;

float distance[3] = {0,0,0};

unsigned long current = millis();

bool isMoving = false;

void moveForwardAfterTurn(){

    current = millis();

    while(true){
        update();
        float requiredAngle;
        if(isTurnRight){
            requiredAngle = targetAngle - angle;
        }else if(isTurnLeft || isUTurn){
            requiredAngle = angle - targetAngle;
        }

        if(requiredAngle <= 0){
            continue;
        }else{
            Serial.println(targetAngle);
            stop();
            resetDistance();
            while(getMovingDistance() < 15){
                if(millis()-current > 30){
                    if(getMovingDistance() < 15){
                        update();
                        Serial.print(String(angle) + " ");
                        if(angle < 0){
                            if(angle < targetAngle - 5){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                                Serial.println("Align Left");
                                alignLeft();
                            }else if(angle > targetAngle + 5){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                                Serial.println("Align Right");
                                alignRight();
                            }else{
                                Serial.println("Move Forward");
                                moveForward();
                            }
                        }else{
                            if(angle < targetAngle + 5){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                                Serial.println("Align Left");
                                alignLeft();
                            }else if(angle > targetAngle - 5){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                                Serial.println("Align Right");
                                alignRight();
                            }else{
                                Serial.println("Move Forward");
                                moveForward();
                            }
                        }
                        current = millis();
                    }else{
                        break;
                    }
                }
            }
            current = millis();
            while(true){
                float ultrasonicResult = getDistance(FRONT);
                if((getMovingDistance() > 30 && int(ultrasonicResult) % 27 <= 4) || ultrasonicResult < 5.5){
                  Serial.print(getMovingDistance());
                  Serial.print("  " + String(int(ultrasonicResult) % 27));
                  Serial.print("  " + String(ultrasonicResult));
                  Serial.println();
                  stop();
                  isMoving = false;
                  isReachPoint = true;
                  break;
                }else{
                  //Moving forward + Align
                  if(millis() - current > 25){
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
                        if(distance[0] < 5){
                          Serial.println("Align Right");
                          alignRight();
                        }else if(distance[0] > 6.5){
                          Serial.println("Align Left");
                          alignLeft();
                        }else{
                          Serial.println("Move Forward");
                          moveForward();
                        }
                      }else if(distance[0] > mazeWidth && distance[1] < mazeWidth){
                        if(distance[1] < 5){
                          Serial.println("Align Left");
                          alignLeft();
                        }else if(distance[1] > 6.5){
                          Serial.println("Align Right");
                          alignRight();
                        }else{
                          Serial.println("Move Forward");
                          moveForward();
                        }
                      }else{
                        Serial.println("Enter 1");
                        update();
                        if(angle < 0){
                            if(angle < targetAngle - 5){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                                Serial.println("Align Left");
                                alignLeft();
                            }else if(angle > targetAngle + 5){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                                Serial.println("Align Right");
                                alignRight();
                            }else{
                                Serial.println("Move Forward");
                                moveForward();
                            }
                        }else{
                            if(angle < targetAngle + 8){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                                Serial.println("Align Left");
                                alignLeft();
                            }else if(angle > targetAngle - 8){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                                Serial.println("Align Right");
                                alignRight();
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
            isReachPoint = true;
            break;
        }
    }
}
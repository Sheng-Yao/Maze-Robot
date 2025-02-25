#include <Arduino.h>
#include <Motor_Basic.h>
#include <math.h>
#include <Ultrasonic.h>
<<<<<<< HEAD
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
    leftSpeedVal = turningSpeed-25;
    rightSpeedVal = turningSpeed-25;
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
    leftSpeedVal = turningSpeed-25;
    rightSpeedVal = turningSpeed-25;
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

=======
#include <Encoder.h>
#include <MPU6050.h>

byte checkingDuration = 25;

// Ensure robot car move to checking point in each block
>>>>>>> d09afb02918754351a041ace247adcbf11a228af
void moveCloseToWall(){
    unsigned long previous = millis();
    while(true){
<<<<<<< HEAD
        if(getDistance(FRONT) <= 5.5){
            stop();
            break;
        }else{
            moveForwardSlow();
            continue;
=======
        if(millis() - previous >= checkingDuration){
            float frontDistance = getDistance(FRONT);
            if(int(frontDistance) % 28 <= 4 || frontDistance <= 5){
                stop();
                break;
            }else{
                moveForward();
                continue;
            }
            previous = millis();
>>>>>>> d09afb02918754351a041ace247adcbf11a228af
        }
    }
}

<<<<<<< HEAD
const byte mazeWidth = 20;
=======
void moveForwardWithAlignment();
>>>>>>> d09afb02918754351a041ace247adcbf11a228af

void turnAndMoveForward(){

    unsigned long previous = millis();

<<<<<<< HEAD
unsigned long current = millis();

bool isMoving = false;

void moveForwardAfterTurn(){

    current = millis();

=======
>>>>>>> d09afb02918754351a041ace247adcbf11a228af
    while(true){
        // Update the robot car angle
        update();

        float requiredAngle;
        if(isTurnRight){
            requiredAngle = targetAngle - angle;
        }else if(isTurnLeft || isUTurn){
            requiredAngle = angle - targetAngle;
        }
<<<<<<< HEAD

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
                        if(angle < 0){
                            if(angle < targetAngle - 5){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                                Serial.print("Align Left");
                                alignLeft();
                            }else if(angle > targetAngle + 5){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                                Serial.print("Align Right");
                                alignRight();
                            }else{
                                Serial.print("Move Forward");
                                moveForward();
                            }
                        }else{
                            if(angle < targetAngle + 5){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                                Serial.print("Align Left");
                                alignLeft();
                            }else if(angle > targetAngle - 5){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                                Serial.print("Align Right");
                                alignRight();
                            }else{
                                Serial.print("Move Forward");
                                moveForward();
                            }
                        }
                        Serial.println(angle);
                        current = millis();
                    }else{
                        break;
                    }
                }
            }
            stop();
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
          
                    Serial.println("FRONT: " + String(ultrasonicResult) + " LEFT: " + String(distance[0]) + " RIGHT: " + String(distance[1]));
          
                    if(distance[0] < mazeWidth && distance[1] < mazeWidth){
                    //   if(distance[0] < 5){
                    //     alignRight();
                    //   }else if(distance[0] > 6){
                    //     alignLeft();
                    //   }else{
                    //     moveForward();
                    //   }
                        if(distance[0] < distance[1] + 3.5){
                            alignRight();
                        }else if(distance[0] > distance[1] + 3.5){
                            alignLeft();
                        }else{
                            moveForward();
                        }
                    }else if(distance[0] < mazeWidth && distance[1] > mazeWidth){
                      if(distance[0] < 5){
                        alignRight();
                      }else if(distance[0] > 7){
                        alignLeft();
                      }else{
                        moveForward();
                      }
                    }else if(distance[0] > mazeWidth && distance[1] < mazeWidth){
                      if(distance[1] < 5){
                        alignRight();
                      }else if(distance[1] > 7){
                        alignLeft();
                      }else{
                        moveForward();
                      }
                    }else{
                      moveForward();
                      // update();
                      // if(angle < targetAngle - 5){
                      //     alignLeft();
                      // }else if(angle > targetAngle + 5){
                      //     alignRight();
                      // }else{
                      //     moveForward();
                      // }
                    }
                    current = millis();
                  }
=======
        Serial.println("targetAngle:" + String(targetAngle) + "angle:" + String(angle) + " requiredAngle:"+String(requiredAngle));

        if(millis() - previous >= checkingDuration){
            if(requiredAngle < 0){
                continue;
            }else{
                // Done turning
                stop();
                resetDistance();

                moveForwardWithAlignment();
            }
            previous = millis();
        }
    }
}

const float ultrasonicMazeSize = 20;
const float encoderMazeSize = 35;

const float lowerAlignmentValue = 4.5;
const float upperAlignmentValue = 6.5;
const float alignmentValueDifference = 1;

bool isReachPoint = false;

void moveForwardWithAlignment(){
    
    unsigned long previous = millis();

    while(true){

        if(millis() - previous <= checkingDuration){
            
            float distance[3] = {0,0,0};
            distance[0] = getDistance(LEFT);
            distance[1] = getDistance(RIGHT);
            distance[2] = getDistance(FRONT);

            if(distance[2] > 5 || (int(distance[2]) % 28 >= 4 && getMovingDistance() < encoderMazeSize)){
                // If no branch at left and right
                if(distance[0] < ultrasonicMazeSize && distance[1] < ultrasonicMazeSize){
                    if(distance[0] > distance[1] + alignmentValueDifference){
                        alignLeft();
                    }else if(distance[1] > distance[0] + alignmentValueDifference){
                        alignRight();
                    }else{
                        moveForward();
                    }
                }else if(distance[0] > ultrasonicMazeSize && distance[1] < ultrasonicMazeSize){// When has left branch
                    if(distance[1] < lowerAlignmentValue){
                        alignLeft();
                    }else if(distance[1] > upperAlignmentValue){
                        alignRight();
                    }else{
                        moveForward();
                    }
                }else if(distance[0] < ultrasonicMazeSize && distance[1] > ultrasonicMazeSize){// WHen has right branch
                    if(distance[0] < lowerAlignmentValue){
                        alignRight();
                    }else if(distance[0] > upperAlignmentValue){
                        alignLeft();
                    }else{
                        moveForward();
                    }
                }else if(distance[0] > ultrasonicMazeSize && distance[1] > ultrasonicMazeSize){// when left and right has branch
                    // Update the robot car angle
                    update();

                    float requiredAngle;
                    if(isTurnRight){
                        requiredAngle = targetAngle - angle;
                    }else if(isTurnLeft || isUTurn){
                        requiredAngle = angle - targetAngle;
                    }

                    if(requiredAngle < 0){
                        if(angle < targetAngle){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                            alignLeft();
                        }else if(angle > targetAngle){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                            alignRight();
                        }
                    }else{
                        moveForward();
                    }
>>>>>>> d09afb02918754351a041ace247adcbf11a228af
                }
            }else{
                stop();
                resetDistance();
                isReachPoint = true;
                break;
                Serial.println("Detection location reached.");
            }
            previous = millis();
        }
    }
}
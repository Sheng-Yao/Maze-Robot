#include <Arduino.h>
#include <Motor_Basic.h>
#include <math.h>
#include <MPU6050.h>
#include <Encoder.h>
#include <Ultrasonic.h>

// Motor speed control
const byte maxSpeed = 255;
const byte equilibriumSpeed = 95; //rough estimate of PWM at the speed pin of the stronger motor, while driving straight // 155
const byte turningSpeed = 125; //125

int leftSpeedVal;
int rightSpeedVal;

bool isReachPoint = false;

void motorSetup(){
    pinMode(motor1Speed, OUTPUT);
    pinMode(motor2Speed, OUTPUT);
    pinMode(motor1A, OUTPUT);
    pinMode(motor1B, OUTPUT);
    pinMode(motor2A, OUTPUT);
    pinMode(motor2B, OUTPUT);
    leftSpeedVal = equilibriumSpeed;
    rightSpeedVal = equilibriumSpeed;
    analogWrite(motor1Speed,rightSpeedVal);
    analogWrite(motor2Speed,leftSpeedVal);
    resetMotor1();
    resetMotor2();
    currentTime = millis();
}

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
    analogWrite(motor1Speed,equilibriumSpeed-10);
    analogWrite(motor2Speed,equilibriumSpeed-10);
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
    noInterrupts();
    isInterruptOn = false;
    leftSpeedVal = turningSpeed;
    rightSpeedVal = turningSpeed;
    analogWrite(motor1Speed,rightSpeedVal);
    analogWrite(motor2Speed,leftSpeedVal);
    goForwardMotor1();
    goBackwardMotor2();
}


void alignRight(){
    stop();
    noInterrupts();
    isInterruptOn = false;
    leftSpeedVal = turningSpeed;
    rightSpeedVal = turningSpeed;
    analogWrite(motor1Speed,rightSpeedVal);
    analogWrite(motor2Speed,leftSpeedVal);
    // resetMotor1();
    goBackwardMotor1();
    goForwardMotor2();
}


bool isTurnLeft = false;
bool isTurnRight = false;
bool isUTurn = false;
void turnLeft(){
    analogWrite(motor1Speed,turningSpeed);
    analogWrite(motor2Speed,turningSpeed);
    goForwardMotor1();
    goBackwardMotor2();
    targetAngle = angle + 90;
}
void turnRight(){
    analogWrite(motor1Speed,turningSpeed);
    analogWrite(motor2Speed,turningSpeed);
    goBackwardMotor1();
    goForwardMotor2();
    targetAngle = angle - 90;
}
void uTurn(){
    analogWrite(motor1Speed,turningSpeed);
    analogWrite(motor2Speed,turningSpeed);
    goForwardMotor1();
    goBackwardMotor2();
    targetAngle = angle + 185;
}



void moveCloseToWall(){
    while(true){
        if(getDistance(FRONT) <= 5.75){
            stop();
            break;
        }else{
            moveForwardSlow();
            continue;
        }
    }
}

const float oneBlockSize = 35;

float distance[3] = {0,0,0};

void moveForwardAfterTurn(){
    while(true){
        update();
        float requiredAngle;
        if(isTurnRight){
            requiredAngle = targetAngle - angle;
        }else if(isTurnLeft || isUTurn){
            requiredAngle = angle - targetAngle;
        }
        // Serial.println("angle:" + String(angle) + " requiredAngle:"+String(requiredAngle));

        if(requiredAngle <= 0){
            continue;
        }else{
            Serial.println(targetAngle);
            stop();
            resetDistance();
            while(getMovingDistance() < 15){
                update();
                if(angle < targetAngle - 3){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                    alignLeft();
                }else if(angle > targetAngle + 3){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                    alignRight();
                }else{
                    moveForward();
                }
            }
            stop();
            while(true){
                float distanceResult = getDistance(FRONT);
                if((getMovingDistance() <= oneBlockSize + 10) && distanceResult > 8.5){
                    distance[0] = getDistance(LEFT);
                    distance[1] = getDistance(RIGHT);
                    if(distance[0] < 8 || distance[1] < 8){
                        if(distance[0] < 8){
                            if(distance[0] < 5.5){
                                alignRight();
                            }else if(distance[0] > 7){
                                alignLeft();
                            }else{
                                moveForward();
                            }
                        }else if(distance[1] < 8){
                            if(distance[1] < 5.5){
                                alignLeft();
                            }else if(distance[1] > 7){
                                alignRight();
                            }else{
                                moveForward();
                            }
                        }
                    }else{
                        update();
                        if(angle < targetAngle - 3){
                            alignLeft();
                        }else if(angle > targetAngle + 3){
                            alignRight();
                        }else{
                            moveForward();
                        }
                    }
                    delay(10);
                    continue;
                }else{
                    stop();
                    break;
                }
            }
            isReachPoint = true;
            break;
        }
    }
}
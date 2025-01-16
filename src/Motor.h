#include <Arduino.h>
#include <math.h>
#include <MPU6050.h>
#include <Encoder.h>
#include <Ultrasonic.h>

// Speed control pins
#define motor1Speed 10
#define motor2Speed 11

// Motor direction pins
#define motor1A A0
#define motor1B A1
#define motor2A 5
#define motor2B 4

// Motor speed control
const byte maxSpeed = 255;
const byte equilibriumSpeed = 105; //rough estimate of PWM at the speed pin of the stronger motor, while driving straight // 155
const byte turningSpeed = 75;

int leftSpeedVal;
int rightSpeedVal;

bool isReachPoint = false;

// Motor element direction control
void resetMotor1(){
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B,LOW);
}

void goForwardMotor1(){
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW);
}

void goBackwardMotor1(){
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B,HIGH);
}

void resetMotor2(){
    digitalWrite(motor2A,LOW);
    digitalWrite(motor2B,LOW);
}

void goForwardMotor2(){
    digitalWrite(motor2A,HIGH);
    digitalWrite(motor2B,LOW);
}

void goBackwardMotor2(){
    digitalWrite(motor2A,LOW);
    digitalWrite(motor2B,HIGH);
}

void motorSetup(){
    pinMode(motor1Speed, OUTPUT);
    pinMode(motor2Speed, OUTPUT);
    pinMode(motor1A, OUTPUT);
    pinMode(motor1B, OUTPUT);
    pinMode(motor2A, OUTPUT);
    pinMode(motor2B, OUTPUT);
    leftSpeedVal = equilibriumSpeed;
    rightSpeedVal = equilibriumSpeed;
    analogWrite(motor1Speed,equilibriumSpeed);
    analogWrite(motor2Speed,equilibriumSpeed);
    resetMotor1();
    resetMotor2();
    currentTime = millis();
}

// Motor movement control
void moveForward(){
    analogWrite(motor1Speed,equilibriumSpeed);
    analogWrite(motor2Speed,equilibriumSpeed);
    goForwardMotor1();
    goForwardMotor2();
}

void alignLeft(){
    detachInterrupt(digitalPinToInterrupt(encoderPinA));
    detachInterrupt(digitalPinToInterrupt(encoderPinB));
    leftSpeedVal = turningSpeed;
    rightSpeedVal = turningSpeed;
    analogWrite(motor1Speed,rightSpeedVal);
    analogWrite(motor2Speed,leftSpeedVal);
    goForwardMotor1();
    resetMotor2();
    // goBackwardMotor2();
}

void alignRight(){
    detachInterrupt(digitalPinToInterrupt(encoderPinA));
    detachInterrupt(digitalPinToInterrupt(encoderPinB));
    leftSpeedVal = turningSpeed;
    rightSpeedVal = turningSpeed;
    analogWrite(motor1Speed,rightSpeedVal);
    analogWrite(motor2Speed,leftSpeedVal);
    resetMotor1();
    //goBackwardMotor1();
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
    turnRight();
    // targetAngle = 180;
}

void stop(){
    analogWrite(motor1Speed,equilibriumSpeed);
    analogWrite(motor2Speed,equilibriumSpeed);
    resetMotor1();
    resetMotor2();
}


void moveCloseToWall(){
    moveForward();
    while(true){
        if(getDistance(FRONT) <= 5){
            stop();
            break;
        }else{
            delay(10);
            continue;
        }
    }
}

void moveForwardAfterTurn(){
    while(true){
        update();
        float requiredAngle;
        if(isTurnRight){
            requiredAngle = abs(angle) + targetAngle;
        }else if(isTurnLeft){
            requiredAngle = abs(angle) - targetAngle;
        }

        if(requiredAngle < 1){
            continue;
        }else{
            moveForward();
            resetDistance();
            detachInterrupt(digitalPinToInterrupt(encoderPinA));
            detachInterrupt(digitalPinToInterrupt(encoderPinB));
            attachInterrupt(digitalPinToInterrupt(encoderPinA), counterLeftUpdate, RISING);
            attachInterrupt(digitalPinToInterrupt(encoderPinB), counterRightUpdate, RISING);
            while(true){
                if(getMovingDistance() < 25 && getDistance(FRONT) >= 7){
                    delay(20);
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
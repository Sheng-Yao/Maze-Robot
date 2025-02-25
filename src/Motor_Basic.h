#include <Arduino.h>

// Speed control pins
#define motor1Speed 10
#define motor2Speed 11

// Motor direction pins
#define motor1A A0
#define motor1B A1
#define motor2A 5
#define motor2B 4

// Motor speed control
// const byte maxSpeed = 255;
const byte equilibriumSpeed = 255;// 105
const byte turningSpeed = 255; // 150

int leftSpeedVal;
int rightSpeedVal;

// Motor 1 movement
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


// Motor 2 movement control
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
    // Pins setup
    pinMode(motor1Speed, OUTPUT);
    pinMode(motor2Speed, OUTPUT);
    pinMode(motor1A, OUTPUT);
    pinMode(motor1B, OUTPUT);
    pinMode(motor2A, OUTPUT);
    pinMode(motor2B, OUTPUT);

    // Motor speed setup
    leftSpeedVal = equilibriumSpeed;
    rightSpeedVal = equilibriumSpeed;
    analogWrite(motor1Speed,rightSpeedVal);
    analogWrite(motor2Speed,leftSpeedVal);

    // Motor direction setup
    resetMotor1();
    resetMotor2();
}


// FLag for interrupt (Handling pulses counting)
bool isInterruptOn = false;

// Desired angle to rotate
float targetAngle = 0;

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

void turnLeft(float angle){
    analogWrite(motor1Speed,turningSpeed);
    analogWrite(motor2Speed,turningSpeed);
    goForwardMotor1();
    goBackwardMotor2();
    targetAngle = angle + 90;
}
void turnRight(float angle){
    analogWrite(motor1Speed,turningSpeed);
    analogWrite(motor2Speed,turningSpeed);
    goBackwardMotor1();
    goForwardMotor2();
    targetAngle = angle - 90;
}
void uTurn(float angle){
    analogWrite(motor1Speed,turningSpeed);
    analogWrite(motor2Speed,turningSpeed);
    goForwardMotor1();
    goBackwardMotor2();
    targetAngle = angle + 180;
}
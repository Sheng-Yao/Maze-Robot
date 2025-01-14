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
const byte slowSpeed = 200;
const byte fastSpeed = 255;

void setSlowSpeed(){
    analogWrite(motor1Speed,slowSpeed);
    analogWrite(motor2Speed,slowSpeed);
}

void setFastSpeed(){
    analogWrite(motor1Speed,fastSpeed);
    analogWrite(motor2Speed,fastSpeed);
}


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
    setSlowSpeed();
    resetMotor1();
    resetMotor2();
}

// Motor movement control
void moveForward(){
    goForwardMotor1();
    goForwardMotor2();
}

void moveBackward(){
    goBackwardMotor1();
    goBackwardMotor2();
}

void turnLeft(){
    goBackwardMotor1();
    goForwardMotor2();
}

void turnRight(){
    goForwardMotor1();
    goBackwardMotor2();
}

void stop(){
    resetMotor1();
    resetMotor2();
}
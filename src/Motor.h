#include <Arduino.h>
#include <math.h>
#include <MPU6050.h>
#include <Encoder.h>

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
const byte equilibriumSpeed = 155; //rough estimate of PWM at the speed pin of the stronger motor, while driving straight 

int leftSpeedVal;
int rightSpeedVal;

float elapsedTime, currentTime, previousTime;
float AccX, AccY; //linear acceleration
float GyroZ; //angular velocity
float yaw;

float angle = 0;
float targetAngle = 0;

bool isDriving = false; //it the car driving forward OR rotate/stationary
bool prevIsDriving = true; //equals isDriving in the previous iteration of void loop()
bool paused = false;

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
    leftSpeedVal = equilibriumSpeed;
    rightSpeedVal = equilibriumSpeed;
    analogWrite(motor1Speed,rightSpeedVal);
    analogWrite(motor2Speed,leftSpeedVal);
    goForwardMotor1();
    resetMotor2();
    // goBackwardMotor2();
}

void alignRight(){
    detachInterrupt(digitalPinToInterrupt(encoderPinA));
    detachInterrupt(digitalPinToInterrupt(encoderPinB));
    leftSpeedVal = equilibriumSpeed;
    rightSpeedVal = equilibriumSpeed;
    analogWrite(motor1Speed,rightSpeedVal);
    analogWrite(motor2Speed,leftSpeedVal);
    resetMotor1();
    //goBackwardMotor1();
    goForwardMotor2();
}

bool isTurnLeft = false;
bool isTurnRight = false;
bool isUTurn = false;

// void turnLeft(){
//     analogWrite(motor1Speed,maxSpeed);
//     analogWrite(motor2Speed,maxSpeed);
//     goBackwardMotor1();
//     goForwardMotor2();
//     // targetAngle += 90;
//     // if (targetAngle > 180){
//     //     targetAngle -= 360;
//     // }
//     // isDriving = false;
// }

void turnRight(){
    analogWrite(motor1Speed,equilibriumSpeed);
    analogWrite(motor2Speed,equilibriumSpeed);
    goBackwardMotor1();
    goForwardMotor2();
    targetAngle = 90;
}

// void uTurn(){
//     analogWrite(motor1Speed,maxSpeed);
//     analogWrite(motor2Speed,maxSpeed);
//     goBackwardMotor1();
//     goForwardMotor2();
//     // targetAngle += 180;
//     // if (targetAngle > 360){
//     //     targetAngle -= 360;
//     // }
//     // isDriving = false;
// }

void stop(){
    analogWrite(motor1Speed,equilibriumSpeed);
    analogWrite(motor2Speed,equilibriumSpeed);
    resetMotor1();
    resetMotor2();
}

void update(){
    // === Read gyroscope (on the MPU6050) data === //
    previousTime = currentTime;
    currentTime = millis();
    elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
    getOrientation();
    // Correct the outputs with the calculated error values
    gyroOutputBuffer -= GyroErrorZ;
    // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
    yaw += gyroOutputBuffer * elapsedTime;
    angle = yaw; //if you mounted MPU6050 in a different orientation to me, angle may not = roll. It can roll, pitch, yaw or minus version of the three
    //for me, turning right reduces angle. Turning left increases angle.
}
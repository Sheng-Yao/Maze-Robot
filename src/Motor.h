#include <Arduino.h>
#include <math.h>
#include <MPU6050.h>

// Speed control pins
#define motor1Speed 10
#define motor2Speed 11

// Motor direction pins
#define motor1A A0
#define motor1B A1
#define motor2A 5
#define motor2B 4

// Motor speed control
const byte slowSpeed = 160;
const byte fastSpeed = 255;
byte equilibriumSpeed = 248; //rough estimate of PWM at the speed pin of the stronger motor, while driving straight 

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
    analogWrite(motor1Speed,0);
    analogWrite(motor2Speed,0);
    resetMotor1();
    resetMotor2();
    currentTime = micros();
}

// Motor movement control
void moveForward(){
    goForwardMotor1();
    goForwardMotor2();
    isDriving = true;
}

void moveBackward(){
    goBackwardMotor1();
    goBackwardMotor2();
}

void turnLeft(){
    goBackwardMotor1();
    goForwardMotor2();
    targetAngle += 90;
    if (targetAngle > 180){
        targetAngle -= 360;
    }
    isDriving = false;
}

void turnRight(){
    goForwardMotor1();
    goBackwardMotor2();
    targetAngle -= 90;
    if (targetAngle <= -180){
        targetAngle += 360;
    }
    isDriving = false;
}

void uTurn(){
    goBackwardMotor1();
    goForwardMotor2();
    targetAngle += 180;
    if (targetAngle > 360){
        targetAngle -= 360;
    }
    isDriving = false;
}

void stop(){
    resetMotor1();
    resetMotor2();
    isDriving = false;
}

int changeSpeed (int motorSpeed, int increment){
  motorSpeed += increment;
  if (motorSpeed > fastSpeed){ //to prevent motorSpeed from exceeding 255, which is a problem when using analogWrite
    motorSpeed = fastSpeed;
  } else if (motorSpeed < slowSpeed){
    motorSpeed = slowSpeed;
  }
  return motorSpeed;
}


void rotate(){//called by void loop(), which isDriving = false
    int deltaAngle = round(targetAngle - angle);
    int targetGyroZ;
    if (abs(deltaAngle) <= 1){
        stop();
    } else {
        if (angle > targetAngle) { //turn left
            turnLeft();
        } else if (angle < targetAngle) {//turn right
            turnRight();
        }

        //setting up propoertional control, see Step 3 on the website
        if (abs(deltaAngle) > 30){
            targetGyroZ = 60;
        } else {
            targetGyroZ = 2 * abs(deltaAngle);
        }
        
        if (round(targetGyroZ - abs(gyroOutputBuffer)) == 0){
        ;
        } else if (targetGyroZ > abs(gyroOutputBuffer)){
            leftSpeedVal = changeSpeed(leftSpeedVal, +1); //would increase abs(GyroX)
        } else {
            leftSpeedVal = changeSpeed(leftSpeedVal, -1);
        }
        rightSpeedVal = leftSpeedVal;
        analogWrite(motor1Speed, leftSpeedVal);
        analogWrite(motor2Speed, rightSpeedVal);
    }
}   

void controlSpeed(){//this function is called by driving ()
    int deltaAngle = round(targetAngle - angle);
    int targetGyroZ;
    
    //setting up propoertional control, see Step 3 on the website
    if (deltaAngle > 30){
        targetGyroZ = 60;
    } else if (deltaAngle < -30){
        targetGyroZ = -60;
    } else {
        targetGyroZ = 2 * deltaAngle;
    }
    
    if (round(targetGyroZ - gyroOutputBuffer) == 0){
        ;
    } else if (targetGyroZ > gyroOutputBuffer){
        leftSpeedVal = changeSpeed(leftSpeedVal, -1); //would increase GyroX
    } else {
        leftSpeedVal = changeSpeed(leftSpeedVal, +1);
    }
}

void driving(){//called by void loop(), which isDriving = true
    int deltaAngle = round(targetAngle - angle); //rounding is neccessary, since you never get exact values in reality
    moveForward();
    if (deltaAngle != 0){
        controlSpeed();
        rightSpeedVal = fastSpeed;
        analogWrite(motor1Speed, leftSpeedVal);
        analogWrite(motor2Speed, rightSpeedVal);
    }
}

void update(){
    // === Read gyroscope (on the MPU6050) data === //
    previousTime = currentTime;
    currentTime = micros();
    elapsedTime = (currentTime - previousTime) / 1000000; // Divide by 1000 to get seconds
    getOrientation();
    // Correct the outputs with the calculated error values
    gyroOutputBuffer -= GyroErrorZ;
    // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
    yaw += gyroOutputBuffer * elapsedTime;
    angle = yaw; //if you mounted MPU6050 in a different orientation to me, angle may not = roll. It can roll, pitch, yaw or minus version of the three
    //for me, turning right reduces angle. Turning left increases angle.
}
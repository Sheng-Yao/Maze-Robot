#include <Arduino.h>

//define pin for DC motors
//Motor 1
//Enable pin (to control the speed using PWM)
#define enableMotor1 3
//Motor pin (controlling the direction)
#define motor1A 1
#define motor1B 4

//Motor 2
//Enable pin (to control the speed using PWM)
#define enableMotor2 5
//Motor pin (controlling the direction)
#define motor2A 6
#define motor2B 7

//ultrasonics sensors TRIG and ECHO pins
//define pins for left ultrasonic sensor
#define leftTrig 8
#define leftEcho 9

//define pins for front ultrasonic sensor
#define frontTrig 10
#define frontEcho 11

//define pins for right ultrasonic sensor
#define rightTrig 12
#define rightEcho 13


void moveForward(){
    //values for analogWrite is 0 ~ 255

    //let both motor runs at full speed
    analogWrite(enableMotor1,255);
    analogWrite(enableMotor2,255);

    //motor 1 moving forward
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW);

    //motor 2 moving forward
    digitalWrite(motor2A,HIGH);
    digitalWrite(motor2B,LOW);
}

void turnLeft(){
    //values for analogWrite is 0 ~ 255

    //let left motor slow and right motor fast
    analogWrite(enableMotor1,100);
    analogWrite(enableMotor2,255);

    //motor 1 moving forward
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW);

    //motor 2 moving forward
    digitalWrite(motor2A,HIGH);
    digitalWrite(motor2B,LOW);
}

void alignLeft(){
    //values for analogWrite is 0 ~ 255

    //let left motor slow and right motor fast
    analogWrite(enableMotor1,200);
    analogWrite(enableMotor2,255);

    //motor 1 moving forward
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW);

    //motor 2 moving forward
    digitalWrite(motor2A,HIGH);
    digitalWrite(motor2B,LOW);
}

void turnRight(){
    //values for analogWrite is 0 ~ 255

    //let right motor slow and left motor fast
    analogWrite(enableMotor1,255);
    analogWrite(enableMotor2,100);

    //motor 1 moving forward
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW);

    //motor 2 moving forward
    digitalWrite(motor2A,HIGH);
    digitalWrite(motor2B,LOW);
}

void alignRight(){
    //values for analogWrite is 0 ~ 255

    //let right motor slow and left motor fast
    analogWrite(enableMotor1,255);
    analogWrite(enableMotor2,200);

    //motor 1 moving forward
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW);

    //motor 2 moving forward
    digitalWrite(motor2A,HIGH);
    digitalWrite(motor2B,LOW);
}

void stop(){
    //values for analogWrite is 0 ~ 255

    //let both motor stop
    analogWrite(enableMotor1,0);
    analogWrite(enableMotor2,0);

    //motor 1 stop
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B,LOW);

    //motor 2 stop
    digitalWrite(motor2A,LOW);
    digitalWrite(motor2B,LOW);
}

void uTurn(){
    //values for analogWrite is 0 ~ 255

    //let both motor stop
    analogWrite(enableMotor1,255);
    analogWrite(enableMotor2,255);

    //motor 1 stop
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW);

    //motor 2 stop
    digitalWrite(motor2A,LOW);
    digitalWrite(motor2B,HIGH);

    //set the delay besed on requirement
    delay(500);
}

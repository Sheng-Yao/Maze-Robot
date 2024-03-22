#include <Arduino.h>

//define pin for DC motors
//Motor 1
//Enable pin (to control the speed using PWM)
#define enableMotor1 3
//Motor pin (controlling the direction)
#define motor1A A0
#define motor1B 4

//Motor 2
//Enable pin (to control the speed using PWM)
#define enableMotor2 5
//Motor pin (controlling the direction)
#define motor2A 6
#define motor2B 7

const byte motor1Speed = 110;
const byte motor2Speed = 140;

void moveForward(){
    //values for analogWrite is 0 ~ 255

    //let both motor runs at full speed
    analogWrite(enableMotor1,motor1Speed);
    analogWrite(enableMotor2,motor2Speed);

    //motor 1 moving forward
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW);

    //motor 2 moving forward
    digitalWrite(motor2A,LOW);
    digitalWrite(motor2B,HIGH);
}

void turnRight(){
    //values for analogWrite is 0 ~ 255

    //let left motor slow and right motor fast
    analogWrite(enableMotor1,motor1Speed);
    analogWrite(enableMotor2,motor1Speed/2);

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
    analogWrite(enableMotor1,motor1Speed-30);
    analogWrite(enableMotor2,motor2Speed);

    //motor 1 moving forward
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW);

    //motor 2 moving forward
    digitalWrite(motor2A,LOW);
    digitalWrite(motor2B,HIGH);
}

void turnLeft(){
    //values for analogWrite is 0 ~ 255

    //let right motor slow and left motor fast
    analogWrite(enableMotor1,motor1Speed/2);
    analogWrite(enableMotor2,motor1Speed);

    //motor 1 moving forward
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B,HIGH);

    //motor 2 moving forward
    digitalWrite(motor2A,LOW);
    digitalWrite(motor2B,HIGH);
}

void alignRight(){
    //values for analogWrite is 0 ~ 255

    //let right motor slow and left motor fast
    analogWrite(enableMotor1,motor1Speed);
    analogWrite(enableMotor2,motor2Speed-30);

    //motor 1 moving forward
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW);

    //motor 2 moving forward
    digitalWrite(motor2A,LOW);
    digitalWrite(motor2B,HIGH);
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

// void uTurn(){
//     //values for analogWrite is 0 ~ 255

//     //let both motor stop
//     analogWrite(enableMotor1,motor1Speed);
//     analogWrite(enableMotor2,motor2Speed);

//     //motor 1 stop
//     digitalWrite(motor1A,HIGH);
//     digitalWrite(motor1B,LOW);

//     //motor 2 stop
//     digitalWrite(motor2A,LOW);
//     digitalWrite(motor2B,HIGH);

//     //set the delay besed on requirement
//     delay(500);
// }

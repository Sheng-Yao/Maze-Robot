#include <Arduino.h>

// Left ultrasonic sensor
#define leftTrig 6
#define leftEcho 7

// Front ultrasonic sensor
#define frontTrig 8
#define frontEcho 9

// Right ultrasonic sensor
#define rightTrig 12
#define rightEcho 13

void ultrasonicSetup(){
    pinMode(leftEcho, INPUT);
    pinMode(leftTrig, OUTPUT);
    pinMode(frontEcho, INPUT);
    pinMode(frontTrig, OUTPUT);
    pinMode(rightEcho, INPUT);
    pinMode(rightTrig, OUTPUT);
}

enum Directions{
    FRONT = 0,
    LEFT = 1,
    RIGHT = 2
};

// Return distance in cm
float getDistance(Directions direction){

    byte trigPinNo, echoPinNo;
    if(direction == FRONT){
        trigPinNo = frontTrig;
        echoPinNo = frontEcho;
    }else if(direction == LEFT){
        trigPinNo = leftTrig;
        echoPinNo = leftEcho;
    }else if(direction == RIGHT){
        trigPinNo = rightTrig;
        echoPinNo = rightEcho;
    }

    digitalWrite(trigPinNo,LOW);
    delay(2);
    digitalWrite(trigPinNo,HIGH);
    delay(10);
    digitalWrite(trigPinNo,LOW);
    return (pulseIn(echoPinNo,HIGH) * 0.034613 / 2.00);
}
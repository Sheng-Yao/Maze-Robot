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

// Return distance in cm
float getDistance(byte trigPinNo, byte echoPinNo){
    digitalWrite(trigPinNo,LOW);
    delay(2);
    digitalWrite(trigPinNo,HIGH);
    delay(10);
    digitalWrite(trigPinNo,LOW);
    return (pulseIn(echoPinNo,HIGH) * 0.034613 / 2);
}
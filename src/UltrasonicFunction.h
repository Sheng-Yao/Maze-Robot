#include <Arduino.h>

//#######   Code related to the ultrasonic sensor   #######//
//get the duration of the echo and return it as long variable
long getDuration(byte trigPinNo, byte echoPinNo){
    digitalWrite(trigPinNo,LOW);
    delayMicroseconds(5);
    digitalWrite(trigPinNo,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinNo,LOW);
    return pulseIn(echoPinNo,HIGH);
}

//calculate the distance from the ultrasonic sensor and return it as int variable
int getDistance(long input){
    //At 30'C, 349.02
    //At 25'C, 346.13
    //At 20'C, 343.21
    return input * 0.31613 / 2;
}
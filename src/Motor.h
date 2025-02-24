#include <Arduino.h>
#include <Motor_Basic.h>
#include <math.h>
#include <Ultrasonic.h>
#include <Encoder.h>
#include <MPU6050.h>

byte checkingDuration = 25;

// Ensure robot car move to checking point in each block
void moveCloseToWall(){
    unsigned long previous = millis();
    while(true){
        if(millis() - previous >= checkingDuration){
            float frontDistance = getDistance(FRONT);
            if(int(frontDistance) % 28 <= 4 || frontDistance <= 5){
                stop();
                break;
            }else{
                moveForward();
                continue;
            }
            previous = millis();
        }
    }
}

void moveForwardWithAlignment();

void turnAndMoveForward(){

    unsigned long previous = millis();

    while(true){
        // Update the robot car angle
        update();

        float requiredAngle;
        if(isTurnRight){
            requiredAngle = targetAngle - angle;
        }else if(isTurnLeft || isUTurn){
            requiredAngle = angle - targetAngle;
        }
        Serial.println("targetAngle:" + String(targetAngle) + "angle:" + String(angle) + " requiredAngle:"+String(requiredAngle));

        if(millis() - previous >= checkingDuration){
            if(requiredAngle < 0){
                continue;
            }else{
                // Done turning
                stop();
                resetDistance();

                moveForwardWithAlignment();
            }
            previous = millis();
        }
    }
}

const float ultrasonicMazeSize = 20;
const float encoderMazeSize = 35;

const float lowerAlignmentValue = 4.5;
const float upperAlignmentValue = 6.5;
const float alignmentValueDifference = 1;

bool isReachPoint = false;

void moveForwardWithAlignment(){
    
    unsigned long previous = millis();

    while(true){

        if(millis() - previous <= checkingDuration){
            
            float distance[3] = {0,0,0};
            distance[0] = getDistance(LEFT);
            distance[1] = getDistance(RIGHT);
            distance[2] = getDistance(FRONT);

            if(distance[2] > 5 || (int(distance[2]) % 28 >= 4 && getMovingDistance() < encoderMazeSize)){
                // If no branch at left and right
                if(distance[0] < ultrasonicMazeSize && distance[1] < ultrasonicMazeSize){
                    if(distance[0] > distance[1] + alignmentValueDifference){
                        alignLeft();
                    }else if(distance[1] > distance[0] + alignmentValueDifference){
                        alignRight();
                    }else{
                        moveForward();
                    }
                }else if(distance[0] > ultrasonicMazeSize && distance[1] < ultrasonicMazeSize){// When has left branch
                    if(distance[1] < lowerAlignmentValue){
                        alignLeft();
                    }else if(distance[1] > upperAlignmentValue){
                        alignRight();
                    }else{
                        moveForward();
                    }
                }else if(distance[0] < ultrasonicMazeSize && distance[1] > ultrasonicMazeSize){// WHen has right branch
                    if(distance[0] < lowerAlignmentValue){
                        alignRight();
                    }else if(distance[0] > upperAlignmentValue){
                        alignLeft();
                    }else{
                        moveForward();
                    }
                }else if(distance[0] > ultrasonicMazeSize && distance[1] > ultrasonicMazeSize){// when left and right has branch
                    // Update the robot car angle
                    update();

                    float requiredAngle;
                    if(isTurnRight){
                        requiredAngle = targetAngle - angle;
                    }else if(isTurnLeft || isUTurn){
                        requiredAngle = angle - targetAngle;
                    }

                    if(requiredAngle < 0){
                        if(angle < targetAngle){ //|| (distance[1] < 6 || (distance[0] > 8 && distance[0] < 12))
                            alignLeft();
                        }else if(angle > targetAngle){ //|| (distance[0] < 6 || (distance[1] > 8 && distance[1] < 12))
                            alignRight();
                        }
                    }else{
                        moveForward();
                    }
                }
            }else{
                stop();
                resetDistance();
                isReachPoint = true;
                break;
                Serial.println("Detection location reached.");
            }
            previous = millis();
        }
    }
}
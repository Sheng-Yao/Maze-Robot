#include "MotorFunction.h"
#include "UltrasonicFunction.h"
#include "Algorithm.h"
#include "EEPROMFunction.h"

//current plan is by reading a pin to indicates the robot state (either in path recoignition and maze solving mode)

//interrupt function
void modeChanging(){
    if(isInSearchingMode){
        isInSearchingMode = false;
        overallPath = memoryRead();
    }else{
        isInSearchingMode = true;
    }

}

void setup() {

  Serial.begin(9600);

  //setup pin mode for ultrasonic sensors
  pinMode(leftTrig,OUTPUT);
  pinMode(leftEcho,INPUT);

  pinMode(frontTrig,OUTPUT);
  pinMode(frontEcho,INPUT);

  pinMode(rightTrig,OUTPUT);
  pinMode(rightEcho,INPUT);


  //setup the enable pins for motor 1 & 2
  //for speed control
  pinMode(enableMotor1,OUTPUT);
  pinMode(enableMotor2,OUTPUT);

  //setup motor pins
  //for direction control
  pinMode(motor1A,OUTPUT);
  pinMode(motor1B,OUTPUT);
  pinMode(motor2A,OUTPUT);
  pinMode(motor2B,OUTPUT);

  // set a pin as INPUT mode
  pinMode(InterruptPin,INPUT);

  // let the arduino to detect the rising edge of the interrupt pin for the mode changing between path recoignition or maze competition
  attachInterrupt(digitalPinToInterrupt(InterruptPin),modeChanging,RISING);
}

//minimum front distance where the robot should stop
const double MINFRONTALLOWDISTANCE = 150;

//minimum for the sum of left and right distance
const double MINLEFTRIGHTALLOWDISTANCE = 200;

//benchmark to indicate the side that have branches
const double LEFTRIGHTBRANCHBENCHMARKDISTANCE = 95;

//ending block distance
const double LEFTRIGHTDISTANCE = 250;

//distance between left and right to trigger the alignment process
const double ALIGNMENTDISTANCEBENCHMARK = 20;

void loop() {

  //initialize distance local variables
  double leftDistance = -1;
  double frontDistance = -1;
  double rightDistance = -1;

  //update the distance variables
  leftDistance = getDistance(getDuration(leftTrig,leftEcho));
  frontDistance = getDistance(getDuration(frontTrig,frontEcho));
  rightDistance = getDistance(getDuration(rightTrig,rightEcho));

  if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
    stop();
    delay(10);
    moveForward();
    delay(5);
    tempBranches += "L";
    while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
      turnLeft();
      delay(125);
      stop();
      delay(5);
      moveForward();
      delay(150);
    }
  }else if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
    stop();
    delay(10);
    moveForward();
    delay(5);
    tempBranches += "R";
    while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
      turnRight();
      delay(125);
      stop();
      delay(5);
      moveForward();
      delay(150);
    }

  }else{
    if(frontDistance > MINFRONTALLOWDISTANCE){
      if(abs(leftDistance - rightDistance) > ALIGNMENTDISTANCEBENCHMARK){
        if(leftDistance - rightDistance > ALIGNMENTDISTANCEBENCHMARK*0.5){
          alignLeft();
          delay(10);
        }else if(rightDistance - leftDistance > ALIGNMENTDISTANCEBENCHMARK*0.5){
          alignRight();
          delay(10);
        }
      }else{
        moveForward();
        delay(10);
      }

    }else{
      stop();
      while(1);
    }
  }






















  // //indicator where the robot is in path searching mode
  // if(isInSearchingMode){
  //   if(abs(totalLeftRightDistance - LEFTRIGHTDISTANCE) <= 3){
  //     if(isAtPotentialEndingPoint){
  //       isEndingPoint = true;
  //       isAtPotentialEndingPoint = false;

  //       //reset the EEPROM memory before storing the information
  //       // memoryReset();

  //       //put in the function of saving the string variable into the EEPROM
  //       //memoryWrite(tempBranches);
  //     }else{
  //       isAtPotentialEndingPoint = true;
  //     }
  //   }

  //   if(!isAtPotentialEndingPoint){
  //     if(isReturning){
  //       if(frontDistance > MINFRONTALLOWDISTANCE && totalLeftRightDistance < MINLEFTRIGHTALLOWDISTANCE){
  //         if(leftDistance - rightDistance > ALIGNMENTDISTANCEBENCHMARK){
  //           alignLeft();
  //         }else if(rightDistance - leftDistance > ALIGNMENTDISTANCEBENCHMARK){
  //           alignRight();
  //         }else{
  //           moveForward();
  //         }
  //       }else{
  //         //not confirm if it works for the particular line
  //         if(tempBranches.length() != transitionPoint.length()){
  //           if(tempBranches[tempBranches.length()-1] == 'L'){
  //             if(frontDistance > MINFRONTALLOWDISTANCE + 50){
  //               //still not confirm wherether the transitionPoint works fine
  //               transitionPoint = tempBranches;
  //               tempBranches[tempBranches.length() - 1] = 'R';
  //               moveForward();
  //               isReturning = false;
  //             }else{
  //               if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                 while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                   turnLeft();
  //                   delay(5);
  //                 }
  //                 tempBranches[tempBranches.length() - 1] = 'F';
  //                 isReturning = false;
  //               }else{
  //                 if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                   while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                     turnRight();
  //                     delay(5);
  //                   }
  //                   tempBranches.remove(tempBranches.length()-1);
  //                   clearLCDModule();
  //                   isReturning = true;
  //                 }
  //               }
  //             }
  //           }else if(tempBranches[tempBranches.length()-1] == 'R'){
  //             if(frontDistance > MINFRONTALLOWDISTANCE + 50){
  //               transitionPoint = tempBranches;
  //               tempBranches[tempBranches.length() - 1] = 'L';
  //               moveForward();
  //               isReturning = false;
  //             }else{
  //               if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                 while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                   turnLeft();
  //                   delay(5);
  //                 }
  //                 tempBranches.remove(tempBranches.length()-1);
  //                 clearLCDModule();
  //                 isReturning = true;
  //               }else{
  //                 if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                   while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                     turnRight();
  //                     delay(5);
  //                   }
  //                   tempBranches[tempBranches.length() - 1] = 'F';
  //                   isReturning = false;
  //                 }
  //               }
  //             }
  //           }
  //         }else{
  //           //indicates that it already go for left side
  //           if(transitionPoint[transitionPoint.length()-1] == 'L'){
  //             if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                 while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                   turnRight();
  //                   delay(5);
  //                 }
  //               tempBranches[tempBranches.length() - 1] = 'F';
  //               isReturning = false;
  //             }else{
  //               while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                 turnLeft();
  //                 delay(5);
  //               }
  //               tempBranches.remove(tempBranches.length()-1);
  //               clearLCDModule();
  //               isReturning = true;
  //             }
  //           }else if(transitionPoint[transitionPoint.length()-1] == 'R'){
  //             if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //               while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                 turnRight();
  //                 delay(5);
  //               }
  //               tempBranches.remove(tempBranches.length()-1);
  //               clearLCDModule();
  //               isReturning = true;
  //             }else{
  //               while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                 turnLeft();
  //                 delay(5);
  //               }
  //               tempBranches[tempBranches.length() - 1] = 'F';
  //               isReturning = false;
  //             }
  //           }
  //         } 
  //       }
  //     }else{
  //       if(frontDistance > MINFRONTALLOWDISTANCE){
  //       // if(frontDistance > MINFRONTALLOWDISTANCE || tempBranches[tempBranches.length()-1] == 'F'){
  //         if(leftDistance - rightDistance > ALIGNMENTDISTANCEBENCHMARK){
  //           alignLeft();
  //         }else if(rightDistance - leftDistance > ALIGNMENTDISTANCEBENCHMARK){
  //           alignRight();
  //         }else{
  //           moveForward();
  //         }

  //           // if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //           //   tempBranches += "L";
  //           //   while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //           //     turnLeft();
  //           //     delay(5);
  //           //   }
  //           // }else if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //           //   tempBranches += "R";
  //           //   while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //           //     turnRight();
  //           //     delay(5);
  //           //   }
  //           // }
  //         }else{
  //           if(totalLeftRightDistance > MINLEFTRIGHTALLOWDISTANCE){
  //             if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //               tempBranches += "L";
  //               while(getDistance(getDuration(rightTrig,rightEcho)) < LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                 turnLeft();
  //                 delay(100);
  //               }
  //             }else if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //               tempBranches += "R";
  //               while(getDistance(getDuration(leftTrig,leftEcho)) < LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //                 turnRight();
  //                 delay(100);
  //               }
  //             }

  //           // if(leftDistance - rightDistance > ALIGNMENTDISTANCEBENCHMARK){
  //           //   alignLeft();
  //           // }else if(rightDistance - leftDistance > ALIGNMENTDISTANCEBENCHMARK){
  //           //   alignRight();
  //           // }else{
  //           //   Serial.println("Entering 1");
  //           //   moveForward();
  //           // }
  //         }
  //       }

  //       // }else{
  //       //   uTurn();
  //       //   isReturning = true;
  //       // }
  //     }
  //   }else{
  //     turnLeft();
  //     delay(15);
  //     while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTDISTANCE / 2){
  //       turnLeft();
  //       delay(5);
  //     }
  //   }
  // }else{
  //   //the maze competition mode
    
  // }
  // delay(2);
}
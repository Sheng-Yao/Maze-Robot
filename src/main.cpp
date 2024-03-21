#include "MotorFunction.h"
#include "UltrasonicFunction.h"
#include "Algorithm.h"

void setup() {

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

}

//minimum front distance where the robot should stop
const byte MINFRONTALLOWDISTANCE = 40;

//minimum for the sum of left and right distance
const byte MINLEFTRIGHTALLOWDISTANCE = 10;

//benchmark to indicate the side that have branches
const byte LEFTRIGHTBRANCHBENCHMARKDISTANCE = 15;

//ending block distance
const byte LEFTRIGHTDISTANCE = 40;

const byte ALIGNMENTDISTANCEBENCHMARK = 3;

void loop() {

  //initialize distance local variables
  byte leftDistance = -1;
  byte frontDistance = -1;
  byte rightDistance = -1;

  //update the distance variables
  leftDistance = getDistance(getDuration(leftTrig,leftEcho));
  frontDistance = getDistance(getDuration(frontTrig,frontEcho));
  rightDistance = getDistance(getDuration(rightTrig,rightEcho));

  //get the sum of left and right distance
  byte totalLeftRightDistance = leftDistance + rightDistance;




  if(isReturning){
    if(frontDistance > MINFRONTALLOWDISTANCE && totalLeftRightDistance < MINLEFTRIGHTALLOWDISTANCE){
      if(leftDistance - rightDistance > ALIGNMENTDISTANCEBENCHMARK){
        alignLeft();
      }else if(rightDistance - leftDistance > ALIGNMENTDISTANCEBENCHMARK){
        alignRight();
      }else{
        moveForward();
      }
    }else{
      if(tempBranches.length() != transitionPoint.length()){
        if(tempBranches[tempBranches.length()-1] == 'L'){
          if(frontDistance > MINFRONTALLOWDISTANCE + 10){
            transitionPoint = tempBranches;
            tempBranches[tempBranches.length() - 1] = 'R';
            moveForward();
            isReturning = false;
          }else{
            if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
              while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
                turnLeft();
                delay(5);
              }
              isReturning = false;
            }else{
              if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
                while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
                  turnRight();
                  delay(5);
                }
              isReturning = true;
              }
            }
            tempBranches.remove(tempBranches.length()-1);
          }
        }else if(tempBranches[tempBranches.length()-1] == 'R'){
          if(frontDistance > MINFRONTALLOWDISTANCE + 10){
            transitionPoint = tempBranches;
            tempBranches[tempBranches.length() - 1] = 'L';
            moveForward();
            isReturning = false;
          }else{
            if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
              while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
                turnLeft();
                delay(5);
              }
              isReturning = true;
            }else{
              if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
                while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
                  turnRight();
                  delay(5);
                }
              isReturning = false;
              }
            }
            tempBranches.remove(tempBranches.length()-1);
          }
        }
      }else{
        if(transitionPoint[transitionPoint.length()-1] == 'L'){
          if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
              while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
                turnRight();
                delay(5);
              }
            isReturning = false;
          }else{
            while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
              turnLeft();
              delay(5);
            }
            isReturning = true;
          }
          tempBranches.remove(tempBranches.length()-1);
        }else if(transitionPoint[transitionPoint.length()-1] == 'R'){
          if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
            while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
              turnRight();
              delay(5);
            }
            isReturning = true;
          }else{
            while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
              turnLeft();
              delay(5);
            }
            isReturning = false;
          }
          tempBranches.remove(tempBranches.length()-1);
        }
      } 
    }
  }else{
    if(frontDistance > MINFRONTALLOWDISTANCE){
      if(totalLeftRightDistance > MINLEFTRIGHTALLOWDISTANCE){

        if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
          tempBranches += "L";
          while(getDistance(getDuration(leftTrig,leftEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
            turnLeft();
            delay(5);
          }
        }else if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
          tempBranches += "R";
          while(getDistance(getDuration(rightTrig,rightEcho)) > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
            turnRight();
            delay(5);
          }
        }
      }else{
        if(leftDistance - rightDistance > ALIGNMENTDISTANCEBENCHMARK){
          alignLeft();
        }else if(rightDistance - leftDistance > ALIGNMENTDISTANCEBENCHMARK){
          alignRight();
        }else{
          moveForward();
        }
      }


    }else{
      uTurn();
      isReturning = true;
    }
  }

  











  // if(isBranching){
  //   if(desiredDirection == 'R'){
  //     if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){   //check if it is right branch
  //       turnRight();

  //       if(currentSubBranching)
  //     }
  //   }else if(desiredDirection == 'L'){
  //     if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){   //check if it is right branch
  //       turnLeft();
  //     }
  //   }
  //   numberOfSubBranch = 0;
  // }




  // if(!isReturning){   //run is the robot is not in branch returning mode

  //   //replace with the min allowable front distance from the ultrasonic sensor
  //   //run the code if the distance still far away from the front wall
  //   if(frontDistance > MINFRONTALLOWDISTANCE){

  //     //branches detection
  //     //check wherether there are branch (either on left or right)
  //     //idea is sum of left and right distance more than the standard distance between two walls
  //     if(totalLeftRightDistance > MINLEFTRIGHTALLOWDISTANCE){
        
  //       if(!isBranching){
  //         //run the code if the branch doesn't being calculated
  //         if(!isBranchCalculated){

  //           //check is either left or right branch
  //           if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){  //check if it is left branch
  //             overallPath[currentNumber][currentLevel] = Branch(currentNumber, currentLevel, 'L');
  //             currentLevel++;
  //           }
            
  //           if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){   //check if it is right branch
  //             overallPath[currentNumber][currentLevel] = Branch(currentNumber, currentLevel, 'R');
  //             currentLevel++;
  //           }

  //           numberOfSubBranch = currentLevel;
  //         }
  //       }else{
  //         switch (currentSubBranching)
  //         {
  //         case 1:
  //           //check whether is left branch
  //           if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //             subBranch1 += "L";
  //           }
            
  //           //check whether is right branch
  //           if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //             subBranch1 += "R";
  //           }
  //           break;
          
  //         case 2:

  //           //check whether is left branch
  //           if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //             subBranch2 += "L";
  //           }
            
  //           //check whether is right branch
  //           if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //             subBranch2 += "R";
  //           }
  //           break;

  //         case 3:

  //           //check whether is left branch
  //           if(leftDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //             subBranch3 += "L";
  //           }
            
  //           //check whether is right branch
  //           if(rightDistance > LEFTRIGHTBRANCHBENCHMARKDISTANCE){
  //             subBranch3 += "R";
  //           }
  //           break;

  //         default:
  //           break;
  //         }
  //       }

        

  //       //put up the flag to avoid overcalculation
  //       isBranchCalculated = true;

  //     }else{  //the robot is located in the straight path (without branches)

  //       //reset the flag to allow the detection for next branch
  //       isBranchCalculated = false;
  //     }

  //     //to move forward
  //     moveForward();
  //     //continue to move forward for 5 milliseconds
  //     delay(5);

  //   }else{  //the robot is too near to the front wall
      
  //     //make a u-turn
  //     uTurn();

  //     //check if it is a dead way without any branches
  //     if(numberOfSubBranch == 0){

  //       //flag to indicate that the robot is going back to previous tree of the branch
  //       isReturning = true;
  //     }else{

  //       //flag to indicate that the robot is going into the branch
  //       isBranching = true;

  //       //reversed the direction of the branch (because the robot is already make a u-turn)
  //       overallPath[currentNumber][currentLevel].reverseDirection();

  //       //get the direction required the robot to turn
  //       desiredDirection = overallPath[currentNumber][currentLevel].getDirection();
  //     }

  //   }

  // }else{  //robot is in returning mode
  //   moveForward();
  // }
}
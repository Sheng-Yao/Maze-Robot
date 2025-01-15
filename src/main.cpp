#include <Motor.h>
#include <Ultrasonic.h>

void setup(){

  // Initialize serial monitor
  Serial.begin(9600);
  
  encoderSetup();
  
  motorSetup();

  mpuSetup();

  ultrasonicSetup();

  Serial.println("Done setup..");

  delay(1000);
}

bool isMoving = false;
bool isReachPoint = false;

float distance[3] = {0,0,0};

const byte mazeWidth = 10;

unsigned long current = millis();

void loop(){

  if(getDistance(frontTrig,frontEcho) > 5){
    if(!isMoving){
      moveForward();
      isMoving = true;
      resetDistance();
    }

    if(getMovingDistance() >= 24){
      stop();
      isMoving = false;
      isReachPoint = true;
    }else{
      // Moving forward + Align
      if(millis() - current > 75){
        distance[0] = getDistance(leftTrig,leftEcho);
        distance[1] = getDistance(rightTrig,rightEcho);
        float distanceDifference = distance[0] - distance[1];
        if(distanceDifference <= -2.5){
          detachInterrupt(digitalPinToInterrupt(encoderPinA));
          detachInterrupt(digitalPinToInterrupt(encoderPinB));
          leftSpeedVal = equilibriumSpeed;
          rightSpeedVal = equilibriumSpeed;
          alignRight();
        }
        else if(distanceDifference >= 2.5){
          detachInterrupt(digitalPinToInterrupt(encoderPinA));
          detachInterrupt(digitalPinToInterrupt(encoderPinB));
          leftSpeedVal = equilibriumSpeed;
          rightSpeedVal = equilibriumSpeed;
          alignLeft();
        }
        else{
          attachInterrupt(digitalPinToInterrupt(encoderPinA), counterLeftUpdate, RISING);
          attachInterrupt(digitalPinToInterrupt(encoderPinB), counterRightUpdate, RISING);
          moveForward();
        }
        current = millis();
      }
    }
  }else{
    Serial.println(String(pulsesLeft - pulsesRight));
    Serial.println(String(pulsesLeft + pulsesRight));
    Serial.println(String(getMovingDistance()));
    stop();
    while(1){;}
  }

  // Serial.println(String(getMovingDistance()));

  // if(isMoving){
    
  //   if(getPosition()){
  //     Serial.println("X: " + String(accelOutputBuffer[0]));
  //     Serial.println("Y: " + String(accelOutputBuffer[1]));
  //     Serial.println();
  //   }else{
  //     Serial.println("Can't get position data");
  //   }

  //   if(getOrientation()){
  //     Serial.println("Rotation: " + String(gyroOutputBuffer));
  //     Serial.println();
  //   }else{
  //     Serial.println("Can't get orientation data");
  //   }

  //   if(getMovingDistance() >= 25){
  //     stop();
  //     isMoving = false;
  //     isReachPoint = true;
  //     paused = true;
  //   }
  // }else{
  //   if(getMovingDistance() < 25){
  //     moveForward();
  //     isMoving = true;
  //   }
  // }

  // update();
 
  // static int count;
  // static int countStraight;
  // if (count < 6){  
  //   count ++;
  // } else { //runs once after void loop() runs 7 times. void loop runs about every 2.8ms, so this else condition runs every 19.6ms or 50 times/second
  //   count = 0;
  //   if (!paused){
  //     if (isDriving != prevIsDriving){
  //       leftSpeedVal = equilibriumSpeed;
  //       countStraight = 0;
  //     }
  //     if (isDriving) {
  //       if (abs(targetAngle - angle) < 3){
  //         if (countStraight < 20){
  //           countStraight ++;
  //         } else {
  //           countStraight = 0;
  //           equilibriumSpeed = leftSpeedVal; //to find equilibrium speed, 20 consecutive readings need to indicate car is going straight
  //         }
  //       } else {
  //         countStraight = 0;
  //       }
  //       driving();
  //     } else {
  //       rotate();
  //     }
  //     prevIsDriving = isDriving;
  //   }
  // } 

  if(isReachPoint){

    distance[0] = getDistance(leftTrig,leftEcho);
    distance[1] = getDistance(rightTrig,rightEcho);
    distance[2] = getDistance(frontTrig,frontEcho);

    Serial.println("Distance (Left): " + String(distance[0]));

    delay(5);

    Serial.println("Distance (Right): " + String(distance[1]));

    delay(5);

    Serial.println("Distance (Front): " + String(distance[2]));

    if(distance[0] < mazeWidth && distance[1] < mazeWidth && distance[2] < mazeWidth){
      Serial.println("Dead End");
      // uTurn();
    }else if(distance[0] < mazeWidth && distance[1] < mazeWidth && distance[2] > mazeWidth){
      Serial.println("Front");
      // uTurn();
    }else if(distance[0] > mazeWidth && distance[1] < mazeWidth && distance[2] < mazeWidth){
      Serial.println("Left branch");
      // turnLeft();
    }else if(distance[0] < mazeWidth && distance[1] > mazeWidth && distance[2] < mazeWidth){
      Serial.println("Right branch");
      // turnRight();
    }else if(distance[0] > mazeWidth && distance[1] > mazeWidth && distance[2] < mazeWidth){
      Serial.println("Left and Right branches");
      // turnLeft();
    }else if(distance[0] > mazeWidth && distance[1] < mazeWidth && distance[2] > mazeWidth){
      Serial.println("Left and front branches");
      // turnLeft();
    }else if(distance[0] < mazeWidth && distance[1] > mazeWidth && distance[2] > mazeWidth){
      Serial.println("Right and front branches");
      // turnRight();
    }else if(distance[0] > mazeWidth && distance[1] > mazeWidth && distance[2] > mazeWidth){
      Serial.println("Left, Right and front branches");
      // turnLeft(); // Possible reach the end point need to add in with end point validation
    }
    
    isReachPoint = false;

    delay(5000);
  }

  // distance[0] = getDistance(leftTrig,leftEcho);
  // distance[1] = getDistance(rightTrig,rightEcho);

  // Serial.println("Left: " + String(distance[0]));
  // Serial.println("Right: " + String(distance[1]));

  // if(getPosition()){
  //   Serial.println("X Position: " + String(accelOutputBuffer[0]));
  //   Serial.println("Y Position: " + String(accelOutputBuffer[1]));
  //   Serial.println("Z Position: " + String(accelOutputBuffer[2]));
  // }

  // update();

  // if(getOrientation()){
  //   Serial.println("Z Rotation angle: " + String(angle));
  // }

  // delay(200);
}
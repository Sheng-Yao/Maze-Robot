#include <Encoder.h>
#include <Motor.h>
#include <MPU6050.h>
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

void loop(){

  if(isMoving){
    
    if(getPosition()){
      Serial.println("X: " + String(accelOutputBuffer[0]));
      Serial.println("Y: " + String(accelOutputBuffer[1]));
      Serial.println();
    }else{
      Serial.println("Can't get position data");
    }

    if(getOrientation()){
      Serial.println("Rotation: " + String(gyroOutputBuffer));
      Serial.println();
    }else{
      Serial.println("Can't get orientation data");
    }

    if(getMovingDistance() >= 25){
      stop();
      isMoving = false;
      isReachPoint = true;
    }
  }else{
    if(getMovingDistance() < 25){
      moveForward();
      isMoving = true;
    }
  }


  if(isReachPoint){
    Serial.println("Distance (Left): " + String(getDistance(leftTrig,leftEcho)));

    delay(5);

    Serial.println("Distance (Right): " + String(getDistance(rightTrig,rightEcho)));

    delay(5);

    Serial.println("Distance (Front): " + String(getDistance(frontTrig,frontEcho)));

    isReachPoint = false;

    turnRight();
    unsigned long lastTime = millis();
    while(true){
      if(getOrientation()){
        if(gyroOutputBuffer * (millis()-lastTime)/1000.0 >= 90){
          stop();
          break;
        }else{
          continue;
        }
      }
    }
  }

}
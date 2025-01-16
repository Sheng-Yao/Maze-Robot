#include <Arduino.h>
#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address

// Pin connection
// VCC -> 3.3 V / 5 V (better) 
// GND -> GND 
// SCL -> A5 
// SDA -> A4 

float elapsedTime, currentTime, previousTime;
float GyroErrorZ;
float gyroOutputBuffer = 0;
float yaw;

float angle = 0;
float targetAngle = 0;

bool getOrientation(){
  // Reset variable holder for X, Y
  gyroOutputBuffer = 0;

  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  gyroOutputBuffer = (Wire.read() << 8 | Wire.read()) / 131.0;
  gyroOutputBuffer = (Wire.read() << 8 | Wire.read()) / 131.0;
  gyroOutputBuffer = 0;
  gyroOutputBuffer = (Wire.read() << 8 | Wire.read()) / 131.0;  // get z rotation (yaw)

  if(gyroOutputBuffer != 0){
    return true;
  }else{
    return false;
  }
}

void calculateError() {
  // Read accelerometer values 200 times
  byte c = 0;
  
  // Read gyro values 200 times
  while (c < 200) {
    if(getOrientation()){
      GyroErrorZ += gyroOutputBuffer;
      c++;
    }
  }
  //Divide the sum by 200 to get the error value
  GyroErrorZ = GyroErrorZ / 200;
  Serial.println("The the gryoscope setting in MPU6050 has been calibrated");
}

void mpuSetup(){
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  // Call this function if you need to get the IMU error values for your module
  calculateError();
}


void update(){
    // === Read gyroscope (on the MPU6050) data === //
    previousTime = currentTime;
    currentTime = millis();
    elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
    getOrientation();
    // Correct the outputs with the calculated error values
    gyroOutputBuffer -= GyroErrorZ;
    // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
    yaw += gyroOutputBuffer * elapsedTime;
    angle = round(yaw*10)/10.0; //if you mounted MPU6050 in a different orientation to me, angle may not = roll. It can roll, pitch, yaw or minus version of the three
    //for me, turning right reduces angle. Turning left increases angle.
}


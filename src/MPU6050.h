#include <Arduino.h>
#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address

// Pin connection
// VCC -> 3.3 V / 5 V (better) 
// GND -> GND 
// SCL -> A5 
// SDA -> A4 

float AccErrorX, AccErrorY, GyroErrorZ;
float accelOutputBuffer[3]={0,0,0};
float gyroOutputBuffer = 0;

bool getPosition() {
  accelOutputBuffer[0] = 0;
  accelOutputBuffer[1] = 0;
  accelOutputBuffer[2] = 0;

  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the MPU6050 datasheet
  accelOutputBuffer[0] = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  accelOutputBuffer[1] = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  accelOutputBuffer[2] = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value

  if(accelOutputBuffer[0] != 0 && accelOutputBuffer[1] != 0 && accelOutputBuffer[2] != 0){
      return true;
  }else{
      return false;
  }
}

bool getOrientation(){
  // Reset variable holder for X, Y
  gyroOutputBuffer = 0;

  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  gyroOutputBuffer = (Wire.read() << 8 | Wire.read()) / 131.0;
  gyroOutputBuffer = (Wire.read() << 8 | Wire.read()) / 131.0;
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
  while (c < 200) {
    if(getPosition()){
      AccErrorX += (atan((accelOutputBuffer[1]) / sqrt(pow((accelOutputBuffer[0]), 2) + pow((accelOutputBuffer[2]), 2))) * 180 / PI);
      AccErrorY += (atan(-1 * (accelOutputBuffer[0]) / sqrt(pow((accelOutputBuffer[1]), 2) + pow((accelOutputBuffer[2]), 2))) * 180 / PI);
      c++;
    }
  }
  //Divide the sum by 200 to get the error value, since expected value of reading is zero
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  
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


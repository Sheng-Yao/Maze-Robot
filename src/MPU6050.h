#include <Arduino.h>
#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address

// Pin connection
// VCC -> 3.3 V / 5 V (better) 
// GND -> GND 
// SCL -> A5 
// SDA -> A4 

// Variable to calculate rotational element
const float GYRO_SCALE = 1.0 / 131.0;
float gyroOutputBuffer = 0;

// Obtain the orientation of robot car
bool getOrientation(){
  // Reset variable holder for X, Y
  gyroOutputBuffer = 0;

  Wire.beginTransmission(MPU);
  Wire.write(0x47);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 2, true);
  // Only take rotational at Z-axis only (Yaw)
  gyroOutputBuffer = (Wire.read() << 8 | Wire.read()) * GYRO_SCALE;

  // Error handling mechanism to avoid no data being obtained from MPU module
  if(gyroOutputBuffer != 0){
    return true;
  }else{
    return false;
  }
}


// MPU6050 calibration value
float GyroErrorZ;

// MPU6050 calibration (By taking 200 samples)
void calculateError() {
  // Read accelerometer values 200 times
  byte c = 0;
  GyroErrorZ = 0;
  
  // Read gyro values 200 times
  while (c < 200) {
    if(getOrientation()){
      GyroErrorZ += gyroOutputBuffer;
      c++;
    }
  }

  //Divide the sum by 200 to get the error value
  GyroErrorZ = GyroErrorZ / 200.00;
  Serial.println("The the gryoscope setting in MPU6050 has been calibrated");
}

// Setup MPU6050 and doing calibration
void mpuSetup(){
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission

  // Call this function if you need to get the IMU error values for your module
  calculateError();
}

// Time variable to calculate rotational degrees
float elapsedTime, previousTime;
float currentTime = millis();

// Variable for rotation angle
volatile float yaw;
volatile float angle = 0;

// Update robot car current angle (The result can be obtained by calling angle variable)
void update(){
  // Get angle value and elimite the error with the calibrated value
  getOrientation();
  gyroOutputBuffer -= GyroErrorZ;

  // Settling time element
  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) * 0.001; // Divide by 1000 to get seconds
  
  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by seconds (s) to get the angle in degrees
  yaw += gyroOutputBuffer * elapsedTime;
  angle = round(yaw*10)/10.0; //if you mounted MPU6050 in a different orientation to me, angle may not = roll. It can roll, pitch, yaw or minus version of the three
  //for me, turning right reduces angle. Turning left increases angle.
}


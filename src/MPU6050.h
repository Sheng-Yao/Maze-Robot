#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

// Pin connection
// VCC -> 3.3 V / 5 V (better) 
// GND -> GND 
// SCL -> A5 
// SDA -> A4 

void mpuSetup(){
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

float accelOutputBuffer[2]={0,0};

bool getPosition() {

    // Reset variable holder for X, Y
    accelOutputBuffer[0] = 0;
    accelOutputBuffer[1] = 0;

    /* Get new sensor events with the readings */
    sensors_event_t accel_value, gyro_value, temp;
    mpu.getEvent(&accel_value, &gyro_value, &temp);

    accelOutputBuffer[0] = accel_value.acceleration.x;
    accelOutputBuffer[1] = accel_value.acceleration.y;

    if(accelOutputBuffer[0] != 0 && accelOutputBuffer[1] != 0){
        return true;
    }else{
        return false;
    }
}

float gyroOutputBuffer = 0;

bool getOrientation(){

    // Reset variable holder for X, Y
    gyroOutputBuffer = 0;

    /* Get new sensor events with the readings */
    sensors_event_t accel_value, gyro_value, temp;
    mpu.getEvent(&accel_value, &gyro_value, &temp);

    gyroOutputBuffer = gyro_value.gyro.z * 57.29577793F;

    if(gyroOutputBuffer != 0){
        return true;
    }else{
        return false;
    }
}
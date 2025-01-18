#include <Arduino.h>

#define encoderPinA 2
#define encoderPinB 3

volatile byte pulsesLeft = 0;
volatile byte pulsesRight = 0;

void counterLeftUpdate()
{
    pulsesLeft++;
}
void counterRightUpdate()
{
    pulsesRight++;
}

void encoderSetup(){
  // Set encoder pins as input with pull-up resistors
  pinMode(encoderPinA, INPUT); 
  pinMode(encoderPinB, INPUT);

  // Attach interrupts to the encoder pins
  attachInterrupt(digitalPinToInterrupt(encoderPinA), counterLeftUpdate, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), counterRightUpdate, RISING);
}

const float wheelCircumference = 0.05 * PI * 0.0325 * 100;

float getMovingDistance(){
    noInterrupts();

    float distance = ((pulsesLeft + pulsesRight) / 2) * wheelCircumference;

   //Restart the interrupt processing
    interrupts();

    return distance;
}

void resetDistance(){
    pulsesLeft = 0;
    pulsesRight = 0;
}
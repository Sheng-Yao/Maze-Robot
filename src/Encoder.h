#include <Arduino.h>

#define encoderPinA 2
#define encoderPinB 3

// Pulses counter
volatile byte pulsesLeft = 0;
volatile byte pulsesRight = 0;

// Pulses increment function (Used in interrupt)
void counterLeftUpdate()
{
    pulsesLeft++;
}
void counterRightUpdate()
{
    pulsesRight++;
}

// Setup encoder pin modes and interrupt of pulses update
void encoderSetup(){
  // Set encoder pins as input to detect the sigal input
  pinMode(encoderPinA, INPUT); 
  pinMode(encoderPinB, INPUT);

  // Attach interrupts to the encoder pins
  attachInterrupt(digitalPinToInterrupt(encoderPinA), counterLeftUpdate, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), counterRightUpdate, RISING);
}

const float encoderConstant = 0.05 * PI * 0.0325 * 100;

// Obtain the distance travelled by motor
float getMovingDistance(){
    // Stop interrupts when calculating the distance
    noInterrupts();

    // Calculate distance travel
    float distance = ((pulsesLeft + pulsesRight) / 2) * encoderConstant;

   // Restart the interrupts
    interrupts();

    return distance;
}

// Reset pulses
void resetDistance(){
    pulsesLeft = 0;
    pulsesRight = 0;
}
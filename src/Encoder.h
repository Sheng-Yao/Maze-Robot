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



float getMovingDistance(){
    detachInterrupt(digitalPinToInterrupt(encoderPinA));
    detachInterrupt(digitalPinToInterrupt(encoderPinB));

    float distance = ((pulsesLeft + pulsesRight) / 4) / (float)20 * 3.142 * (0.068) * 100;

   //Restart the interrupt processing
    attachInterrupt((encoderPinA), counterLeftUpdate, RISING);
    attachInterrupt(digitalPinToInterrupt(encoderPinB), counterRightUpdate, RISING);

    return distance;
}

void resetDistance(){
    pulsesLeft = 0;
    pulsesRight = 0;
}
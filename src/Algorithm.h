#include <Arduino.h>

//digital pin 2 is the interrupt pin
#define InterruptPin 2

String tempBranches;

bool isReturning = false;

String transitionPoint = "";

bool isAtPotentialEndingPoint = false;

bool isEndingPoint = false;

bool isInSearchingMode = true;

String overallPath = "";
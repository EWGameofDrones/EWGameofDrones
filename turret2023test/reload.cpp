#include <AccelStepper.h>

#define motorPin1  2
#define motorPin2  3
#define motorPin3  4
#define motorPin4  5

int stepsPerRevolution = 64;
int degreePerRevolution = 5.625;

AccelStepper stepper(AccelStepper::HALF4WIRE, motorPin1, motorPin3, motorPin2, motorPin4);

// Degrees of the movement. First, five full rotations (360 degrees each), then back five times.
int steps[] = {-161,161};
int stepsCount = 2; // Total number of elements in the steps[] array

int stepsIndex = 0;

float degToSteps(float deg) {
  return (stepsPerRevolution / degreePerRevolution) * deg;
}

void reloadSetUp() {
  stepper.setMaxSpeed(2000.0);
  stepper.setAcceleration(1500.0);
  stepper.setSpeed(2000);
  pinMode(13,INPUT);
}

void reloadStepper() {
  if(digitalRead(13)==HIGH){
    if (!stepper.isRunning() && stepper.distanceToGo() == 0) {
      stepper.moveTo(degToSteps(steps[stepsIndex]));
      stepsIndex++;
      if (stepsIndex >= stepsCount) {
        stepsIndex = 0;
      }
    }
  Serial.print("WOKRIN \n");
  }
stepper.run();
}

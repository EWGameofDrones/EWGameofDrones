/* GODs Summer 2023 - Will, Eddy, Matthew, Malia, Kayla
*
* Description: will shoot any bullet. On button press will shoot the bullet. Wait 1 sec to press again to reload to shoot again.
* 
*/

//------------------------------------------------------------------------------------

#include <AccelStepper.h>

#define motorPin1  2
#define motorPin2  3
#define motorPin3  4
#define motorPin4  5

int stepsPerRevolution = 64;
int degreePerRevolution = 5.625;

AccelStepper stepper(AccelStepper::HALF4WIRE, motorPin1, motorPin3, motorPin2, motorPin4);

// Degrees of movement
int steps[] = {-200,200}; // foward + backwards
int stepsCount = 2; // Total number of elements in the steps[] array

bool forwardDirection = true;
bool isMoving = false;
int stepsIndex = 0;

float degToSteps(float deg) {
  return (stepsPerRevolution / degreePerRevolution) * deg;
}

void reloadSetUp() {
  stepper.setMaxSpeed(4000.0); //3000
  stepper.setAcceleration(3000.0); //2000
  stepper.setSpeed(3000); //initial speed 2000
  pinMode(13,INPUT);
}

int check = 0;

void reloadStepper() {

  if (digitalRead(13) == HIGH) {
    //check = 0;
    // If the button is pressed, perform the state transition
    if (!isMoving) {
      // If currently in state start1, move to state forward
      if (forwardDirection && stepsIndex == 0) {
        forwardDirection = true;
        stepsIndex = 1;
        check = 10;
      }
      // If currently in state forward, move to state backward
      else if (forwardDirection && stepsIndex == 1) {
        forwardDirection = false;
        stepsIndex = 1;
        check = 20;
      }
      // If currently in state backward, move to state forward
      else if (!forwardDirection && stepsIndex == 1) {
        forwardDirection = true;
        stepsIndex = 0;
        check = 30;
      }
    }

  }

  // Perform the stepper movement if it's running
  if (isMoving) {
    stepper.run();
    // Check if the movement is completed
    if (!stepper.isRunning()) {
      isMoving = false;
    }
  } else {
    // Start the motor movement based on the current state
    if (forwardDirection) {
      stepper.moveTo(degToSteps(steps[stepsIndex]));
    } else {
      stepper.moveTo(degToSteps(-steps[stepsIndex]));
    }
    isMoving = true;
  }
}

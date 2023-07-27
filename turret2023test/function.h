/* GODs Summer 2023 - Will, Eddy, Matthew, Malia, Kayla
*
* Description: This is the code for the second game of drones turret. It features a jostick that controls two bigger stepper motors that are
* 
* 
*/

//------------------------------------------------------------------------------------

#ifndef FUNCTION_H
#define FUNCTION_H

#include <Arduino.h>
#include <AccelStepper.h>

//extern allows you to access the variables from the various cpp files in the .ino file (not really needed if just calling functions)

extern AccelStepper stepper1;
extern AccelStepper stepper2;
extern AccelStepper stepper;
extern float deg;

//all equipment functions
void laserSetUp(); //sets up all the pins for laser + laser button
void laserButton(); //runs the code for laser + button

void joystickDirection(); //gives you the direction of the joystick in direction variable
void stepperSetUp(); //sets up steppers
void moveOneRotation(); //moves two stepper motors one full rotation then reverses the rotation
void moveStepper1(); //deals with x? axis
void moveStepper2(); //deals with y? axis

void reloadStepper();
void reloadSetUp();
void resetPos(); //resets position of turret when joystick button is pressed

//testing functions
void testJoystick(); //tests direction of joystick (does not really account for corners)
void testLaserButton(); //tests to make sure when clicking, laser turns on
void moreTest();
void doEverything();//this is the loop of all the functions that will be run in the .ino file

//test functions - I just made these to make sure everything is hooked up correctly
void setupTest();
void loopTest();

float degToSteps(float);

#endif
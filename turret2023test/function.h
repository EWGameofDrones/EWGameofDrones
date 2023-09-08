/* GODs Summer 2023 - Will, Eddy, Matthew, Malia, Kayla
*
*
* Description: Header file for all function files. If you call a function in the .ino file it has to be decleared here.
* 
* 
*/

//------------------------------------------------------------------------------------------------------

#ifndef FUNCTION_H
#define FUNCTION_H

#include <Arduino.h>
#include <AccelStepper.h>

//------------------------------------------------------------------------------------------------------

//extern allows you to access the variables from the various cpp files in the .ino file (not really needed if just calling functions)

extern AccelStepper stepper1;
extern AccelStepper stepper2;
extern AccelStepper stepper;
extern float deg;

extern int direction;

//------------------------------------------------------------------------------------------------------

//all equipment functions
void laserSetUp(); //sets up all the pins for laser + laser button
void laserButton(); //runs the code for laser + button, UNIMPLEMENTED. currently hardwired to turret

void joystickDirection(); //gives you the direction of the joystick in direction variable
void stepperSetUp(); //sets up steppers
void moveOneRotation(); //a test function that moves two stepper motors one full rotation then reverses the rotation
void moveStepper1(); //reads joystickDirection() to move on x/y axis
void moveStepper2(); //reads joystickDirection() to move on x/y axis

void reloadStepper(); //will move the reload stick foward with a press and back with another press
void reloadSetUp(); //include this in loop()
void resetPos(); //resets position of turret when joystick button is pressed (not working!!)

//testing functions
void testJoystick(); //tests direction of joystick (does not really account for corners)
void testLaserButton(); //tests to make sure when clicking, laser turns on
void moreTest();
void doEverything();//this is the loop of all the functions that will be run in the .ino file

//test functions - I just made these to make sure everything is hooked up correctly
void setupTest();
void loopTest();

float degToSteps(float); //converts degrees to steps

//------------------------------------------------------------------------------------------------------

#endif

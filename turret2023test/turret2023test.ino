/* GODs Summer 2023 - Will, Eddy, Matthew, Malia, Kayla
*
* Description: This is the code for the second game of drones turret. It features a jostick that controls two bigger stepper motors that are
* are attached to two big easy drivers. A smaller stepper motor that loads up the bullets. Buttons to control firing, laser, camera, and screen.
* 
*/

//------------------------------------------------------------------------------------
//included files

#include "function.h"
//#include <AccelStepper.h>
//#include <Stepper.h>

//#define ANALOG_IN A0
//#define hello 3200

//------------------------------------------------------------------------------------
// Declaring all variables

//they should all be declared in the function.h file

//------------------------------------------------------------------------------------
//template for switch states

/*
enum NAMEHERE{start1, reset1};
int DIFFERENTNAMEHERE(int state){

    switch(state){ // State transitions
      
      default:
      break;

      case start1:

        break;

      case RESET:

        break;
        
    }
    switch(state){ // State Action
      case start1:

        break;

      case IDLE1:
        break;

      case RESET:

        break;  
    }

    return state;
}
*/

//------------------------------------------------------------------------------------

/*
void setup() {               
  pinMode(8, OUTPUT); //8 = direction, 9 = step
  pinMode(9, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}

void loop() {
  digitalWrite(9, HIGH);
  delay(1);        
  digitalWrite(9, LOW);
  delay(1);         
}
*/

//------------------------------------------------------------------------------------

//AccelStepper stepper(AccelStepper::DRIVER, motor1StepPin, motor1DirPin);

const long xhigh = 180;
const long xlow = 1;
const long yhigh = 180;
const long ylow = 2;

void setup() {

  Serial.begin(9600);

  laserSetUp(); //look at laserButton.cpp for setup
  stepperSetUp(); //look at joystick.cpp for setup
  reloadSetUp(); //look at reload.cpp for setup

  //setupTest();

}

// a lot of the code that you can test for the individual parts will be testXXXX() in each of their own files (will be the last function given)
// will only call functions from other files and loop them continuously
void loop() {


  laserButton(); //laserButton.cpp

  //this will loop and run the stepper motors based on joystick position
  moveStepper1();
  moveStepper2();
  stepper1.runSpeed(); // Move the X-axis motor at the specified speed (if any)
  stepper2.runSpeed(); // Move th

  reloadStepper(); //reload.cpp

  //random testing functions
  //testLaserButton();
  //moveOneRotation();
  //moreTest();
  //testJoystick();
 
}


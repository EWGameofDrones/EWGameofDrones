/* GODs Summer 2023 - Will, Eddy, Matthew, Malia, Kayla
*
* Description: This file creates directions for the joystick using the direction variable will return what direction the user is pushing 
* the joystick in (does not account for the diagonals - will show just as up, down, left, right). This also contains the code o use the
* stepper motors (the big ones) with the easy driver (NOTE: DO NOT USE BIG EASY DRIVERS!!)
* 
* green, red, blue, yellow
*/

//------------------------------------------------------------------------------------

#include <Arduino.h>
#include "function.h"
#include <AccelStepper.h>
#include <Stepper.h>

#define x_pin A0
#define y_pin A1

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int direction = 0;

const int motor1StepPin = 9; //all shifted two down due to laser pin
const int motor1DirPin = 8;
const int motor2StepPin = 11;
const int motor2DirPin = 10;
const int joystickButton = 7;

const long xhigh = 180;
const long xlow = 1;
const long yhigh = 180;
const long ylow = 2;

const int stepsPerRevolution = 200;

//FIXME
const float maxSpeed = 1000.0; // Steps per second (increase for faster movement)
const float acceleration = 500.0; // Steps per second per second (adjust for smoother/faster acceleration)

/* JOYSTICK VALUES

    (x,y)
    UP = y > 1000
    DOWN = y < 50  
    RIGHT = x < 50 
    LEFT = x > 1000

*/

// Create instances of AccelStepper for each motor
AccelStepper stepper1(AccelStepper::DRIVER, motor1StepPin, motor1DirPin);
AccelStepper stepper2(AccelStepper::DRIVER, motor2StepPin, motor2DirPin);

void stepperSetUp(){

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  
  stepper1.setMaxSpeed(maxSpeed);
  stepper1.setAcceleration(acceleration);
  stepper2.setMaxSpeed(maxSpeed);
  stepper2.setAcceleration(acceleration);

  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  //stepper1.setSpeed(100); // Set an initial speed (positive value indicates up direction)
}

void joystickDirection(){

  xValue = analogRead(A0);
  yValue = analogRead(A1);

  if(yValue > 1000){ //up
    direction = 10;
  }
  else if(yValue < 50){ //down
    direction = 20;
  }
  else if(xValue < 50){ //right
    direction = 30;
  }
  else if(xValue > 1000){ //left
    direction = 40;
  }
  else{
    direction = 0;
  }
  return direction;
}

void moveStepper1(){

  joystickDirection();

  int xValue = analogRead(x_pin);
  int yValue = analogRead(y_pin);
  
  if (direction == 10) { // Joystick moved RIGHT out of the "null zone"
    digitalWrite(motor1DirPin, HIGH); // Set direction to move clockwise
  } else if (direction == 20) { // Joystick moved LEFT out of the "null zone"
    digitalWrite(motor1DirPin, LOW); // Set direction to move counterclockwise
  } else {
    // No direction, stop the motor
    digitalWrite(motor1StepPin, LOW);
    return; // Exit the function if joystick is in the null zone
  }

  // Calculate the step delay based on the joystick position
  long XStepDelay = map(xValue, 520, 1023, xhigh, xlow);

  // Perform the stepper motor movement for the correct length of time
  digitalWrite(motor1StepPin, HIGH);
  delayMicroseconds(XStepDelay);
  digitalWrite(motor1StepPin, LOW);
}

long currentStepsMotor2 = 0;
int MAX_STEPS_MOTOR2 = 1200;
int MIN_STEPS_MOTOR2 = -900;

void moveStepper2(){

  int xValue = analogRead(x_pin);
  int yValue = analogRead(y_pin);

  joystickDirection();
  
  if (direction == 30) { // Joystick moved RIGHT out of the "null zone"
    digitalWrite(motor2DirPin, HIGH); // Set direction to move clockwise
  } 
  else if (direction == 40) { // Joystick moved LEFT out of the "null zone"
    digitalWrite(motor2DirPin, LOW); // Set direction to move counterclockwise
  } 
  else { // No direction, stop the motor
    digitalWrite(motor2StepPin, LOW);
    return; // Exit the function if joystick is in the null zone
  }
  
    if (digitalRead(motor2DirPin) == HIGH && currentStepsMotor2 >= MAX_STEPS_MOTOR2) {
      digitalWrite(motor2StepPin, LOW);
      return; // Exit the function if the maximum limit is reached
  }

  // Check if moving counterclockwise and the minimum limit is reached
  if (digitalRead(motor2DirPin) == LOW && currentStepsMotor2 <= MIN_STEPS_MOTOR2) {
    digitalWrite(motor2StepPin, LOW);
    return; // Exit the function if the minimum limit is reached
  }
  
  // Calculate the step delay based on the joystick position
  long YStepDelay = map(yValue, 520, 1023, yhigh, ylow);

  // Perform the stepper motor movement for the correct length of time
  digitalWrite(motor2StepPin, HIGH);
  delayMicroseconds(YStepDelay);
  digitalWrite(motor2StepPin, LOW);

  if (digitalRead(motor2DirPin) == HIGH){
    currentStepsMotor2++;
  } 
  else{
    currentStepsMotor2--;
  }
  

}

void moveOneRotation(){
  stepper1.moveTo(stepsPerRevolution);
  stepper2.moveTo(-stepsPerRevolution);
  while (stepper1.isRunning() || stepper2.isRunning()) {
    stepper1.run();
    stepper2.run();
  }
  delay(1000);

  // Move motor 1 reverse and motor 2 forward for one revolution
  stepper1.moveTo(-stepsPerRevolution);
  stepper2.moveTo(stepsPerRevolution);
  while (stepper1.isRunning() || stepper2.isRunning()) {
    stepper1.run();
    stepper2.run();
  }
  delay(1000);
}

//tests joystick code
void testJoystick(){
  // making sure that the button works in conjunction with the joystick
  if (digitalRead(6) == HIGH){
    Serial.print("WOKRING \n");
    digitalWrite(5, HIGH);
  }
  if(direction == 10){ //up
    digitalWrite(9, HIGH);
    delay(1);
    digitalWrite(9, LOW);
    delay(1);
    stepper1.runSpeed();
    stepper2.runSpeed();
  }
  else if(direction == 20){ //down
    digitalWrite(11,HIGH);
    delay(1);
    digitalWrite(11, LOW);
    delay(1);
  }
  else if(direction == 30){ //right
    digitalWrite(9, HIGH);
    delay(1);
    digitalWrite(9, LOW);
    delay(1);
    digitalWrite(11,HIGH);
    delay(1);
    digitalWrite(11, LOW);
    delay(1);
  }
  else if(direction == 40){ //left
    digitalWrite(9, LOW);
    digitalWrite(11,LOW);
  }
  else{
    digitalWrite(9,LOW);
    digitalWrite(11,LOW);
  }
}

/*
#include <Arduino.h>
#include "function.h"
#include <AccelStepper.h>
#include <Stepper.h>
#include <EEPROM.h> //EEPROM = Electrically Erasable Programmable Read-Only Memory = allows you to read/write data even if power is shut off

#define x_pin A0
#define y_pin A1

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int direction = 0;

const int motor1StepPin =11;
const int motor1DirPin = 10;
const int motor2StepPin = 9;
const int motor2DirPin = 8;
const int joystickButton = 7;

const int motor1StartAddress = 0;
const int motor2StartAddress = sizeof(long); // EEPROM address 1 more than motor1StartAddress

const long xhigh = 180;
const long xlow = 1;
const long yhigh = 180;
const long ylow = 2;

const int stepsPerRevolution = 200;

//FIXME
const float maxSpeed = 1000.0; // Steps per second (increase for faster movement)
const float acceleration = 500.0; // Steps per second per second (adjust for smoother/faster acceleration)

/* JOYSTICK VALUES

    (x,y)
    UP = y > 1000
    DOWN = y < 50  
    RIGHT = x < 50 
    LEFT = x > 1000

*/

// Create instances of AccelStepper for each motor
/*
AccelStepper stepper1(AccelStepper::DRIVER, motor1StepPin, motor1DirPin);
AccelStepper stepper2(AccelStepper::DRIVER, motor2StepPin, motor2DirPin);

void stepperSetUp(){

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  
  stepper1.setMaxSpeed(maxSpeed);
  stepper1.setAcceleration(acceleration);
  stepper2.setMaxSpeed(maxSpeed);
  stepper2.setAcceleration(acceleration);

  //stepper2.setCurrentPosition(0);

  long motor1StartPosition = 0;
  long motor2StartPosition = 0;
  EEPROM.get(motor1StartAddress, motor1StartPosition);
  EEPROM.get(motor2StartAddress, motor2StartPosition);

  stepper1.setCurrentPosition(motor1StartPosition);
  stepper2.setCurrentPosition(motor2StartPosition);

  //stepper1.setSpeed(100); // Set an initial speed (positive value indicates up direction)
}

void joystickDirection(){

  xValue = analogRead(A0);
  yValue = analogRead(A1);

  if(yValue > 1000){ //up
    direction = 10;
  }
  else if(yValue < 50){ //down
    direction = 20;
  }
  else if(xValue < 50){ //right
    direction = 30;
  }
  else if(xValue > 1000){ //left
    direction = 40;
  }
  else{
    direction = 0;
  }
  return direction;
}

void moveStepper1(){
  int xValue = analogRead(x_pin);
  int yValue = analogRead(y_pin);
  
  if (direction == 10) { // Joystick moved RIGHT out of the "null zone"
    digitalWrite(motor1DirPin, HIGH); // Set direction to move clockwise
  } else if (direction == 20) { // Joystick moved LEFT out of the "null zone"
    digitalWrite(motor1DirPin, LOW); // Set direction to move counterclockwise
  } else {
    // No direction, stop the motor
    digitalWrite(motor1StepPin, LOW);
    return; // Exit the function if joystick is in the null zone
  }

  // Calculate the step delay based on the joystick position
  long XStepDelay = map(xValue, 520, 1023, xhigh, xlow);

  // Perform the stepper motor movement for the correct length of time
  digitalWrite(motor1StepPin, HIGH);
  delayMicroseconds(XStepDelay);
  digitalWrite(motor1StepPin, LOW);
}
long currentStepsMotor2 = 0;
int MAX_STEPS_MOTOR2 = 1200;
int MIN_STEPS_MOTOR2 = -900;

void moveStepper2(){
  int xValue = analogRead(x_pin);
  int yValue = analogRead(y_pin);
  
  if (direction == 30) { // Joystick moved RIGHT out of the "null zone"
    digitalWrite(motor2DirPin, HIGH); // Set direction to move clockwise
  } else if (direction == 40) { // Joystick moved LEFT out of the "null zone"
    digitalWrite(motor2DirPin, LOW); // Set direction to move counterclockwise
  } else {
    // No direction, stop the motor
    digitalWrite(motor2StepPin, LOW);
    return; // Exit the function if joystick is in the null zone
  }

    if (digitalRead(motor2DirPin) == HIGH && currentStepsMotor2 >= MAX_STEPS_MOTOR2) {
    digitalWrite(motor2StepPin, LOW);
    return; // Exit the function if the maximum limit is reached
  }

  // Check if moving counterclockwise and the minimum limit is reached
  if (digitalRead(motor2DirPin) == LOW && currentStepsMotor2 <= MIN_STEPS_MOTOR2) {
    digitalWrite(motor2StepPin, LOW);
    return; // Exit the function if the minimum limit is reached
  }

  // Calculate the step delay based on the joystick position
  long YStepDelay = map(yValue, 520, 1023, yhigh, ylow);

  // Perform the stepper motor movement for the correct length of time
  digitalWrite(motor2StepPin, HIGH);
  delayMicroseconds(YStepDelay);
  digitalWrite(motor2StepPin, LOW);

    if (digitalRead(motor2DirPin) == HIGH) {
    currentStepsMotor2++;
  } else {
    currentStepsMotor2--;
  }

}
/*
void moveStepper1(){
  int xValue = analogRead(x_pin);
  int yValue = analogRead(y_pin);
  
  if (direction == 10) { // Joystick moved RIGHT out of the "null zone"
    digitalWrite(motor1DirPin, HIGH); // Set direction to move clockwise
  } else if (direction == 20) { // Joystick moved LEFT out of the "null zone"
    digitalWrite(motor1DirPin, LOW); // Set direction to move counterclockwise
  } else {
    // No direction, stop the motor
    digitalWrite(motor1StepPin, LOW);
    return; // Exit the function if joystick is in the null zone
  }

  // Calculate the step delay based on the joystick position
  long XStepDelay = map(xValue, 520, 1023, xhigh, xlow);

  // Perform the stepper motor movement for the correct length of time
  digitalWrite(motor1StepPin, HIGH);
  delayMicroseconds(XStepDelay);
  digitalWrite(motor1StepPin, LOW);

  // store current position of motor 1 in EEPROM
  long motor1Position = stepper1.currentPosition();
  EEPROM.put(motor1StartAddress, motor1Position);
}
long currentStepsMotor2 = 0;
int MAX_STEPS_MOTOR2 = 1200;
int MIN_STEPS_MOTOR2 = -900;

void moveStepper2(){
  int xValue = analogRead(x_pin);
  int yValue = analogRead(y_pin);
  
  if (direction == 30) { // Joystick moved RIGHT out of the "null zone"
    digitalWrite(motor2DirPin, HIGH); // Set direction to move clockwise
  } else if (direction == 40) { // Joystick moved LEFT out of the "null zone"
    digitalWrite(motor2DirPin, LOW); // Set direction to move counterclockwise
  } else {
    // No direction, stop the motor
    digitalWrite(motor2StepPin, LOW);
    return; // Exit the function if joystick is in the null zone
  }

    if (digitalRead(motor2DirPin) == HIGH && currentStepsMotor2 >= MAX_STEPS_MOTOR2) {
    digitalWrite(motor2StepPin, LOW);
    return; // Exit the function if the maximum limit is reached
  }

  // Check if moving counterclockwise and the minimum limit is reached
  if (digitalRead(motor2DirPin) == LOW && currentStepsMotor2 <= MIN_STEPS_MOTOR2) {
    digitalWrite(motor2StepPin, LOW);
    return; // Exit the function if the minimum limit is reached
  }

  // Calculate the step delay based on the joystick position
  long YStepDelay = map(yValue, 520, 1023, yhigh, ylow);

  // Perform the stepper motor movement for the correct length of time
  digitalWrite(motor2StepPin, HIGH);
  delayMicroseconds(YStepDelay);
  digitalWrite(motor2StepPin, LOW);

  
    if (digitalRead(motor2DirPin) == HIGH) {
    currentStepsMotor2++;
  } else {
    currentStepsMotor2--;
  }

  // store the current position of motor 2 in EEPROM
  long motor2Position = stepper2.currentPosition();
  EEPROM.put(motor2StartAddress, motor2Position);
}
*/
//---------------------------------------------------------------------------------------------
/*
void moveOneRotation(){
  stepper1.moveTo(stepsPerRevolution);
  stepper2.moveTo(-stepsPerRevolution);

  while ((stepper1.isRunning()) || (stepper2.isRunning())) {
    stepper1.run();
    stepper2.run();
  }
  delay(1000);

  // Move motor 1 reverse and motor 2 forward for one revolution
  stepper1.moveTo(-stepsPerRevolution);
  stepper2.moveTo(stepsPerRevolution);
  while (stepper1.isRunning() || stepper2.isRunning()) {
    stepper1.run();
    stepper2.run();
  }
  delay(1000);
}

//tests joystick code
void testJoystick(){
  // making sure that the button works in conjunction with the joystick
  if (digitalRead(6) == HIGH){
    Serial.print("WOKRING \n");
    digitalWrite(5, HIGH);
  }
  if(direction == 10){ //up
    digitalWrite(9, HIGH);
    delay(1);
    digitalWrite(9, LOW);
    delay(1);
    stepper1.runSpeed();
    stepper2.runSpeed();
  }
  else if(direction == 20){ //down
    digitalWrite(11,HIGH);
    delay(1);
    digitalWrite(11, LOW);
    delay(1);
  }
  else if(direction == 30){ //right
    digitalWrite(9, HIGH);
    delay(1);
    digitalWrite(9, LOW);
    delay(1);
    digitalWrite(11,HIGH);
    delay(1);
    digitalWrite(11, LOW);
    delay(1);
  }
  else if(direction == 40){ //left
    digitalWrite(9, LOW);
    digitalWrite(11,LOW);
  }
  else{
    digitalWrite(9,LOW);
    digitalWrite(11,LOW);
  }
}*/

/* GODs Summer 2023 - Will, Eddy, Matthew, Malia, Kayla
*
* Description: the button to detect whether to turn on the laser or not.
* 
*/

//------------------------------------------------------------------------------------

#include <Arduino.h>
#include "function.h"

const int laserButtonPin = 12;
const int laserPin = 6;

int buttonState = 0;
int laserPrev = 0;

bool ledOn = false;

int laserButton1 = digitalRead(laserButtonPin);
int laser1 = digitalRead(laserButtonPin);
int laser2 = digitalRead(laserPin);

void laserSetUp(){

  pinMode(laserButtonPin,INPUT); //LASER BUTTON PIN
  pinMode(laserPin,OUTPUT); //LASER PIN
  digitalWrite(laserButtonPin, LOW); //laser is turned off initally
  
}

void laserButton(){
  /*
  buttonState = digitalRead(laserButtonPin);

  if (buttonState != laserPrev) {
    if (buttonState == LOW) {
      ledOn = !ledOn;
      digitalWrite(laserPin, ledOn ? HIGH : LOW);
    }
    delay(50);
  }
  laserPrev = buttonState;*/
    buttonState = digitalRead(laserButtonPin);

  if (buttonState != laserPrev) {
    if (buttonState == HIGH) { // If the button is pressed
      ledOn = !ledOn; // Toggle the LED state
      digitalWrite(laserPin, ledOn ? HIGH : LOW); // Turn the laser on or off based on ledOn state
    }
    delay(50); // Debounce the button
  }
  laserPrev = buttonState;
}

//test laser and button code
void testLaserButton(){
  
  if(digitalRead(laserButtonPin) == HIGH){
    Serial.print("PLEASE WORK \n");
    digitalWrite(laserPin,HIGH);
  }
  else{
    Serial.print("NO \n");
    digitalWrite(laserPin,LOW);
  }

}

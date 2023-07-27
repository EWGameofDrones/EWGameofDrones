/* GODs Summer 2023 - Will, Eddy, Matthew, Malia, Kayla
*
* Description: the button to detect whether to turn on the laser or not.
* 
*/

//------------------------------------------------------------------------------------

#include <Arduino.h>
#include "function.h"

const int laserButtonPin = 6;
const int laserPin = 5;

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
  
  buttonState = digitalRead(laserButtonPin);

  if (buttonState != laserPrev) {
    if (buttonState == LOW) {
      ledOn = !ledOn;
      digitalWrite(laserPin, ledOn ? HIGH : LOW);
    }
    delay(50);
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
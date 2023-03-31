#include <AltSoftSerial.h>
#include <XBee.h>
#include <Printers.h>
#include <SoftwareSerial.h> //Used for transmitting to the device
#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module

RFID nano; //Create instance
SoftwareSerial portOne(4, 5);  //RX, TX Nano
//SoftwareSerial portTwo(2,3);      //RX, TX XBee

XBee xbee = XBee(); // create the XBee object

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x41C19C0F); // Current Coordinator
//XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x41C1DD6D);
//ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

#define BUZZER1 10
//#define BUZZER1 0 //For testing silently
#define BUZZER2 9

//nano 
boolean tagDetected; //Keeps track of when we've beeped
long lastSeen = 0; //Tracks the time when we last detected a tag
int counter = 0; //Tracks how many times we've read a tag

// Payload 
uint8_t payload2[] = {'H', 'i'};
uint8_t payload[16];
char cstr[16];
int sensorID = 7;
int rssi = 0;
char temp_char;
int randomNumber = random(100, 999);
int packet;
int flow_control;

void setup()
{
  
  Serial.begin(115200);
  Serial.begin(9600);
  portOne.begin(Serial);
  //portTwo.begin(9600);
  //xbee.begin(portTwo);
  delay(1);
//
//  pinMode(BUZZER1, OUTPUT);
//  pinMode(BUZZER2, OUTPUT);
//
//  digitalWrite(BUZZER2, LOW); //Pull half the buzzer to ground and drive the other half.

  while (!Serial); //Wait for the serial port to come online
//
  if (setupNano(38400) == false) //Configure nano to run at 38400bps
  {
    Serial.println(F("Module failed to respond. Please check wiring."));
    while (1); //Freeze!
  }

  nano.setRegion(REGION_NORTHAMERICA); //Set to North America

  nano.setReadPower(500); //Limited read range
  //nano.setReadPower(2700); //You'll need an external power supply for this setting
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling

  nano.startReading(); //Begin scanning for tags

  Serial.println("Go!");
//  ZBTxRequest zbTx = ZBTxRequest(addr64, payload2, sizeof(payload));
//  xbee.send(zbTx);
//
//  lowBeep(); //Indicate no tag found
//  tagDetected = false;
}

// Method that prints out rssi strength indication
void calc_strength(int rssi){
  if(rssi >= -50){
    Serial.print(" Excellent signal");
  } else if(rssi >= -60 && rssi < -50 ) {
    Serial.print(" Very Good signal");
  } else if(rssi >= -70 && rssi < -60) {
    Serial.print(" Good signal");
  } else if(rssi >= -80 && rssi < -70) {
    Serial.print(" Low signal");
  } else if(rssi >= -90 && rssi < -80) {
    Serial.print(" Very Low signal");
  } else {
    Serial.print(" NO signal");
  }
}

// Helper method to stringify a payload
void stringify(int rssi){
  // Turn rssi into a postive int
  rssi = rssi * -1;
  
  String temp = (String) 1;
  temp += (String) sensorID;
  temp +=  (String)rssi;
  packet = temp.toInt();
  
  Serial.print("-- ");
  Serial.print("Packet info: ");
  Serial.print(packet);
  Serial.print(" - ");
  
}

void nanoLoop(){
  Serial.println("Case 1: Nano Loop");
  //portOne.listen();
  if (nano.check() == true) //Check to see if any new data has come in from module
    {
      byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp
      
      if (responseType == RESPONSE_IS_TAGFOUND)
      {
        //Serial.print(F("Tag detected: "));
        rssi = nano.getTagRSSI(); //Get the RSSI for this tag read
        
        calc_strength(rssi);
        
        Serial.print(F(" rssi["));
        Serial.print(rssi);
        Serial.print(F("]"));
        Serial.println("");
  
        if (tagDetected == false) //Beep if we've detected a new tag
        {
          tagDetected = true;
          //highBeep();
        }
        else if (millis() - lastSeen > 250) //Beep every 250ms
        {
          //highBeep();
        }
        lastSeen = millis();
  
      }
    }
  
  if (tagDetected == true && (millis() - lastSeen) > 1000)
  {
    Serial.println(F("No tag found..."));

    tagDetected = false;
    //lowBeep();
  }

  //If user presses a key, pause the scanning
//  if (Serial.available())
//  {
//    nano.stopReading(); //Stop scanning for tags
//
//    Serial.read(); //Throw away character
//    Serial.println("Scanning paused. Press key to continue.");
//    while (!Serial.available());
//    Serial.read(); //Throw away character
//
//    nano.startReading(); //Begin scanning for tags
//  }  
  
  flow_control = 2;
  return;
}
void XBeeLoop(){
  Serial.println("Case 2: Xbee Loop");
  //portTwo.listen();
  stringify(rssi);
      
  itoa(packet, cstr, 10);
  for(int index = 0; index < sizeof(cstr); index++){
    payload[index] = cstr[index];
    //temp_char = (char) payload[index];
    
  }
  
  ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
  xbee.send(zbTx);
  flow_control = 3;
  return;
}

void loop()
{
  
  switch (flow_control) {
    case 1:
      nanoLoop();
      //Serial.println("Case 1: Nano Read");
      //flow_control = 2;  
      break;
    case 2:
      // statements
      XBeeLoop();
      break;
    default:
      // statements
      Serial.println("Case Default: Error");
      flow_control = 1;
      break;
  }
  
}

//Gracefully handles a reader that is already configured and already reading continuously
//Because Stream does not have a .begin() we have to do this outside the library
boolean setupNano(long baudRate)
{
  nano.begin(portOne); //Tell the library to communicate over software serial port

  //nano.enableDebugging();

  //Test to see if we are already connected to a module
  //This would be the case if the Arduino has been reprogrammed and the module has stayed powered
  portOne.begin(baudRate); //For this test, assume module is already at our desired baud rate
  while (!portOne); //Wait for port to open

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while (portOne.available()) portOne.read();

  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
  {
    //This happens if the baud rate is correct but the module is doing a ccontinuous read
    nano.stopReading();

    Serial.println(F("Module continuously reading. Asking it to stop..."));

    delay(1500);
  }
  else
  {
    //The module did not respond so assume it's just been powered on and communicating at 115200bps
    portOne.begin(115200); //Start software serial at 115200

    nano.setBaud(baudRate); //Tell the module to go to the chosen baud rate. Ignore the response msg

    portOne.begin(baudRate); //Start the software serial port, this time at user's chosen baud rate
  }

  //Test the connection
  nano.getVersion();
  if (nano.msg[0] != ALL_GOOD) return (false); //Something is not right

  //The M6E has these settings no matter what
  nano.setTagProtocol(); //Set protocol to GEN2

  nano.setAntennaPort(); //Set TX/RX antenna ports to 1

  return (true); //We are ready to rock
}

void lowBeep()
{
  tone(BUZZER1, 130, 150); //Low C
  //delay(150);
}

void highBeep()
{
  tone(BUZZER1, 2093, 150); //High C
  //delay(150);
}

#include <XBee.h>
#include <Printers.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h> //Used for transmitting to the device
#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module
#include "binary.h"

RFID nano; //Create instance
SoftwareSerial nanoSerial(2, 3); //RX, TX
XBeeWithCallbacks xbee;
AltSoftSerial SoftSerial;

#define BUZZER1 10
//#define BUZZER1 0 //For testing silently
#define BUZZER2 9


#define DebugSerial Serial // using the Arduino serial monitor for debugging
#define XBeeSerial SoftSerial // using software serial for Xbee comms



boolean tagDetected; //Keeps track of when we've beeped
int rssi = 22;
long lastSeen = 0; //Tracks the time when we last detected a tag
int counter = 0; //Tracks how many times we've read a tag

////////////////////////////////////////////////////////////////////////////////
int sensorID = 4; //ONLY LINE OF CODE THAT NEEDS TO BE UPDATED ON EACH SANDWICH
///////////////////////////////////////////////////////////////////////////////

void setup()
{
  
  // Setup debug serial output & we want DebugSerial to poll faster than XbeeSerial
  DebugSerial.begin(115200);
  DebugSerial.println(F("Starting..."));

// Setup XBee serial communication, baud rate MUST match Xbee settings (in XCTU)  
  XBeeSerial.begin(9600);
  xbee.begin(XBeeSerial);
  delay(1);

  // Setup callbacks for debugging
  xbee.onPacketError(printErrorCb, (uintptr_t)(Print*)&DebugSerial);
  xbee.onResponse(printErrorCb, (uintptr_t)(Print*)&DebugSerial);
  xbee.onZBRxResponse(processRxPacket); //if packet is recieved, processRxPacket()

  // Send a first packet right away
  sendDistancePacket();
  
  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);

  digitalWrite(BUZZER2, LOW); //Pull half the buzzer to ground and drive the other half.

  while (!DebugSerial); //Wait for the serial port to come online

  if (setupNano(38400) == false) //Configure nano to run at 38400bps
  {
    DebugSerial.println(F("Module failed to respond. Please check wiring."));
    while (1); //Freeze!
  }

  nano.setRegion(REGION_NORTHAMERICA); //Set to North America

  nano.setReadPower(500); //Limited read range
  //nano.setReadPower(2700); //You'll need an external power supply for this setting
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling

  nano.startReading(); //Begin scanning for tags

  DebugSerial.println("Go!");

  lowBeep(); //Indicate no tag found
  tagDetected = false;
}

void sendDistancePacket() 
{
    DebugSerial.println(F("..Preparing to send.."));
    DebugSerial.println(sensorID);
    DebugSerial.println(rssi);
    // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    txRequest.setAddress64(0x0013A20041C19C0F); // address of coordinator
    //txRequest.setAddress64(0x0013A20041C1DD6D); // address of coordinator2
    
    // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
    AllocBuffer<5> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
    
    // Packet type & payload data items
    packet.append<uint8_t>(1);
    packet.append<int>(sensorID); 
    packet.append<int>(rssi);
    // Note: Can add on another append<> for each additional 
    // data item, but payload bytes must be adjusted. 

    // Note: Must send hour, minute, second instead of now() because 
    // timestamp must be when sensor detected object, not current internal time. 

    // Set payload
    txRequest.setPayload(packet.head, packet.len());

    // Send the packet
    xbee.send(txRequest);
    //DebugSerial.println(" Packet sent");
}

void sendNullPacket() 
{
  // For more in-depth code explaination, read through sendDistancePacket(), as 
  // it is very similar to this fcn.
  
  // This function's purpose is to let MATLAB knwow when the sensor  
  // has not seen something in more than 5 seconds.
  
    ZBTxRequest txRequest;
    txRequest.setAddress64(0x0013A20041C19C0F);

    AllocBuffer<3> packet; 

    packet.append<uint8_t>(2);
    packet.append<int>(sensorID); 

    txRequest.setPayload(packet.head, packet.len());

    xbee.send(txRequest);
}

//////////////////////////////////////////////////////////////
// Arduino should only be receiving a Current Time packet from 
// MATLAB and no one else for no other reason!
void processRxPacket(ZBRxResponse& rx, uintptr_t)
{
  
}


void loop()
{

  // Check the serial port to see if there is a new packet available
  xbee.loop();
  
  if (nano.check() == true) //Check to see if any new data has come in from module
  {
    byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp

    if (responseType == RESPONSE_IS_TAGFOUND)
    {
      DebugSerial.print(F("Tag detected: "));
      DebugSerial.println(counter++);
      
      sendDistancePacket();
      
      if (tagDetected == false) //Beep if we've detected a new tag
      {
        tagDetected = true;
        highBeep();
      }
      else if (millis() - lastSeen > 250) //Beep every 250ms
      {
        highBeep();
      }
      lastSeen = millis();

    }
  }

  if (tagDetected == true && (millis() - lastSeen) > 1000)
  {
    DebugSerial.println(F("No tag found..."));
    //sendNullPacket();
    tagDetected = false;
    lowBeep();
  }

  //If user presses a key, pause the scanning
  if (DebugSerial.available())
  {
    nano.stopReading(); //Stop scanning for tags

    DebugSerial.read(); //Throw away character
    DebugSerial.println("Scanning paused. Press key to continue.");
    while (!DebugSerial.available());
    DebugSerial.read(); //Throw away character

    nano.startReading(); //Begin scanning for tags
  }
}

//Gracefully handles a reader that is already configured and already reading continuously
//Because Stream does not have a .begin() we have to do this outside the library
boolean setupNano(long baudRate)
{
  nano.begin(nanoSerial); //Tell the library to communicate over software serial port

  //nano.enableDebugging();

  //Test to see if we are already connected to a module
  //This would be the case if the Arduino has been reprogrammed and the module has stayed powered
  nanoSerial.begin(baudRate); //For this test, assume module is already at our desired baud rate
  while (!nanoSerial); //Wait for port to open

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while (nanoSerial.available()) nanoSerial.read();

  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
  {
    //This happens if the baud rate is correct but the module is doing a ccontinuous read
    nano.stopReading();

    DebugSerial.println(F("Module continuously reading. Asking it to stop..."));

    delay(1500);
  }
  else
  {
    //The module did not respond so assume it's just been powered on and communicating at 115200bps
    nanoSerial.begin(115200); //Start software serial at 115200

    nano.setBaud(baudRate); //Tell the module to go to the chosen baud rate. Ignore the response msg

    nanoSerial.begin(baudRate); //Start the software serial port, this time at user's chosen baud rate
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

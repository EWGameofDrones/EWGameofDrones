/*// Copyright 2015, Matthijs Kooijman <matthijs@stdin.nl>
//
// Permission is hereby granted, free of charge, to anyone
// obtaining a copy of this document and accompanying files, to do
// whatever they want with them without any restriction, including, but
// not limited to, copying, modification and redistribution.
//
// NO WARRANTY OF ANY KIND IS PROVIDED.
//
// Modified 2021, Noelle Parker <noelle.parker@navy.mil>
//
// GENERAL PURPOSE: 
// This code file sends and receives data to/from other Xbee Routers in the network
// as well as sends and recieves data to/from MATLAB. This code should be uploaded 
// to the Coordinator in the Xbee network and the Coordinator should be serially 
// connected to MATLAB. 
//
// WIRING DESCRIPTION: 
// In this projeect, the Xbee set up to be the Coordinator in the network is 
// attached to a Sparkfun Xbee-Arduino shield. This shield is connected to an 
// Arduino Uno and the whole unit is connected to the computer while running
// MATLAB. There are LED components connected to GND and Analog Pins 1, 2, and 
// 3 on a mini-breadboard.
// WARNING: Digital Pins 2 & 3 need rerouting to 8 & 9 respectively because 
// the pins used for the AltSoftSerial library are NOT the pins used by the 
// Xbee-Arduino library. Use jumper wires to connect these digital pins.  
// 
//*/

#include <XBee.h>
#include <Printers.h>
#include <AltSoftSerial.h>
#include "binary.h"
#include <TimeLib.h>

XBeeWithCallbacks xbee;

AltSoftSerial SoftSerial;

#define MATLABSerial Serial // used to send/receive from MATLAB and/or Arduino serial monitor debugging 
#define XBeeSerial SoftSerial // using software serial for Xbee comms

// Variables for receiving real-world time from MATLAB
double pcTime = 0;
String packetDelivered = "nothing";
int syncTimeVerifyLight = A1; //digital pin 15 
int armVerifyLight = A2; //digital pin 16 
int testLED = 13; //digital pin 13 
int fireVerifyLight = A3;

// Check to make sure the real-world time is only broadcasted once to all other Arduinos

// Variables for sending distance timestamp to MATLAB
String hours = "0";
String minutes = "0"; 
String seconds = "0";
String colon = ":";
String timestamp = "00:00:00"; 
String packetToSend = "nothing";

time_t tS = 0;

// Variables for sending sensor info to MATLAB
String distStr = "0";
String sensorID = "0";
String comma = ",";

String indicator = "N"; //for sending null distance packets
String handshake = "0";
String jamming = "J"; //for sending jamming packet

unsigned long last_handshake_time = 0; 
// Must use this to avoid potential timeout issues with 
// MATLAB and potential latency issues with xbeeloop().


void setup() 
{
  // Define Inputs/Outputs 
  pinMode(syncTimeVerifyLight, OUTPUT);  
  pinMode(armVerifyLight, OUTPUT);  
  pinMode(testLED, OUTPUT);  
  digitalWrite(testLED,LOW);

  // Setup debug serial output & we want MATLABSerial to poll faster than XbeeSerial
  MATLABSerial.begin(115200);
  MATLABSerial.println(F("Starting..."));

  // Setup XBee serial communication, baud rate MUST match Xbee settings (in XCTU)  
  XBeeSerial.begin(9600);
  xbee.begin(XBeeSerial);
  delay(1);

  // Setup callbacks for debugging
  xbee.onPacketError(printErrorCb, (uintptr_t)(Print*)&MATLABSerial);
  xbee.onResponse(printErrorCb, (uintptr_t)(Print*)&MATLABSerial);
  xbee.onZBRxResponse(processRxPacket); //if packet is received, processRxPacket()

  // Send a first packet right away
  sendPacket();
}

// Used when receiving system time from MATLAB
void updateCurrTime()
{
  time_t t = now();
  setTime(pcTime);
}

// Pads single digits with a zero to display clock time corrctly 
String str2digits(int number) 
{
  String zero = "0";
  String numStr = "00";
  
  if (number >= 0 && number < 10) 
  {
    numStr = zero + String(number);
  }
  else
  {
    numStr = String(number);
  }
  return numStr;

}

void sendToMATLAB()
{
  MATLABSerial.println(handshake);
}

///////////////////////////////////////////////////////////////////
// Coordinator can broadcast a packet containing the current 
// real-world time to all other devices in the network  
void sendPacket() 
{    
    // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    txRequest.setAddress64(0x000000000000FFFF); //specific command for Broacasting

    // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
    AllocBuffer<5> packet; 

    // Packet type & payload data items
    packet.append<uint8_t>(1);
    packet.append<uint32_t>(now()); // broadcast most current time

    // Can add on another append<> for each additional data item, but payload bytes must be adjusted. 
    txRequest.setPayload(packet.head, packet.len());

    // Send the packet
    xbee.send(txRequest);
      
}

void sendArmMissilePacketOn()
{
  // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    //txRequest.setAddress64(0x0013A20041C178C1); // address of coordinator
    txRequest.setAddress64(0x0013A20041C165F8); // address Mario Coordinator
    // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
    AllocBuffer<11> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
    
    // Packet type & payload data items
    packet.append<uint8_t>(1);
    packet.append<int>(9); 
    packet.append<int>(3);
    packet.append<int>(hour(tS)); 
    packet.append<int>(minute(tS));
    packet.append<int>(second(tS));
    // Note: Can add on another append<> for each additional 
    // data item, but payload bytes must be adjusted. 

    // Note: Must send hour, minute, second instead of now() because 
    // timestamp must be when sensor detected object, not current internal time. 

    // Set payload
    txRequest.setPayload(packet.head, packet.len());

    // Send the packet
    xbee.send(txRequest);
}

void sendArmMissilePacketOff()
{
  // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    //txRequest.setAddress64(0x0013A20041C178C1); // address of coordinator
    txRequest.setAddress64(0x0013A20041C165F8); // address Mario Coordinator

    // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
    AllocBuffer<11> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
    
    // Packet type & payload data items
    packet.append<uint8_t>(2);
    packet.append<int>(9); 
    packet.append<int>(3);
    packet.append<int>(hour(tS)); 
    packet.append<int>(minute(tS));
    packet.append<int>(second(tS));
    // Note: Can add on another append<> for each additional 
    // data item, but payload bytes must be adjusted. 

    // Note: Must send hour, minute, second instead of now() because 
    // timestamp must be when sensor detected object, not current internal time. 

    // Set payload
    txRequest.setPayload(packet.head, packet.len());

    // Send the packet
    xbee.send(txRequest);
}

void sendFireMissilePacket()
{
  // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    //txRequest.setAddress64(0x0013A20041C178C1); // address of coordinator
    txRequest.setAddress64(0x0013A20041C165F8); // address Mario Coordinator

    // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
    AllocBuffer<11> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
    
    // Packet type & payload data items
    packet.append<uint8_t>(3);
    packet.append<int>(9); 
    packet.append<int>(4);
    packet.append<int>(hour(tS)); 
    packet.append<int>(minute(tS));
    packet.append<int>(second(tS));
    // Note: Can add on another append<> for each additional 
    // data item, but payload bytes must be adjusted. 

    // Note: Must send hour, minute, second instead of now() because 
    // timestamp must be when sensor detected object, not current internal time. 

    // Set payload
    txRequest.setPayload(packet.head, packet.len());

    // Send the packet
    xbee.send(txRequest);
}

void sendYawNegativePacket()
{
  // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    //txRequest.setAddress64(0x0013A20041C165F8); // address Mario Coordinator
    txRequest.setAddress64(0x0013A20041C178C1); // address Nerf Router 9


    // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
    AllocBuffer<11> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
    
    // Packet type & payload data items
    packet.append<uint8_t>(4);
    packet.append<int>(4); 
    packet.append<int>(4);
    packet.append<int>(hour(tS)); 
    packet.append<int>(minute(tS));
    packet.append<int>(second(tS));
    // Note: Can add on another append<> for each additional 
    // data item, but payload bytes must be adjusted. 

    // Note: Must send hour, minute, second instead of now() because 
    // timestamp must be when sensor detected object, not current internal time. 

    // Set payload
    txRequest.setPayload(packet.head, packet.len());

    // Send the packet
    xbee.send(txRequest);
}

void sendYawPositivePacket()
{
  // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    //txRequest.setAddress64(0x0013A20041C165F8); // address Mario Coordinator
    txRequest.setAddress64(0x0013A20041C178C1); // address Nerf Router 9


    // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
    AllocBuffer<11> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
    
    // Packet type & payload data items
    packet.append<uint8_t>(5);
    packet.append<int>(5); 
    packet.append<int>(5);
    packet.append<int>(hour(tS)); 
    packet.append<int>(minute(tS));
    packet.append<int>(second(tS));
    // Note: Can add on another append<> for each additional 
    // data item, but payload bytes must be adjusted. 

    // Note: Must send hour, minute, second instead of now() because 
    // timestamp must be when sensor detected object, not current internal time. 

    // Set payload
    txRequest.setPayload(packet.head, packet.len());

    // Send the packet
    xbee.send(txRequest);
}

void sendPitchPositivePacket()
{
  // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    //txRequest.setAddress64(0x0013A20041C165F8); // address Mario Coordinator
    txRequest.setAddress64(0x0013A20041C178C1); // address Nerf Router 9


    // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
    AllocBuffer<11> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
    
    // Packet type & payload data items
    packet.append<uint8_t>(7);
    packet.append<int>(7); 
    packet.append<int>(7);
    packet.append<int>(hour(tS)); 
    packet.append<int>(minute(tS));
    packet.append<int>(second(tS));
    // Note: Can add on another append<> for each additional 
    // data item, but payload bytes must be adjusted. 

    // Note: Must send hour, minute, second instead of now() because 
    // timestamp must be when sensor detected object, not current internal time. 

    // Set payload
    txRequest.setPayload(packet.head, packet.len());

    // Send the packet
    xbee.send(txRequest);
}

void sendPitchNegativePacket()
{
  // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    //txRequest.setAddress64(0x0013A20041C165F8); // address Mario Coordinator
    txRequest.setAddress64(0x0013A20041C178C1); // address Nerf Router 9


    // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
    AllocBuffer<11> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
    
    // Packet type & payload data items
    packet.append<uint8_t>(6);
    packet.append<int>(6); 
    packet.append<int>(6);
    packet.append<int>(hour(tS)); 
    packet.append<int>(minute(tS));
    packet.append<int>(second(tS));
    // Note: Can add on another append<> for each additional 
    // data item, but payload bytes must be adjusted. 

    // Note: Must send hour, minute, second instead of now() because 
    // timestamp must be when sensor detected object, not current internal time. 

    // Set payload
    txRequest.setPayload(packet.head, packet.len());

    // Send the packet
    xbee.send(txRequest);
}
//////////////////////////////////////////////////////////////////
// Coordinator should be receiving sensor information from all other devices in network
void processRxPacket(ZBRxResponse& rx, uintptr_t)
{
  // Establish memory buffer 
  Buffer b(rx.getData(), rx.getDataLength()); 

  // Check which kind of packet was sent (valid or null)
    uint8_t type = b.remove<uint8_t>();

  // Note b.len() should be equal to the amount of data bytes expected in the payload   
  if (type == 1 && b.len() == 10) // valid distance packet sent
  {
    // Remove each data item from the payload IN THE ORDER IT WAS SENT and
    // then save them into temp variables
    int sensID = b.remove<int>(); 
    int distance = b.remove<int>();
    int hr = b.remove<int>();
    int mnt = b.remove<int>();
    int scd = b.remove<int>();

    

    // Convert temp variables into strings to sent to MATLAB
    sensorID = String(sensID);
    distStr = String(distance);
   
    // Note: str2digits() pads single digits with a zero for a correct time display 
    hours = str2digits(hr); 
    minutes = str2digits(mnt);
    seconds = str2digits(scd);

    // Concat time variables into single string
    timestamp = hours + colon + minutes + colon + seconds;
    
    // Create a string package where the values in the array are as follows:
    // |Sensor ID| , |Distance to Nearest Object| , |Current Time| 
    packetToSend = sensorID + comma + distStr + comma + timestamp;

    //Send packet to MATLAB
    MATLABSerial.println(packetToSend);
    
    return;
  }
  else if (type == 2 && b.len() == 2) // null packet received
  {
    digitalWrite(testLED, LOW);
    int sensID = b.remove<int>();
    sensorID = String(sensID); 
    
// packet display (0,N or 1,N etc) 
    
    packetToSend = sensorID + comma + indicator;
    
    //Send packet to MATLAB
    MATLABSerial.println(packetToSend);

    return;
  }
  else if (type == 3 && b.len() == 4) // jam packet received
  {
    int sensID = b.remove<int>();
    sensorID = String(sensID); 
    int val = b.remove<int>();
    //Serial.print("Type 3 len 4, TURNING ON LED with val: ");
//    //Serial.println(val);
//    if (val == 1){
//        digitalWrite(testLED, HIGH);
//    }

    
    if (val == 1){
          packetToSend = sensorID + comma + jamming;
    }
    else
        packetToSend = sensorID + comma + indicator;

//    packetToSend = sensorID + comma + jamming;
    
    //Send packet to MATLAB
    MATLABSerial.println(packetToSend);

    return;
  }
    

  MATLABSerial.println(F("Unknown or invalid packet"));
  printResponse(rx, MATLABSerial);
}


void loop() 
{
  // Check the serial port to see if there is a new packet available
  xbee.loop();
  
  
  // Arduino should only receive a Current Time packet from MATLAB if
  // MATLAB is sending something AND the real-world time has not been sent out
  if(MATLABSerial.available())
  {
    // Receive string & then process current time info from MATLAB
    packetDelivered = MATLABSerial.readStringUntil('\n');

    // If packet is a Time Sync packet
    if(packetDelivered.charAt(0) == 'T') 
    {
      //Turn on Read LED
      digitalWrite(syncTimeVerifyLight, HIGH);

      // Process packetDelivered to get pcTime: 
      // Remove header char and comma from time string
      unsigned int removeCount = 2;
      packetDelivered.remove(0, removeCount);

      // Convert packet from string to double
      pcTime = packetDelivered.toDouble();

      // Update internal Arduino clock
      updateCurrTime();

      // Broadcast packet to all other Routers
      sendPacket();

    }
    // If packet is a Arm Missile packet
    else if(packetDelivered.charAt(0) == 'A') 
    {
      // Process packetDelivered to get toggle state: 
      // Remove header char and comma from time string
   
      unsigned int removeCount = 2;
      packetDelivered.remove(0, removeCount);

      // Convert packet from string to double
      double temp = 2;
      temp = packetDelivered.toDouble();

      // If toggle switch in GUI is on or off, turn
      // LED on or off respectively
      if(temp == 1)
      {
        //Turn ON Arm LED
        digitalWrite(armVerifyLight, HIGH);
        sendArmMissilePacketOn();
      }
      else if(temp == 0)
      {
        //Turn OFF Arm LED
        digitalWrite(armVerifyLight, LOW);
        sendArmMissilePacketOff();
      }
      
    }
    // If packet is a Fire Missile packet
    else if(packetDelivered.charAt(0) == 'F') 
    {
      //Turn ON Fire LED
      digitalWrite(fireVerifyLight, HIGH);
      sendFireMissilePacket();
    }
    // If packet is a Stepper Left packet
    else if(packetDelivered.charAt(0) == 'L') 
    {
     
      sendYawNegativePacket();
      
      delay(10); // remove if adding in other code
    }
    // If packet is a Stepper Right packet
    else if(packetDelivered.charAt(0) == 'R') 
    {
     
      sendYawPositivePacket();
      
      delay(10); // remove if adding in other code
    }
     // If packet is a Stepper Up packet
    else if(packetDelivered.charAt(0) == 'U') 
    {
     
      sendPitchPositivePacket();
      
      delay(10); // remove if adding in other code
    }
     // If packet is a Stepper Down packet
    else if(packetDelivered.charAt(0) == 'D') 
    {
     
      sendPitchNegativePacket();
      
      delay(10); // remove if adding in other code
    }
    
  }
  if(millis() - last_handshake_time > 2000)
  {
    sendToMATLAB();

    last_handshake_time = millis();
  }
  else
  {
    // Keep LEDs off otherwise
    digitalWrite(syncTimeVerifyLight, LOW);
    digitalWrite(fireVerifyLight, LOW);

  }  

  // FUTURE WORK: Code the LED to light up when any 
  // object is sensed regardless of threshold.
  
}

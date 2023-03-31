// Copyright 2015, Matthijs Kooijman <matthijs@stdin.nl>
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
// This code file reads in distance values to a Coordinator Xbee 
// that were sent from a router Xbee and then outputs them to 
// the Serial monitor to be displayed in cm.
//

#include <XBee.h>
#include <Printers.h>
#include <AltSoftSerial.h>
#include "binary.h"

XBeeWithCallbacks xbee;

AltSoftSerial SoftSerial;
#define DebugSerial Serial
#define XBeeSerial SoftSerial

String distStr = "0";
String comma = ",";
String sensorID = "NA";
String packetToSend = "nothing";

/*String colon = ":";*/

void setup() 
{
  // Setup debug serial output
  DebugSerial.begin(115200);
  DebugSerial.println(F("Starting..."));

  // Setup XBee serial communication
  XBeeSerial.begin(9600);
  xbee.begin(XBeeSerial);
  delay(1);

  // Setup callbacks
  xbee.onPacketError(printErrorCb, (uintptr_t)(Print*)&DebugSerial);
  xbee.onResponse(printErrorCb, (uintptr_t)(Print*)&DebugSerial);
  xbee.onZBRxResponse(processRxPacket); //if packet is recieved, processRxPacket()
}

void processRxPacket(ZBRxResponse& rx, uintptr_t)
{
  Buffer b(rx.getData(), rx.getDataLength()); 

  if(b.len() == 5)
  {
    // b.len() should be equal to the amount of data bytes expected in the payload
    uint8_t type = b.remove<uint8_t>();
    int sensID = b.remove<int>();
    sensorID = String(sensID);
    int distance = b.remove<int>();
    distStr = String(distance);

    if (type == 1) 
    {
      /*DebugSerial.print(F("Ultrasonic packet received from "));
      printHex(DebugSerial, rx.getRemoteAddress64());
      DebugSerial.println();
      DebugSerial.print(F("Distance: "));
      DebugSerial.print(distStr);
      DebugSerial.println(F(" cm "));
      //keep printing all data items to DebugSerial using b.remove<> IN ORDER ORIGINALLY SENT  
      */ 

      // creating a string package where the values in the array are as follows:
      // |Sensor ID| , |Distance to Nearest Object| , |Current Time| 
      packetToSend = sensorID + comma + distStr;

      //Sending packet
      DebugSerial.println(packetToSend);
      return;
    }
    
  }

  DebugSerial.println(F("Unknown or invalid packet"));
  printResponse(rx, DebugSerial);
}

void loop() 
{
  // Check the serial port to see if there is a new packet available
  xbee.loop();
}

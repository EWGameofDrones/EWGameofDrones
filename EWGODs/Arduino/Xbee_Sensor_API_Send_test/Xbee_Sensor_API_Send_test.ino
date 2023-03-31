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
// This code file reads values from an ultrasonic sensor
// and then sends them to the coordinator Xbee, to be 
// read in by Xbee_Sensor_API_Recieve_test.ino.
//

#include <XBee.h>
#include <Printers.h>
#include <AltSoftSerial.h>
#include "binary.h"

/*//TO DO
//#include <Time.h>
//#include <TimeLib.h>*/

XBeeWithCallbacks xbee;

AltSoftSerial SoftSerial;

#define DebugSerial Serial // using the Arduino serial monitor for debugging
#define XBeeSerial SoftSerial // using software serial for Xbee comms

const int trigPin = 12; //digital pin 12, ultrasonic sensor trig 
const int echoPin = 11; //digital pin 11, ultrasonic sensor echo 

long duration;
int distance;

/*//TO DO
String distStr = "0";
String hours = "0";
String minutes = "0";
String seconds = "0";
String currTime = "00:00:00";*/

int sensorID = 2;

int writeVerifyLight = A2; //digital pin 16, for debugging
int thresholdValue = 80; //comparison value to have the sensor send data to the Xbee

void setup() 
{
  // Define Inputs/Outputs 
  pinMode(writeVerifyLight, OUTPUT); 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  
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

  // Send a first packet right away
  sendPacket();
}

/*//TO DO
void updateCurrTimeStr()
{
  time_t t = now();
  //setTime(t); //will need this when recieving current time from MatLab
  
  hours = String(hour(t));
  minutes = String(minute(t));
  seconds = String(second(t));

  currTime = hours + colon + minutes + colon + seconds;
}*/

void sensorPulse()
{
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Set the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin, return the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
    
  // Calculate the distance
  distance= duration*0.034/2;

  // FUTURE WORK: figure out a way to use interrupts instead of delayMicroseconds()in sensorPulse() 
  // Perhaps implement a RTOS library/code structure to handle packets with priority.
}

void sendPacket() {
    // Prepare the Zigbee Transmit Request API packet
    ZBTxRequest txRequest;
    txRequest.setAddress64(0x0013A20041C165F8);

    // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
    AllocBuffer<5> packet; 
    
    // Packet type & payload data items
    packet.append<uint8_t>(1);
    packet.append<int>(sensorID); 
    packet.append<int>(distance); 

    // Can add on another append<> for each additional data item, but payload bytes must be adjusted. 
    txRequest.setPayload(packet.head, packet.len());

    // Send the packet
    xbee.send(txRequest);
}

unsigned long last_tx_time = 0; //must use to avoid potential latency issues

void loop() {
  //Get new distance from ultrasonic sensor
  sensorPulse();

  // Check the serial port to see if there is a new packet available
  xbee.loop();

  if(distance < thresholdValue && millis() - last_tx_time > 100) 
    {      
      // Turns on the LED
      digitalWrite(writeVerifyLight, HIGH); //not necessary, Sparkfun Shield has onboard debug light
      
      /*//TO DO
      //Prepping the variables needed for sending timestamp
      //updateCurrTimeStr(); 
      //distStr = String(distance) //was using strings to send/parse data */

      sendPacket();

      last_tx_time = millis(); //update time most recent packet sent
      // NOTE: delay() will not work properly with xbee.loop(), do not use.
    }
    else 
    {
      // Keep LED off otherwise
      digitalWrite(writeVerifyLight, LOW); 
    }

    // FUTURE WORK: Code the LED to light up when any object is sensed regardless of threshold.
    // May also want to set up a way to send a value outside the threshold if the sensor 
    // has been idle  for too long to prevent timeouts. 
}

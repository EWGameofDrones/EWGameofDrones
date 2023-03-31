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
  // This code file reads values from an ultrasonic sensor and then sends them to
  // the Coordinator Xbee, to be read in by Xbee_MATLAB_SendAndRecieve.ino. This
  // file also handles receiving the real-world time from the network Coordinator
  // and updating Arduino's internal clock in order to correctly timestamp each
  // distance seen by the sensor.
  //
  // WIRING DESCRIPTION:
  // In this project, the Arduino Uno running this code is part of a "sandwich" unit
  // composed of an Uno, a Sparkfun Xbee-Arduino shield, a Series 3 Xbee, an debug LED
  // component, and an ultrasonic sensor soldered to the PCB portion of the shield.
  // This sandwich is powered by a 9 volt battery, independent of a computer.
  // The sensor has 4 pins that have soldered connections to the Uno via the shield.
  // The 5V voltage and ground pins need to be connected to the sensor's VCC and GND
  // pins, respectively. The sensor's Trig pin needs to be connected to digital pin 12,
  // and the sensor's Echo pin needs to be connected to digital pin 11 on the Uno. The
  // LED should be connected to analog pin A2 and GND on the Uno, although redundant.
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

// Include AccelStepper library and define pulse/Dir pins on arduino for stepper motors
//#include <AccelStepper.h>
//AccelStepper stepper(1, 7, 6); //mode / pulse / dir

//#include <Servo.h>
//Servo myservo;

XBeeWithCallbacks xbee;

AltSoftSerial SoftSerial;

#define DebugSerial Serial // using the Arduino serial monitor for debugging
#define XBeeSerial SoftSerial // using software serial for Xbee comms



const int armVerifyLight = A2; //digital pin 16
const int fireVerifyLight = 13; //digital pin 13
const int pushMotor = 4;
const int pullMotor = 5;
const int StepPinYaw0 = 6;
const int DirPinYaw0 = 11;
const int MS1 = 12;
const int MS2 = 7;

long duration;

time_t t = 0;

// For receiving real-world time & updating Arduino internal clock
String currTime = "00:00:00";
uint32_t currT = 0;

////////////////////////////////////////////////////////////////////////////////
int sensorID = 9; //ONLY LINE OF CODE THAT NEEDS TO BE UPDATED ON EACH SANDWICH
///////////////////////////////////////////////////////////////////////////////

// Variables related to attached hardware (sensor & LED)

int thresholdValue = 80; //comparison value in centimeters to trigger data send when sensor sees object

// Note: If the threshold needs to change after hangar calibration
// testing, you MUST change the gaugeOffset value in the
// readFromXbee.m AND the gauge components in MATLAB GUI to use the right
// limits and display the Tick Labels properly. Refer to the MATLAB
// code for more information.

int moving_distance;
const int Servo_control_pin = 10;

void setup()
{
  // Define Inputs/Outputs
  pinMode(armVerifyLight, OUTPUT);
  pinMode(fireVerifyLight, OUTPUT);
  // Stepper Motor
  pinMode(DirPinYaw0, OUTPUT);
  pinMode(StepPinYaw0, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(DirPinYaw0, LOW);
  digitalWrite(StepPinYaw0, LOW);
  // Linear actuator
  pinMode(pushMotor, OUTPUT);
  pinMode(pullMotor, OUTPUT);
  digitalWrite(pushMotor, LOW);
  digitalWrite(pullMotor, HIGH);
//
//  stepper.setMaxSpeed(3000);
//  stepper.setAcceleration(6000);

//  moving_distance = 12800;
//  stepper.moveTo(moving_distance);

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

  // Set servo position to 150 (range is 0-180 deg - calibrated with current setup
  //90º
  //  for (int i = 0; i < 50; i++) {
  //    digitalWrite(Servo_control_pin, HIGH);
  //    delayMicroseconds(1230);
  //    digitalWrite(Servo_control_pin, LOW);
  //    delayMicroseconds(18780);
}

void updateCurrTimeStr()
{
  setTime(currT);
}



void sendDistancePacket()
{
  // Prepare the Zigbee Transmit Request API packet
  ZBTxRequest txRequest;
  txRequest.setAddress64(0x0013A20041C165F8); // address of coordinator

  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  AllocBuffer<11> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)

  // Packet type & payload data items
  packet.append<uint8_t>(1);
  packet.append<int>(sensorID);
  packet.append<int>(123);
  packet.append<int>(hour(t));
  packet.append<int>(minute(t));
  packet.append<int>(second(t));
  // Note: Can add on another append<> for each additional
  // data item, but payload bytes must be adjusted.

  // Note: Must send hour, minute, second instead of now() because
  // timestamp must be when sensor detected object, not current internal time.

  // Set payload
  txRequest.setPayload(packet.head, packet.len());

  // Send the packet
  xbee.send(txRequest);
}

void sendNullPacket()
{
  // For more in-depth code explaination, read through sendDistancePacket(), as
  // it is very similar to this fcn.

  // This function's purpose is to let MATLAB knwow when the sensor
  // has not seen something in more than 5 seconds.

  ZBTxRequest txRequest;
  txRequest.setAddress64(0x0013A20041C165F8);

  AllocBuffer<3> packet;

  packet.append<uint8_t>(2);
  packet.append<int>(sensorID);

  txRequest.setPayload(packet.head, packet.len());

  xbee.send(txRequest);
}

//////////////////////////////////////////////////////////////
// Arduino should only be receiving a Current Time packet from
// MATLAB and no one else for no other reason!
void processRxPacket(ZBRxResponse & rx, uintptr_t)
{
  // Establish memory buffer
  Buffer b(rx.getData(), rx.getDataLength());

  // Note b.len() should be equal to the amount of data bytes expected in the payload
  if (b.len() == 5) //this number MUST match send allocation buffer
  {
    // Remove each data item from the payload and then save them into temp variables
    uint8_t type = b.remove<uint8_t>();
    currT = b.remove<uint32_t>();

    // Cast time received from MATLAB as a double to feed into the setTime() fcn
    currT = (double)currT;

    if (type == 1)
    {
      // Update Arduino internal clock
      updateCurrTimeStr();
      return;
    }

  }
  else if (b.len() == 11)
  {
    // Remove each data item from the payload and then save them into temp variables
    uint8_t type = b.remove<uint8_t>();

    if (type == 1)
    {

      digitalWrite(armVerifyLight, HIGH);
      return;
    }

    if (type == 2)
    {

      digitalWrite(armVerifyLight, LOW);
      return;
    }
    if (type == 4)
    {
      digitalWrite(StepPinYaw0, HIGH);
      delay(10);
      digitalWrite(StepPinYaw0, LOW);
      delay(10);
    }

    if (type == 3)
    {
      digitalWrite(pushMotor, HIGH);
      digitalWrite(pullMotor, LOW);
      delay(250);
      digitalWrite(pushMotor, LOW);
      digitalWrite(pullMotor, HIGH);
    }
    else {
      digitalWrite(pushMotor, LOW);
      digitalWrite(pullMotor, HIGH);
      delay(2000);
    }


  }

  DebugSerial.println(F("Unknown or invalid packet"));
  printResponse(rx, DebugSerial);
}

unsigned long last_tx_time = 0; //must use to avoid potential latency issues with xbeeloop()

void loop()
{
  // Check the serial port to see if there is a new packet available
  xbee.loop();



  // FUTURE WORK: Code the LED to light up when any object is sensed regardless of threshold.

}

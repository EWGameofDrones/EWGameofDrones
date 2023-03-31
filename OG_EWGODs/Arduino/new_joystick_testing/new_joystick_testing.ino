#include <Time.h>
#include <TimeLib.h>
#include <AltSoftSerial.h>
#include <XBee.h>
#include <Printers.h>
#include <SoftwareSerial.h> //Used for transmitting to the device
#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module
#include "binary.h"

//DEFINE
#define joyX A2
#define joyY A1
#define JOYSTICK_BUTTON_PIN 14
#define BUTTON_ON_PIN 17
#define BUTTON_OFF_PIN 18
#define BUTTON_FIRE_PIN 19 

// create the XBee object
XBeeWithCallbacks xbee;
AltSoftSerial SoftSerial;

#define DebugSerial Serial // used to send/receive from MATLAB and/or Arduino serial monitor debugging 
#define XBeeSerial SoftSerial // using software serial for Xbee comms

// JOYSTICK_BUTTON_PIN Variables will change:
int lastJoystickBtnState = LOW;  // the previous state from the input pin
int currentJoystickBtnState;
// BUTTON_2_PIN Variables will change:
int lastBtnOnState = LOW;  // the previous state from the input pin
int currentBtnOnState;
int lastBtnOffState = LOW;
int currentBtnOffState;
int currentBtnFireState;
int lastBtnFireState = LOW;

XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x41C1646A); // Current CoordinatorA

//nano 
long lastSeen = 0; //Tracks the time when we last detected a tag

// Payload 
int sensorID = 9;
int rssi = 0;
time_t t = 0;

// For receiving real-world time & updating Arduino internal clock
String currTime = "00:00:00";
uint32_t currT = 0;

//arm
int armCounter = 3;

void zbReceive(ZBRxResponse& rx, uintptr_t) {
  // Establish memory buffer 
  Buffer b(rx.getData(), rx.getDataLength()); 

  // Note b.len() should be equal to the amount of data bytes expected in the payload
  if(b.len() == 5) //this number MUST match send allocation buffer 
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

  DebugSerial.println(F("Unknown or invalid packet"));
  printResponse(rx, DebugSerial);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_ON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OFF_PIN, INPUT_PULLUP);

  DebugSerial.begin(115200);
  
  XBeeSerial.begin(9600);
  xbee.setSerial(XBeeSerial);
  delay(1);

  // Make sure that any errors are logged to Serial. The address of
  // Serial is first cast to Print*, since that's what the callback
  // expects, and then to uintptr_t to fit it inside the data parameter.
  xbee.onPacketError(printErrorCb, (uintptr_t)(Print*)&DebugSerial);
  xbee.onTxStatusResponse(printErrorCb, (uintptr_t)(Print*)&DebugSerial);
  xbee.onZBRxResponse(zbReceive);


  while (!DebugSerial); //Wait for the serial port to come online
  DebugSerial.println("Go!");


}

void updateCurrTimeStr()
{
  setTime(currT); 
}

void xbeeSendPacket(int current_rssi){
  DebugSerial.println("Sending packet");
  
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C1646A); // address of actual coordinator

  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  AllocBuffer<11> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
  
  // Packet type & payload data items
  packet.append<uint8_t>(1);
  packet.append<int>(sensorID);
  packet.append<int>(current_rssi);
  packet.append<int>(hour(t)); 
  packet.append<int>(minute(t));
  packet.append<int>(second(t));
  
  // Set payload
  txRequest.setPayload(packet.head, packet.len());

  // Send the packet
  xbee.send(txRequest);
  
}

void xbeeSendYawPacket(int pin, int dir){
  DebugSerial.println("Sending Yaw packet");
  
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C1646A); // address of actual coordinator

  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  AllocBuffer<5> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
  
  // Packet type & payload data items
  packet.append<uint8_t>(1);
  packet.append<int>(pin);
  packet.append<int>(dir); //direction
  
  // Set payload
  txRequest.setPayload(packet.head, packet.len());

  // Send the packet
  xbee.send(txRequest);
  
}

void xbeeSendPitchPacket(int pin, int dir){
  DebugSerial.println("Sending Pitch packet");
  
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C1646A); // address of actual coordinator

  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  AllocBuffer<5> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
  
  // Packet type & payload data items
  packet.append<uint8_t>(1);
  packet.append<int>(pin);
  packet.append<int>(dir); //direction
  
  // Set payload
  txRequest.setPayload(packet.head, packet.len());

  // Send the packet
  xbee.send(txRequest);
  
}
void xbeeSendBtnPacket(int pin, int toggle){
  //DebugSerial.println("Sending packet");
  if(toggle ==1){
    DebugSerial.println("Sending ON packet");
  } else {
    DebugSerial.println("Sending OFF packet");
  }
  
  
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C1646A); // address of actual coordinator

  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  AllocBuffer<5> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
  
  // Packet type & payload data items
  packet.append<uint8_t>(1);
  packet.append<int>(pin);
  packet.append<int>(toggle);
  
  // Set payload
  txRequest.setPayload(packet.head, packet.len());

  // Send the packet
  xbee.send(txRequest);
  
}

void xbeeSendFirePacket(int pin){
  int temp = 13;
  DebugSerial.println("Sending FIRE packet");
  
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C1646A); // address of actual coordinator

  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  AllocBuffer<5> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
  
  // Packet type & payload data items
  packet.append<uint8_t>(1);
  packet.append<int>(pin);
  packet.append<int>(temp);
  
  // Set payload
  txRequest.setPayload(packet.head, packet.len());

  // Send the packet
  xbee.send(txRequest);
  
}
void xbeeSendArmPacket(int pin){
  int temp = armCounter % 2;
  
  DebugSerial.println("Sending ARM packet");
  DebugSerial.println("TEMP: ");
  DebugSerial.print(temp);
  
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C1646A); // address of actual coordinator

  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  AllocBuffer<5> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
  
  // Packet type & payload data items
  packet.append<uint8_t>(1);
  packet.append<int>(pin);
  packet.append<int>(temp);
  
  // Set payload
  txRequest.setPayload(packet.head, packet.len());
  // Send the packet
  xbee.send(txRequest);
  armCounter++;
  
}
void xbeeSendNullPacket(){
  // For debugging purposes 
  DebugSerial.println("Sending NULL packet");
  
  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  // Prepare the Zigbee Transmit Request API packet
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C1646A); // address of actual coordinator


  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  AllocBuffer<3> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
  
  // Packet type & payload data items
  packet.append<uint8_t>(2);
  packet.append<int>(sensorID); 

  // Set payload
  txRequest.setPayload(packet.head, packet.len());

  // Send the packet
  xbee.send(txRequest);

}

unsigned long last_tx_time = 0; //must use to avoid potential latency issues with xbeeloop()


void loop() {
  xbee.loop();
  
  //JOYSTICK AXIS
  // put your main code here, to run repeatedly:
  int xValue = analogRead(joyX);
  int yValue = analogRead(joyY);
  //int oValue = digitalRead(joyO);

  if( xValue > 600 || xValue < 400) {
    Serial.print("X Value: ");
    Serial.println(xValue);
    if(xValue > 600){
      xbeeSendYawPacket(11, 1);
    } else {
      xbeeSendYawPacket(11, 0);
    }
  }
  if( yValue > 600 || yValue < 400) {
    Serial.print("Y Value: ");
    Serial.println(yValue);
    if(yValue > 600){
      xbeeSendPitchPacket(15, 1);
      
    }else {
      xbeeSendPitchPacket(15, 0);
    }
  }

  //JOYSTIC BTN
  // read the state of the switch/button:
  currentJoystickBtnState = digitalRead(JOYSTICK_BUTTON_PIN);

  if(lastJoystickBtnState == HIGH && currentJoystickBtnState == LOW){
    Serial.println("The joystick button is pressed");
    xbeeSendArmPacket(JOYSTICK_BUTTON_PIN);
  }
  else if(lastJoystickBtnState == LOW && currentJoystickBtnState == HIGH){
    Serial.println("The joystick button is released");
  }

  // save the the last state
  lastJoystickBtnState = currentJoystickBtnState;
  //END JOYSTICK BTN LOGIC

  //BUTTON_ON_PIN
  // read the state of the switch/button:
  currentBtnOnState = digitalRead(BUTTON_ON_PIN);

  if(lastBtnOnState == LOW && currentBtnOnState == HIGH){
    Serial.println("The button on is pressed");
    xbeeSendBtnPacket(13, 1); 
  }
  else if(lastBtnOnState == HIGH && currentBtnOnState == LOW){
    Serial.println("The button on is released");
  }
  
  // save the the last state
  lastBtnOnState = currentBtnOnState;
  //END BTN_ON_PIN

  //BUTTON_OFF_PIN
  // read the state of the switch/button:
  currentBtnOffState = digitalRead(BUTTON_OFF_PIN);

  if(lastBtnOffState == LOW && currentBtnOffState == HIGH){
    Serial.println("The button off is pressed");
    xbeeSendBtnPacket(13, 0); 
  }
  else if(lastBtnOffState == HIGH && currentBtnOffState == LOW){
    Serial.println("The button off is released");
  }
  
  // save the the last state
  lastBtnOffState = currentBtnOffState;
  //END BTN_OFF_PIN

  //BUTTON_FIRE_PIN
  // read the state of the switch/button:
  currentBtnFireState = digitalRead(BUTTON_FIRE_PIN);

  if(lastBtnFireState == LOW && currentBtnFireState == HIGH){
    Serial.println("The button FIRE is pressed");
    xbeeSendFirePacket(BUTTON_FIRE_PIN);
  }
  else if(lastBtnFireState == HIGH && currentBtnFireState == LOW){
    Serial.println("The button FIRE is released");
  }
  
  // save the the last state
  lastBtnFireState = currentBtnFireState;
  //END BTN_FIRE_PIN

  

}

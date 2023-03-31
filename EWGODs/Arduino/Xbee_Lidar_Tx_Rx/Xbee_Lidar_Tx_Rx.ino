#include <AltSoftSerial.h>
#include <Time.h>
#include <TimeLib.h>
#include <XBee.h>
#include <Printers.h>
#include <SoftwareSerial.h>
#include <TFMini.h>
#include "binary.h"

TFMini tfmini;

SoftwareSerial SerialTFMini(12, 13);

// create the XBee object
XBeeWithCallbacks xbee;
AltSoftSerial SoftSerial;

#define DebugSerial Serial // used to send/receive from MATLAB and/or Arduino serial monitor debugging 
#define XBeeSerial SoftSerial // using software serial for Xbee comms

XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x41C165F8); // Current Actual Coordinator

// Payload 
int sensorID = 4;
int distance = 0;
time_t t = 0;

long lastSeen = 0; //Tracks the time when we last detected a tag

// For receiving real-world time & updating Arduino internal clock
String currTime = "00:00:00";
uint32_t currT = 0;



int getTFminiData() {
  static char i = 0;
  char j = 0;
  int checksum = 0; 
  static int rx[9];
  int distance = 0;
  if(SerialTFMini.available())
  {  
    // Serial.println( "tfmini serial available" );
    rx[i] = SerialTFMini.read();
    if(rx[0] != 0x59) {
      i = 0;
    } else if(i == 1 && rx[1] != 0x59) {
      i = 0;
    } else if(i == 8) {
      for(j = 0; j < 8; j++) {
        checksum += rx[j];
      }
      if(rx[8] == (checksum % 256)) {
        distance = rx[2] + rx[3] * 256;
      }
      i = 0;
    } else 
    {
      i++;
    } 
  }  

  return distance;
}

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
  pinMode(10, OUTPUT);//Used to trigger
  int ledPin = 10;
  
  // Step 1: Initialize hardware serial port (serial debug port)
  //Serial.begin(115200);
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
  // wait for serial port to connect. Needed for native USB port only
  while (!Serial);
     
  DebugSerial.println ("Initializing...");

  // Step 2: Initialize the data rate for the SoftwareSerial port
  SerialTFMini.begin(TFMINI_BAUDRATE);

  // Step 3: Initialize the TF Mini sensor
  tfmini.begin(&SerialTFMini);    

}

void updateCurrTimeStr()
{
  setTime(currT); 
}

void xbeeSendPacket(int distance){
  DebugSerial.println("Sending packet");
  
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C165F8); // address of actual coordinator

  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  AllocBuffer<11> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
  
  // Packet type & payload data items
  packet.append<uint8_t>(1);
  packet.append<int>(sensorID);
  packet.append<int>(distance);
  packet.append<int>(hour(t)); 
  packet.append<int>(minute(t));
  packet.append<int>(second(t));
  
  // Set payload
  txRequest.setPayload(packet.head, packet.len());

  // Send the packet
  xbee.send(txRequest);
  
}

void xbeeSendNullPacket(){
  // For debugging purposes 
  DebugSerial.println("Sending NULL packet");
  
  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  // Prepare the Zigbee Transmit Request API packet
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C165F8); // address of actual coordinator


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
// **** Sending packet on an infinite loop ****
void loop() {
  xbee.loop();
  
  distance = getTFminiData();
  
  if(distance && (millis() - lastSeen) > 100) {
      xbeeSendPacket(distance);
      DebugSerial.print(distance);
      DebugSerial.println("cm\t");
      
      distance = 0;
      
      // Timestamp when distance was received
      t = now();
      
      lastSeen = millis();
      
    }
    
  if((millis() - lastSeen) > 3000){
    xbeeSendNullPacket();
    lastSeen = millis();
  } 

  //delay(100);

}

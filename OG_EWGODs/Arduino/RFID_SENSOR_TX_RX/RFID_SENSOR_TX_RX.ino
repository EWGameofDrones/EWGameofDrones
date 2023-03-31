#include <Time.h>
#include <TimeLib.h>
#include <AltSoftSerial.h>
#include <XBee.h>
#include <Printers.h>
#include <SoftwareSerial.h> //Used for transmitting to the device
#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module
#include "binary.h"
#include "pitches.h"

RFID nano; //Create instance
SoftwareSerial NanoSoftSerial(4, 5);  //RX, TX Nano

//XBee xbee = XBee(); // create the XBee object
// create the XBee object
XBeeWithCallbacks xbee;
AltSoftSerial SoftSerial;

#define DebugSerial Serial // used to send/receive from MATLAB and/or Arduino serial monitor debugging 
#define XBeeSerial SoftSerial // using software serial for Xbee comms

// SH + SL Address of receiving XBee
//XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x41C19C0F); // Current Test Coordinator
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x41C165F8); // Current Actual Coordinator
//XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x41C1DD6D); // Other Test Coordinator
//ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
//ZBTxStatusResponse txStatus = ZBTxStatusResponse();

//nano 
long lastSeen = 0; //Tracks the time when we last detected a tag

// Payload 
int sensorID = 8;
int rssi = 0;
time_t t = 0;

// For receiving real-world time & updating Arduino internal clock
String currTime = "00:00:00";
uint32_t currT = 0;

#define BUZZER1 13

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

void setup()
{
  
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

  pinMode(BUZZER1, OUTPUT);
  


  while (!DebugSerial); //Wait for the serial port to come online
  if (setupNano(38400) == false) //Configure nano to run at 38400bps
  {
    DebugSerial.println(F("Module failed to respond. Please check wiring."));
    while (1); //Freeze!
  }

  nano.setRegion(REGION_NORTHAMERICA); //Set to North America

  nano.setReadPower(1500); //Limited read range
  //nano.setReadPower(2700); //You'll need an external power supply for this setting
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling

  nano.startReading(); //Begin scanning for tags
  DebugSerial.println("Go!");
}

// Method that prints out rssi strength indication
// For debugging
void calc_strength(int rssi){
  if(rssi >= -50){
    DebugSerial.print(" Excellent signal");
  } else if(rssi >= -60 && rssi < -50 ) {
    DebugSerial.print(" Very Good signal");
  } else if(rssi >= -70 && rssi < -60) {
    DebugSerial.print(" Good signal");
  } else if(rssi >= -80 && rssi < -70) {
    DebugSerial.print(" Low signal");
  } else if(rssi >= -90 && rssi < -80) {
    DebugSerial.print(" Very Low signal");
  } else {
    DebugSerial.print(" NO signal");
  }
}

void updateCurrTimeStr()
{
  setTime(currT); 
}

void xbeeSendPacket(int current_rssi){
  DebugSerial.println("Sending packet");
  
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C165F8); // address of actual coordinator

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

void loop()
{
  xbee.loop();
  if (nano.check() == true) //Check to see if any new data has come in from module
    {
      byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp
      if (responseType == RESPONSE_IS_KEEPALIVE)
      {
        Serial.println(F("Scanning"));
      }
      else if (responseType == RESPONSE_IS_TAGFOUND ) //&& (millis() - lastSeen) > 100
      {
        //Serial.print(F("Tag detected: "));
        int actual_rssi = nano.getTagRSSI(); //Get the RSSI for this tag read
        
        calc_strength(actual_rssi);
        
        xbeeSendPacket(actual_rssi);

        
        //tone(BUZZER1,NOTE_E6,125);
        //delay(130);
        //tone(BUZZER1,NOTE_G6,125);
        //delay(130);
        //tone(BUZZER1,NOTE_E7,125);
        //delay(130);
        tone(BUZZER1,NOTE_C7,125);
        //delay(130);
        //tone(BUZZER1,NOTE_D7,125);
        //delay(130);
        //tone(BUZZER1,NOTE_G7,125);
        //delay(125);
        //noTone(BUZZER1);
        
        DebugSerial.print(F(" rssi["));
        DebugSerial.print(actual_rssi);
        DebugSerial.print(F("]"));
        DebugSerial.println("");
        
        actual_rssi = 0;
        
        // Timestamp when distance was received
        t = now();
        
        lastSeen = millis();
  
      }
    }
    
  if((millis() - lastSeen) > 3000)
  {
    xbeeSendNullPacket();
   

    // Play Fireball sound
    tone(BUZZER1,NOTE_G4,35);
    //delay(35);
    //tone(BUZZER1,NOTE_G5,35);
    //delay(35);
    //tone(BUZZER1,NOTE_G6,35);
    //delay(35);
    //noTone(BUZZER1);
    lastSeen = millis();
  }
  
}

//Gracefully handles a reader that is already configured and already reading continuously
//Because Stream does not have a .begin() we have to do this outside the library
boolean setupNano(long baudRate)
{
  nano.begin(NanoSoftSerial); //Tell the library to communicate over software serial port

  //nano.enableDebugging();

  //Test to see if we are already connected to a module
  //This would be the case if the Arduino has been reprogrammed and the module has stayed powered
  NanoSoftSerial.begin(baudRate); //For this test, assume module is already at our desired baud rate
  while (!NanoSoftSerial); //Wait for port to open

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while (NanoSoftSerial.available()) NanoSoftSerial.read();

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
    NanoSoftSerial.begin(115200); //Start software serial at 115200

    nano.setBaud(baudRate); //Tell the module to go to the chosen baud rate. Ignore the response msg

    NanoSoftSerial.begin(baudRate); //Start the software serial port, this time at user's chosen baud rate
  }

  //Test the connection
  nano.getVersion();
  if (nano.msg[0] != ALL_GOOD) return (false); //Something is not right

  //The M6E has these settings no matter what
  nano.setTagProtocol(); //Set protocol to GEN2

  nano.setAntennaPort(); //Set TX/RX antenna ports to 1

  return (true); //We are ready to rock
}

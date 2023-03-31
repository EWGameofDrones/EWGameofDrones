#include <Stepper.h>

//#include <NeoSWSerial.h>

//#include <Servo.h>

#include <Time.h>
#include <TimeLib.h>
#include <AltSoftSerial.h>
#include <XBee.h>
#include <Printers.h>
#include <SoftwareSerial.h> //Used for transmitting to the device
#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module
#include "binary.h"



// create the XBee object
XBeeWithCallbacks xbee;
AltSoftSerial SoftSerial;

#define DebugSerial Serial // used to send/receive from MATLAB and/or Arduino serial monitor debugging 
#define XBeeSerial SoftSerial // using software serial for Xbee comms
#define laserPin 13
#define cameraPin 10
#define dcMotorPin 12

//5v stepepr defines
#define A 7
#define B 6
#define C 5
#define D 4
#define NUMBER_OF_STEPS_PER_REV 110

//STEPPER DEFINES 
//Stepper Yaw
//DIR-BLUE | STEP-GREEN | ENABLE-YELLOW

//Stepper Pitch
//DIR-ORANGE | STEP-RED | ENABLE-BROWN

// 
int pos=0;

// stepper control outputs
#define DirPinYaw0    16
#define StepPinYaw0   15
#define EnablePinYaw0 14

#define DirPinPitch0    19
#define StepPinPitch0   18
#define EnablePinPitch0 17

#define stepsPerRevolution 80 //200
#define stepsPerRevolutionPitch 30 //50

//END STEPPER DEFINES

//LASER VARIABLES
int laserState = LOW;
int laserToggle = 1;

// MOTOR VARIABLES
// Motor speeds
int fast = 1;
int medium = 5;
int slow = 10;
//************************************************
int speedYaw = fast; // change motor speeds here
int speedPitch = fast;
//************************************************



XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x41C1646A); // Current CoordinatorA
// 5v STEPPER FUNCTIONS
void write(int a,int b,int c,int d){
  digitalWrite(A,a);
  digitalWrite(B,b);
  digitalWrite(C,c);
  digitalWrite(D,d);
}

void onestep(){
  write(1,0,0,0);
  delay(1);
  write(1,1,0,0);
  delay(1);
  write(0,1,0,0);
  delay(1);
  write(0,1,1,0);
  delay(1);
  write(0,0,1,0);
  delay(1);
  write(0,0,1,1);
  delay(1);
  write(0,0,0,1);
  delay(1);
  write(1,0,0,1);
  delay(1);
}
void reverseStep(){
  write(1,0,0,1);
  delay(1);
  write(0,0,0,1);
  delay(1);
  write(0,0,1,1);
  delay(1);
  write(0,0,1,0);
  delay(1);
  write(0,1,1,0);
  delay(1);
  write(0,1,0,0);
  delay(1);
  write(1,1,0,0);
  delay(1);
  write(1,0,0,0);
  delay(1);
}
// 5V STEPPER MOTOR CLOCKWISE
void fiveVoltCW(){
  int i = 0;
  for(i; i < NUMBER_OF_STEPS_PER_REV; i++){
    onestep();
  }
}
// 5V STEPPER MOTOR COUNTER-CLOCKWISE
void fiveVoltCCW(){
  int i = 0;
  for(i; i < NUMBER_OF_STEPS_PER_REV; i++){
    reverseStep();
  }
}
//STEPPER FUNCTIONS
void YawCCW() {
  digitalWrite(EnablePinYaw0, LOW);    
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(DirPinYaw0, LOW); //CCW?
    digitalWrite(StepPinYaw0, HIGH);
    delay(speedYaw);
    digitalWrite(StepPinYaw0, LOW);
    delay(speedYaw);
  }
}

void YawCW() {
  digitalWrite(EnablePinYaw0, LOW); 
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(DirPinYaw0, HIGH); //CW?
    digitalWrite(StepPinYaw0, HIGH);
    delay(speedYaw);
    digitalWrite(StepPinYaw0, LOW);
    delay(speedYaw);
  }
}

void PitchCCW() {
  digitalWrite(EnablePinPitch0, LOW);
  for (int x = 0; x < stepsPerRevolutionPitch; x++) {
    digitalWrite(DirPinPitch0, LOW);
    digitalWrite(StepPinPitch0, HIGH);
    delay(speedPitch);
    digitalWrite(StepPinPitch0, LOW);
    delay(speedPitch);
  }
}

void PitchCW() {
  digitalWrite(EnablePinPitch0, LOW);
  for (int x = 0; x < stepsPerRevolutionPitch; x++) {
    digitalWrite(DirPinPitch0, HIGH);
    digitalWrite(StepPinPitch0, HIGH);
    delay(speedPitch);
    digitalWrite(StepPinPitch0, LOW);
    delay(speedPitch);
  }
}

void zbReceive(ZBRxResponse& rx, uintptr_t) {

  DebugSerial.println("Packet received....");
  // Establish memory buffer 
  Buffer b(rx.getData(), rx.getDataLength()); 

  // Note b.len() should be equal to the amount of data bytes expected in the payload
  if(b.len() == 5 || b.len() == 3) //this number MUST match send allocation buffer 
  {
    // Remove each data item from the payload and then save them into temp variables    
    uint8_t type = b.remove<uint8_t>();
    
    if (type == 1) 
    {
      // Remove each data item from the payload IN THE ORDER IT WAS SENT and
      // then save them into temp variables
      int pin = b.remove<int>();
      //pin 14 joystick button
      //pin 17 non joystick button
      //Yaw pin 11
      if(pin == 11){
        DebugSerial.println("YAWWWWW!");
        int dir = b.remove<int>();
        if(dir == 1){
          YawCCW();
        } else {
          YawCW();
        }
      }
      else if(pin == 15){
        DebugSerial.println("PITCHHH!!");
        int dir = b.remove<int>();
        if(dir == 1){
          PitchCCW();
        } else {
          PitchCW();
        }
      }
      //ARM
      else if(pin == 14){
        int toggle = b.remove<int>();
        
        if(toggle == 1) {
            DebugSerial.println("ARMED....");
            digitalWrite(dcMotorPin, HIGH);

        } else {
          digitalWrite(dcMotorPin, LOW);
        }
      }
      //SCAN
      else if (pin == 13) {
        
        int toggle = b.remove<int>();
        
        if(toggle == 1) {
            DebugSerial.println("SCANNING...");
            // set initial LED state
            laserState = HIGH;
            digitalWrite(laserPin, laserState);
            digitalWrite(cameraPin, HIGH);

        } else {
          laserState = LOW;
          digitalWrite(laserPin, laserState);
          digitalWrite(cameraPin, LOW);
        }

      }
      //FIRE
      else if (pin == 19) {
        
        DebugSerial.println("FIRE Packet received from PIN: ");
        DebugSerial.println(pin);
        //fire nerf dart
        fiveVoltCW();
        fiveVoltCCW();
      }
      //ignore temp for now not using this
      int temp = b.remove<int>();

      
      DebugSerial.println("Packet received from PIN: ");
      DebugSerial.println(pin);
      return;
    }
    else if (type == 2) {
      DebugSerial.println("NULL Packet received");
    }
    
  }

  DebugSerial.println(F("Unknown or invalid packet"));
  printResponse(rx, DebugSerial);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  DebugSerial.begin(115200);
  
  XBeeSerial.begin(9600);
  xbee.setSerial(XBeeSerial);
  delay(1);

  //Stepper setup
  pinMode(DirPinYaw0, OUTPUT);
  pinMode(StepPinYaw0, OUTPUT);
  pinMode(EnablePinYaw0, OUTPUT);
  
  pinMode(DirPinPitch0, OUTPUT);
  pinMode(StepPinPitch0, OUTPUT);
  pinMode(EnablePinPitch0, OUTPUT);

  digitalWrite(EnablePinYaw0, HIGH); // ENABLE pin set to HIGH disables steppers, LOW enables them
  digitalWrite(EnablePinPitch0, HIGH);

  //servo setup
  //myServo.attach(servoPin);

  // Make sure that any errors are logged to Serial. The address of
  // Serial is first cast to Print*, since that's what the callback
  // expects, and then to uintptr_t to fit it inside the data parameter.
  xbee.onPacketError(printErrorCb, (uintptr_t)(Print*)&DebugSerial);
  xbee.onTxStatusResponse(printErrorCb, (uintptr_t)(Print*)&DebugSerial);
  xbee.onZBRxResponse(zbReceive);

  //Laser setup
  pinMode(laserPin, OUTPUT);
  //Camera setup
  pinMode(cameraPin, OUTPUT);
  //DC Motor setup;
  pinMode(dcMotorPin, OUTPUT);

  // set initial LED state
  digitalWrite(laserPin, laserState);

  //5v stepper setup
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  
  while (!DebugSerial); //Wait for the serial port to come online
  DebugSerial.println("Go!");


}


void xbeeSendPacket(int current_rssi){
  DebugSerial.println("Sending packet");

  int sensorID = 13;
  
  ZBTxRequest txRequest;
  //txRequest.setAddress64(0x0013A20041C19C0F); // address of test coordinator
  txRequest.setAddress64(0x0013A20041C1646A); // address of actual coordinator

  // Allocate <X> payload bytes: 1 type byte plus however many other data bytes needed
  AllocBuffer<5> packet; // <X> = 1 1-byte (uint_8) + 5 2-bytes (integers)
  
  // Packet type & payload data items
  packet.append<uint8_t>(1);
  packet.append<int>(sensorID);
  packet.append<int>(current_rssi);

  
  // Set payload
  txRequest.setPayload(packet.head, packet.len());

  // Send the packet
  xbee.send(txRequest);
  
}
void xbeeSendNullPacket(){
  // For debugging purposes 
  DebugSerial.println("Sending NULL packet");
  int sensorID = 13;
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

}

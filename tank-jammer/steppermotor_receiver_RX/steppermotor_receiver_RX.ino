//receiver for stepper motor 

#include <ArduinoBLE.h>

// Stepper motor pins
#define dir_pitch 2
#define stp_pitch 3
#define dir_yaw 4
#define stp_yaw 5
#define Fire 8
#define Fire_pulse 9
#define MicroStep 12

// Variables for stepper control
int x;
int dly = 50;
int steps = 85;
float xVoltage = 0.0;
float yVoltage = 0.0;

// BLE Service and Characteristics UUIDs (must match transmitter)
BLEService joystickService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEFloatCharacteristic xCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEFloatCharacteristic yCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(stp_yaw, OUTPUT);
  pinMode(dir_yaw, OUTPUT);
  pinMode(stp_pitch, OUTPUT);
  pinMode(dir_pitch, OUTPUT);
  pinMode(MicroStep, OUTPUT);
  pinMode(Fire, INPUT_PULLUP);
  pinMode(Fire_pulse, OUTPUT);
  resetEDPins();

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Start scanning for joystick service
  Serial.println("BLE Stepper Motor Central");
  BLE.scan();
}

void loop() {
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    if (peripheral.localName() == "Joystick Controller") {
      BLE.stopScan();
      controlSteppers(peripheral);
      BLE.scan();
    }
  }
}

void controlSteppers(BLEDevice peripheral) {
  Serial.println("Connecting to Joystick Controller...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect");
    return;
  }

  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed");
    peripheral.disconnect();
    return;
  }

  // Get the characteristics
  BLECharacteristic xChar = peripheral.characteristic("19B10001-E8F2-537E-4F6C-D104768A1214");
  BLECharacteristic yChar = peripheral.characteristic("19B10002-E8F2-537E-4F6C-D104768A1214");

  while (peripheral.connected()) {
    // Read the characteristics
    if (xChar.readValue(&xVoltage, sizeof(xVoltage)) && 
        yChar.readValue(&yVoltage, sizeof(yVoltage))) {
      
      // Debug output
      Serial.print("Received - X: ");
      Serial.print(xVoltage);
      Serial.print(" Y: ");
      Serial.println(yVoltage);

      // Control steppers based on joystick values
      //***** UP ***********
      if ((xVoltage > 1.0 && xVoltage < 2.0) && yVoltage > 3.0) { //xacis no movement, y axis, joystick pointing up
        moveSteppers(LOW, false, true); //stepper motor (pitch direction, yaw direction, both motors) (LOW [pitch is moving up], false [yaw motor not moving], true [both motors are active])
      }
      //***** UP-RIGHT *****
      else if (xVoltage > 3.0 && yVoltage > 3.0) {
        moveSteppers(LOW, HIGH, true);
      }
      //***** RIGHT ********
      else if (xVoltage > 3.0 && (yVoltage > 1.0 && yVoltage < 2.0)) {
        moveSteppers(false, HIGH, false);
      }
      //***** DOWN-RIGHT ***
      else if (xVoltage < 1.0 && yVoltage > 3.0) {
        moveSteppers(HIGH, HIGH, true);
      }
      //***** DOWN *********
      else if ((xVoltage > 1.0 && xVoltage < 2.0) && yVoltage < 1.0) {
        moveSteppers(HIGH, false, true);
      }
      //***** DOWN-LEFT ****
      else if (xVoltage < 1.0 && yVoltage < 1.0) {
        moveSteppers(HIGH, LOW, true);
      }
      //***** LEFT *********
      else if (xVoltage < 1.0 && (yVoltage > 1.0 && yVoltage < 2.0)) {
        moveSteppers(false, LOW, false);
      }
      //***** UP-LEFT ******
      else if (xVoltage < 1.0 && yVoltage > 3.0) {
        moveSteppers(LOW, LOW, true);
      }
    }
    delay(10);
  }
  
  Serial.println("Peripheral disconnected");
}

void moveSteppers(int pitch_direction, int yaw_direction, bool both_motors) {
  if (both_motors) {
    digitalWrite(dir_pitch, pitch_direction);
    digitalWrite(dir_yaw, yaw_direction);
    for (x = 0; x < steps; x++) {
      digitalWrite(stp_pitch, HIGH);
      digitalWrite(stp_yaw, HIGH);
      delayMicroseconds(dly);
      digitalWrite(stp_pitch, LOW);
      digitalWrite(stp_yaw, LOW);
      delayMicroseconds(dly);
    }
  } else if (pitch_direction != false) {
    digitalWrite(dir_pitch, pitch_direction);
    for (x = 0; x < steps; x++) {
      digitalWrite(stp_pitch, HIGH);
      delayMicroseconds(dly);
      digitalWrite(stp_pitch, LOW);
      delayMicroseconds(dly);
    }
  } else if (yaw_direction != false) {
    digitalWrite(dir_yaw, yaw_direction);
    for (x = 0; x < steps; x++) {
      digitalWrite(stp_yaw, HIGH);
      delayMicroseconds(dly);
      digitalWrite(stp_yaw, LOW);
      delayMicroseconds(dly);
    }
  }
}

void resetEDPins() {
  digitalWrite(stp_yaw, LOW);
  digitalWrite(dir_yaw, LOW);
  digitalWrite(stp_pitch, LOW);
  digitalWrite(dir_pitch, LOW);
  digitalWrite(MicroStep, HIGH);
}
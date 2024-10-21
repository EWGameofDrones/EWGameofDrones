#include <ArduinoBLE.h>

// Declare pin functions for stepper motors
#define dir_pitch 2
#define stp_pitch 3
#define dir_yaw 4
#define stp_yaw 5

// BLE Service and Characteristics UUIDs
BLEService joystickService("180D");
BLEFloatCharacteristic xVoltageChar("2A37", BLERead | BLENotify);
BLEFloatCharacteristic yVoltageChar("2A38", BLERead | BLENotify);

// Declare motor control variables
int dly = 500;
int steps = 20;

void setup() {
  Serial.begin(9600);

  // Initialize motor pins
  pinMode(stp_yaw, OUTPUT);
  pinMode(dir_yaw, OUTPUT);
  pinMode(stp_pitch, OUTPUT);
  pinMode(dir_pitch, OUTPUT);

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("MotorReceiver");
  BLE.setAdvertisedService(joystickService);

  // Add characteristics to the service
  joystickService.addCharacteristic(xVoltageChar);
  joystickService.addCharacteristic(yVoltageChar);

  // Start advertising
  BLE.addService(joystickService);
  BLE.advertise();

  Serial.println("BLE Motor Receiver is ready!");
}

void loop() {
  BLEDevice central = BLE.central();  // Wait for a central device to connect

  if (central) {
    Serial.println("Connected to central device");

    while (central.connected()) {
      // Check if the joystick data has been updated
      if (xVoltageChar.written() && yVoltageChar.written()) {
        // Read joystick data from BLE characteristics
        float xVoltage = xVoltageChar.value();
        float yVoltage = yVoltageChar.value();

        // Display received joystick values in Serial Monitor
        Serial.print("Received X Voltage: ");
        Serial.println(xVoltage);
        Serial.print("Received Y Voltage: ");
        Serial.println(yVoltage);

        // Control the stepper motors based on the joystick input
        if ((xVoltage > 1.0 && xVoltage < 2.0) && yVoltage > 3.0) {  // UP
          digitalWrite(dir_pitch, LOW);
          moveStepper(stp_pitch);
        } else if (xVoltage > 3.0 && yVoltage > 3.0) {  // UP-RIGHT
          digitalWrite(dir_pitch, LOW);
          digitalWrite(dir_yaw, HIGH);
          moveStepper(stp_pitch);
          moveStepper(stp_yaw);
        } else if (xVoltage > 3.0 && (yVoltage > 1.0 && yVoltage < 2.0)) {  // RIGHT
          digitalWrite(dir_yaw, HIGH);
          moveStepper(stp_yaw);
        } else if (xVoltage < 1.0 && yVoltage > 3.0) {  // DOWN-RIGHT
          digitalWrite(dir_pitch, HIGH);
          digitalWrite(dir_yaw, HIGH);
          moveStepper(stp_pitch);
          moveStepper(stp_yaw);
        } else if ((xVoltage > 1.0 && xVoltage < 2.0) && yVoltage < 1.0) {  // DOWN
          digitalWrite(dir_pitch, HIGH);
          moveStepper(stp_pitch);
        } else if (xVoltage < 1.0 && yVoltage < 1.0) {  // DOWN-LEFT
          digitalWrite(dir_pitch, HIGH);
          digitalWrite(dir_yaw, LOW);
          moveStepper(stp_pitch);
          moveStepper(stp_yaw);
        } else if (xVoltage < 1.0 && (yVoltage > 1.0 && yVoltage < 2.0)) {  // LEFT
          digitalWrite(dir_yaw, LOW);
          moveStepper(stp_yaw);
        } else if (xVoltage < 1.0 && yVoltage > 3.0) {  // UP-LEFT
          digitalWrite(dir_pitch, LOW);
          digitalWrite(dir_yaw, LOW);
          moveStepper(stp_pitch);
          moveStepper(stp_yaw);
        }
      }
    }
    Serial.println("Disconnected from central device");
  }
}

// Function to move the stepper motor
void moveStepper(int stepPin) {
  for (int x = 0; x < steps; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(dly);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(dly);
  }
}

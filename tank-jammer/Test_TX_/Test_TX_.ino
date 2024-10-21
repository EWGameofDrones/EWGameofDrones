#include <ArduinoBLE.h>

// Declare pin functions for joystick
#define xAxis A0
#define yAxis A1

// Declare variables for functions
int xValue = 0;
int yValue = 0;

// BLE Service and Characteristics UUIDs
BLEService joystickService("180D"); // Create a BLE service
BLEFloatCharacteristic xVoltageChar("2A37", BLERead | BLENotify);  // X-axis characteristic
BLEFloatCharacteristic yVoltageChar("2A38", BLERead | BLENotify);  // Y-axis characteristic

void setup() {
  Serial.begin(9600);

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("JoystickTransmitter");
  BLE.setAdvertisedService(joystickService);

  // Add the characteristics to the service
  joystickService.addCharacteristic(xVoltageChar);
  joystickService.addCharacteristic(yVoltageChar);

  // Add service and start advertising
  BLE.addService(joystickService);
  BLE.advertise();

  Serial.println("BLE Joystick Transmitter is ready!");
}

void loop() {
  // Read joystick values
  xValue = analogRead(xAxis);
  yValue = analogRead(yAxis);

  // Convert the raw data value (0 - 1023) to voltage (0.0V - 5.0V)
  float xVoltage = xValue * (5 / 1024.0);
  float yVoltage = yValue * (5 / 1024.0);

  // Print joystick values for debugging
  Serial.print("X Voltage: ");
  Serial.println(xVoltage);
  Serial.print("Y Voltage: ");
  Serial.println(yVoltage);

  // Write the values to BLE characteristics
  xVoltageChar.writeValue(xVoltage);
  yVoltageChar.writeValue(yVoltage);

  delay(100);  // Small delay to avoid flooding the BLE connection
}

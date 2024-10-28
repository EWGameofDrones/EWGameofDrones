//transmitter for joystick

#include <ArduinoBLE.h>

// Joystick pins
#define xAxis A0
#define yAxis A1

// BLE Service and Characteristics
BLEService joystickService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEFloatCharacteristic xCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEFloatCharacteristic yCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  
  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Set advertised local name and service UUID
  BLE.setLocalName("Joystick Controller");
  BLE.setAdvertisedService(joystickService);

  // Add characteristics to the service
  joystickService.addCharacteristic(xCharacteristic);
  joystickService.addCharacteristic(yCharacteristic);

  // Add service
  BLE.addService(joystickService);

  // Start advertising
  BLE.advertise();
  Serial.println("BLE Joystick Peripheral");
}

void loop() {
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Read joystick values
      int xValue = analogRead(xAxis);
      int yValue = analogRead(yAxis);
      
      // Convert to voltage (0.0V - 5.0V)
      float xVoltage = xValue * (3.3 / 1023.0);
      float yVoltage = yValue * (3.3 / 1023.0);

      // Update characteristics
      xCharacteristic.writeValue(xVoltage);
      yCharacteristic.writeValue(yVoltage);

      // Debug output
      Serial.print("X: ");
      Serial.print(xVoltage);
      Serial.print(" Y: ");
      Serial.println(yVoltage);

      delay(50); // Small delay to prevent flooding
    }
    
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
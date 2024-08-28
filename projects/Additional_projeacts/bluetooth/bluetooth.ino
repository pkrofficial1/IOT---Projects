#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const char *remoteDeviceAddress = "41:42:cf:3d:e8:33";  // Your speaker's MAC address

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Client");
  Serial.println("Attempting to connect...");

  if (SerialBT.connect(remoteDeviceAddress)) {
    Serial.println("Connected to the device successfully!");
  } else {
    Serial.println("Failed to connect to the device.");
  }
}

void loop() {
  if (SerialBT.connected()) {
    Serial.println("Device is connected...");
    delay(2000);
  } else {
    Serial.println("Device is disconnected. Attempting to reconnect...");
    if (SerialBT.connect(remoteDeviceAddress)) {
      Serial.println("Reconnected successfully!");
    } else {
      Serial.println("Reconnection failed.");
    }
    delay(5000);
  }
}

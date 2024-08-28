#include "BluetoothSerial.h"  // Bluetooth Serial library
BluetoothSerial SerialBT;

const int ledPins[5] = {2, 4, 16, 17, 18};  // GPIO pins where LEDs are connected
int currentPattern = 1;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_LED_Control");
  Serial.println("The device started, now you can pair it with Bluetooth!");

  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  if (SerialBT.available()) {
    char receivedChar = SerialBT.read();
    Serial.println(receivedChar);

    switch (receivedChar) {
      case '1':
        currentPattern = 1;
        break;
      case '2':
        currentPattern = 2;
        break;
      case '3':
        currentPattern = 3;
        break;
      case '4':
        currentPattern = 4;
        break;
      case '5':
        currentPattern = 5;
        break;
    }
  }

  // Execute the selected pattern continuously
  runPattern(currentPattern);
}

void runPattern(int pattern) {
  switch (pattern) {
    case 1:
      knightRider();
      break;
    case 2:
      sideWave();
      break;
    case 3:
      alternateBlink();
      break;
    case 4:
      outwardBlink();
      break;
    case 5:
      randomTwinkle();
      break;
  }
}

void knightRider() {
  for (int i = 0; i < 5; i++) {
    if (i != 2) {  // Skip the middle LED
      digitalWrite(ledPins[i], HIGH);
      delay(200);
      digitalWrite(ledPins[i], LOW);
    }
  }
  for (int i = 3; i >= 0; i--) {
    if (i != 2) {  // Skip the middle LED
      digitalWrite(ledPins[i], HIGH);
      delay(200);
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void sideWave() {
  for (int i = 0; i < 5; i++) {
    if (i != 2) {  // Skip the middle LED
      digitalWrite(ledPins[i], HIGH);
      delay(100);
      digitalWrite(ledPins[i], LOW);
    }
  }
  delay(200);
  for (int i = 4; i >= 0; i--) {
    if (i != 2) {  // Skip the middle LED
      digitalWrite(ledPins[i], HIGH);
      delay(100);
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void alternateBlink() {
  for (int i = 0; i < 5; i++) {
    if (i != 2) {  // Skip the middle LED
      digitalWrite(ledPins[i], i % 2 == 0 ? HIGH : LOW);
    }
  }
  delay(500);
  for (int i = 0; i < 5; i++) {
    if (i != 2) {  // Skip the middle LED
      digitalWrite(ledPins[i], i % 2 == 0 ? LOW : HIGH);
    }
  }
  delay(500);
}

void outwardBlink() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledPins[i], HIGH);
    digitalWrite(ledPins[4 - i], HIGH);
    delay(200);
    digitalWrite(ledPins[i], LOW);
    digitalWrite(ledPins[4 - i], LOW);
  }
}

void randomTwinkle() {
  int randomLed = random(0, 5);
  if (randomLed != 2) {  // Skip the middle LED
    digitalWrite(ledPins[randomLed], HIGH);
    delay(200);
    digitalWrite(ledPins[randomLed], LOW);
  }
}

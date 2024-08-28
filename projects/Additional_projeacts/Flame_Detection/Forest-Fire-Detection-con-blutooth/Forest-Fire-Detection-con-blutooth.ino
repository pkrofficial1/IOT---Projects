#include "DHT.h"
#include "BluetoothSerial.h" // Include the BluetoothSerial library

#define FLAME_SENSOR_PIN 34
#define LED_PIN 2 // Built-in LED on most ESP32 boards
#define BUZZER_PIN 19
#define THRESHOLD 500 // Adjust this threshold based on your sensor and environment

#define DHTPIN 18    // GPIO pin for the DHT11 sensor
#define DHTTYPE DHT11   // Define the type of DHT sensor

DHT dht(DHTPIN, DHTTYPE);
BluetoothSerial SerialBT; // Create a BluetoothSerial object

bool alarmActive = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);         // Set LED pin as output
  pinMode(BUZZER_PIN, OUTPUT);      // Set buzzer pin as output
  Serial.begin(115200);             // Initialize serial communication
  SerialBT.begin("ESP32_FireAlarm"); // Initialize Bluetooth with a name
  dht.begin();                      // Initialize DHT11 sensor
}

void loop() {
  int flameValue = analogRead(FLAME_SENSOR_PIN); // Read the analog value from the flame sensor

  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Print temperature and humidity to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Check for user input for stop command via Serial Monitor or Bluetooth
  if (Serial.available() || SerialBT.available()) {
    String input = (Serial.available()) ? Serial.readStringUntil('\n') : SerialBT.readStringUntil('\n');
    input.trim(); // Remove any leading/trailing whitespace

    Serial.print("Received command: ");
    Serial.println(input);

    if (input == "stop") {
      alarmActive = false; // Deactivate alarm
      digitalWrite(LED_PIN, LOW);  // Turn off the LED
      noTone(BUZZER_PIN); // Turn off the buzzer
      Serial.println("Alarm stopped and reset.");
      SerialBT.println("Alarm stopped and reset."); // Notify via Bluetooth
    }
  }

  if (flameValue < THRESHOLD) { // Flame detected if value is below the threshold
    if (!alarmActive) { // If the alarm is not already active
      alarmActive = true; // Activate alarm
      digitalWrite(LED_PIN, HIGH); // Turn on the LED
      Serial.println("Fire!!! Fire!!! Please use the Emergency Exit"); // Notify fire detected
      SerialBT.println("Fire!!! Fire!!! Please use the Emergency Exit"); // Notify via Bluetooth
    }
    playSiren(); // Play the siren sound
  } else {
    if (alarmActive) {
      // Optional: Turn off the alarm automatically if desired
    }
  }

  delay(1000); // Delay for sensor readings and avoid high CPU usage
}

void playSiren() {
  // Create a siren effect by alternating frequencies
  for (int i = 1000; i < 2000; i += 10) {
    tone(BUZZER_PIN, i);
    delay(5);
  }
  for (int i = 2000; i > 1000; i -= 10) {
    tone(BUZZER_PIN, i);
    delay(5);
  }
}

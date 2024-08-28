#define LDR_PIN 34  // Pin connected to the LDR sensor's analog output
#define LED_PIN 23  // Pin connected to the LED

int threshold = 2000;  // Light threshold value to turn the LED on/off

void setup() {
  Serial.begin(115200);  // Initialize serial communication at a baud rate of 115200
  pinMode(LDR_PIN, INPUT);  // Set LDR_PIN as input
  pinMode(LED_PIN, OUTPUT); // Set LED_PIN as output
  digitalWrite(LED_PIN, LOW);  // Make sure the LED is off initially
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);  // Read the analog value from the LDR sensor
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);  // Print the LDR value to the serial monitor

  // Check if the LDR value is below the threshold (indicating darkness)
  if (ldrValue > threshold) {
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn off the LED
  }

  delay(500);  // Wait for 500ms before reading again
}

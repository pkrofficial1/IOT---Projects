
#define LED_PIN 23
#define POT_PIN 34

void setup() {
  pinMode(LED_PIN, OUTPUT);      // Set the LED pin as output
  analogReadResolution(10);      // Set the ADC resolution to 10 bits (0-1023)
  analogSetAttenuation(ADC_11db); // Set attenuation to 11dB (0-3.3V range)
}

void loop() {
  int potValue = analogRead(POT_PIN);          // Read the potentiometer value
  int blinkDelay = map(potValue, 0, 1023, 50, 1000); // Map the value to a delay (50ms to 1000ms)

  digitalWrite(LED_PIN, HIGH); // Turn on the LED
  delay(blinkDelay);           // Wait for the calculated delay
  digitalWrite(LED_PIN, LOW);  // Turn off the LED
  delay(blinkDelay);           // Wait for the calculated delay
}

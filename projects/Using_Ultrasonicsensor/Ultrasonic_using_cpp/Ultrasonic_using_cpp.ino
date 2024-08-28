#define TRIG_PIN 5
#define ECHO_PIN 18
#define LED1_PIN 19
#define LED2_PIN 22
#define LED3_PIN 21
#define BUZZER_PIN 23

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize all LEDs and the buzzer to off
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  noTone(BUZZER_PIN); // Ensure the buzzer is off
}

void loop() {
  long duration, distance;

  // Send a 10us pulse to the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the pulse from the echo pin
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance in cm
  distance = duration * 0.034 / 2;

  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Control the LEDs and the buzzer based on distance
  if (distance >= 30) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    noTone(BUZZER_PIN); // Turn off the buzzer
  } else if (distance >= 11 && distance < 30) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, LOW);
    noTone(BUZZER_PIN); // Turn off the buzzer
  } else if (distance < 10) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    blinkLED3WithEmergencyTone(); // Blink LED3 and play the emergency tone
  } else {
    // Turn off all LEDs and the buzzer if distance is between 10 cm and 20 cm
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    noTone(BUZZER_PIN); // Turn off the buzzer
  }

  delay(100); // Short delay before next measurement
}

void blinkLED3WithEmergencyTone() {
  for (int i = 0; i < 5; i++) { // Blink LED3 and play tone 5 times
    digitalWrite(LED3_PIN, HIGH);
    tone(BUZZER_PIN, 1000); // 1000 Hz tone
    delay(200);
    
    digitalWrite(LED3_PIN, LOW);
    tone(BUZZER_PIN, 1500); // 1500 Hz tone
    delay(200);
  }
  
  noTone(BUZZER_PIN); // Stop the tone
}

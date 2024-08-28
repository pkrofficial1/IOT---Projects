/*
  ESP32 Indian Ambulance Siren with Slower Modulation

  This example demonstrates how to create an Indian ambulance siren sound effect
  with slower modulation using a buzzer connected to an ESP32 using the LEDC module.

  Code adapted from various sources.

  This example code is in the public domain.
*/

// Buzzer PIN (choose a PWM-capable GPIO pin)
#define BUZZER_PIN 2

// Define high and low tone frequencies (Hz)
#define HIGH_TONE_FREQ 1800
#define LOW_TONE_FREQ 800

// Define modulation step size
#define STEP_SIZE 50  // Adjust this value to control the modulation speed

void setup() {
  // Setup timer and attach timer to a buzzer pin
  ledcAttach(BUZZER_PIN, 5000, 12); // 5000 Hz base frequency, 12-bit resolution
}

void loop() {
  // Increase frequency from low to high
  for (int freq = LOW_TONE_FREQ; freq <= HIGH_TONE_FREQ; freq += STEP_SIZE) {
    ledcWriteTone(BUZZER_PIN, freq);
    delay(10); // Adjust delay for smooth modulation
  }

  // Decrease frequency from high to low
  for (int freq = HIGH_TONE_FREQ; freq >= LOW_TONE_FREQ; freq -= STEP_SIZE) {
    ledcWriteTone(BUZZER_PIN, freq);
    delay(10); // Adjust delay for smooth modulation
  }
}

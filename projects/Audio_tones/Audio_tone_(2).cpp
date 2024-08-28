/*
  ESP32 Music Tone Player

  This example demonstrates how to play music tones on a buzzer
  connected to an ESP32 using the LEDC module.

  Code adapted from various sources.

  This example code is in the public domain.
*/

// Define musical notes and their corresponding frequencies
#define NOTE_C4 261
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494

// Buzzer PIN (choose a PWM-capable GPIO pin)
#define BUZZER_PIN 2

void setup() {
  // Setup timer and attach timer to a buzzer pin
  ledcAttach(BUZZER_PIN, 5000, 12); // 5000 Hz base frequency, 12-bit resolution
}

void loop() {
  // Play a simple melody
  playTone(NOTE_C4, 200);
  playTone(NOTE_D4, 200);
  playTone(NOTE_E4, 200);
  playTone(NOTE_F4, 200);
  playTone(NOTE_G4, 200);
  playTone(NOTE_A4, 200);
  playTone(NOTE_B4, 200);
  playTone(NOTE_C4, 200);

  delay(500); // Delay between melodies
}

// Function to play a tone for a specified duration
void playTone(int tone, int duration) {
  ledcWriteTone(BUZZER_PIN, tone); // Set the tone frequency
  delay(duration); // Wait for the duration
  ledcWriteTone(BUZZER_PIN, 0); // Turn off the tone by setting frequency to 0
  delay(20); // Short delay between tones for separation
}

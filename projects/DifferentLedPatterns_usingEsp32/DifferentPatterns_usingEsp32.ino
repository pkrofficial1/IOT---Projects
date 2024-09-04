// Define LED pins
const int led1 = 18;
const int led2 = 19;
const int led3 = 21;
came
// Define push button pin
const int buttonPin = 15;

// Variables to store button state and pattern mode
int buttonState = 0;
int lastButtonState = 0;
int pattern = 0;

void setup() {
  // Initialize LED pins as outputs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Initialize push button pin as input with internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Start serial communication for debugging
  Serial.begin(115200);
}

void loop() {
  // Read the button state
  buttonState = digitalRead(buttonPin);

  // Check if the button is pressed
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Debounce delay
    delay(50);

    // Read the button state again
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
      // Change the pattern
      pattern++;
      if (pattern > 2) {
        pattern = 0; // Reset pattern if it exceeds the number of patterns
      }
      // Print the current pattern to the serial monitor
      Serial.println("Pattern: " + String(pattern));
    }
  }

  // Store the current button state for the next iteration
  lastButtonState = buttonState;

  // Control LED patterns based on the pattern variable
  switch (pattern) {
    case 0:
      pattern1();
      break;
    case 1:
      pattern2();
      break;
    case 2:
      pattern3();
      break;
  }
}

// Function for LED pattern 1
void pattern1() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  delay(500);
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  delay(500);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  delay(500);
}

// Function for LED pattern 2
void pattern2() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  delay(500);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  delay(500);
}

// Function for LED pattern 3
void pattern3() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  delay(500);
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  delay(500);
}

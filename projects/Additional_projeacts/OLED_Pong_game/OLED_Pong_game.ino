#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PADDLE_HEIGHT 8
#define PADDLE_WIDTH 2
#define BALL_SIZE 2
#define PADDLE1_PIN 32
#define PADDLE2_PIN 33
#define BUZZER_PIN 25

int paddle1Y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
int paddle2Y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
int ballX = SCREEN_WIDTH / 2;
int ballY = SCREEN_HEIGHT / 2;
int ballSpeedX = 1;
int ballSpeedY = 1;
int score1 = 0;
int score2 = 0;

void setup() {
  pinMode(PADDLE1_PIN, INPUT_PULLUP);
  pinMode(PADDLE2_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  if (digitalRead(PADDLE1_PIN) == LOW && paddle1Y > 0) {
    paddle1Y--;
  } else if (digitalRead(PADDLE2_PIN) == LOW && paddle1Y < SCREEN_HEIGHT - PADDLE_HEIGHT) {
    paddle1Y++;
  }

  // Update ball position
  ballX += ballSpeedX;
  ballY += ballSpeedY;

  // Ball collision with top and bottom
  if (ballY <= 0 || ballY >= SCREEN_HEIGHT - BALL_SIZE) {
    ballSpeedY = -ballSpeedY;
    tone(BUZZER_PIN, 1000, 100); // Play a tone at 1000 Hz for 100 ms
  }

  // Ball collision with paddles
  if ((ballX <= PADDLE_WIDTH && ballY >= paddle1Y && ballY <= paddle1Y + PADDLE_HEIGHT) || 
      (ballX >= SCREEN_WIDTH - PADDLE_WIDTH - BALL_SIZE && ballY >= paddle2Y && ballY <= paddle2Y + PADDLE_HEIGHT)) {
    ballSpeedX = -ballSpeedX;
    tone(BUZZER_PIN, 1500, 100); // Play a tone at 1500 Hz for 100 ms
  }

  // Ball out of bounds
  if (ballX <= 0) {
    score2++;
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    ballSpeedX = 1;
    ballSpeedY = 1;
    tone(BUZZER_PIN, 500, 500); // Play a tone at 500 Hz for 500 ms
  }
  
  if (ballX >= SCREEN_WIDTH - BALL_SIZE) {
    score1++;
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    ballSpeedX = -1;
    ballSpeedY = 1;
    tone(BUZZER_PIN, 500, 500); // Play a tone at 500 Hz for 500 ms
  }

  // Clear display
  display.clearDisplay();

  // Draw paddles
  display.fillRect(0, paddle1Y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
  display.fillRect(SCREEN_WIDTH - PADDLE_WIDTH, paddle2Y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);

  // Draw ball
  display.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, SSD1306_WHITE);

  // Display scores
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 0);
  display.print("P1: ");
  display.print(score1);
  display.setCursor(SCREEN_WIDTH - 60, 0);
  display.print("P2: ");
  display.print(score2);

  // Update display
  display.display();
  delay(10);
}
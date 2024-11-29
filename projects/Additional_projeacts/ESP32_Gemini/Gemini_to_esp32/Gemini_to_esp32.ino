#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Are-you-watching";
const char* password = "";
const char* Gemini_Token = "";
const char* Gemini_Max_Tokens = "100";
String res = "";

// Define LED pins for ESP32 38-pin
const int yellowLED = 18;
const int greenLED = 19;

void setup() {
  Serial.begin(115200);

  // Setup LEDs as outputs
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  while (!Serial)
    ;

  // wait for WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println("");
  Serial.println("Ask your Question: ");
  while (!Serial.available())
    ;
  while (Serial.available()) {
    char add = Serial.read();
    res = res + add;
    delay(1);
  }
  int len = res.length();
  res = res.substring(0, (len - 1));
  res = "\"" + res + "\"";
  Serial.println("");
  Serial.print("Asking Your Question: ");
  Serial.println(res);

  HTTPClient https;

  // Turn on yellow LED during processing
  digitalWrite(yellowLED, HIGH);

  if (https.begin("  " + (String)Gemini_Token)) {
    https.addHeader("Content-Type", "application/json");
    String payload = String("{\"contents\": [{\"parts\":[{\"text\":" + res + "}]}],\"generationConfig\": {\"maxOutputTokens\": " + (String)Gemini_Max_Tokens + "}}");

    int httpCode = https.POST(payload);

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = https.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      String Answer = doc["candidates"][0]["content"]["parts"][0]["text"];

      // Filter special characters, whitespace, and newlines
      Answer.trim();
      String filteredAnswer = "";
      for (size_t i = 0; i < Answer.length(); i++) {
        char c = Answer[i];
        if (isalnum(c) || isspace(c)) {
          filteredAnswer += c;
        } else {
          filteredAnswer += ' ';
        }
      }
      Answer = filteredAnswer;

      Serial.println("");
      Serial.println("Here is your Answer: ");
      Serial.println("");
      Serial.println(Answer);

      // Turn off yellow LED and turn on green LED once the answer is received
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, HIGH);

    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());

      // In case of error, turn off yellow LED
      digitalWrite(yellowLED, LOW);
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");

    // If connection fails, turn off yellow LED
    digitalWrite(yellowLED, LOW);
  }
  res = "";

  // Turn off green LED after a short delay
  delay(3000);  // Keep green LED on for 3 seconds
  digitalWrite(greenLED, LOW);
}

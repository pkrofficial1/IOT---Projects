#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "pkr";
const char* password = "praveen@123";

// Replace with your server IP address and path
const char* serverName = "http://"system_ip_address"/esp32_api/api1.php";

// DHT Sensor
#define DHTPIN 4 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  dht.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Determine command based on temperature and humidity
    String command = "normal";
    if (temperature > 30.0) {
      command = "hot";
    } else if (humidity > 70.0) {
      command = "cloud";
    }

    // Create JSON payload
    String jsonPayload = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + ", \"command\": \"" + command + "\"}";
    
    Serial.println("Sending payload: " + jsonPayload);

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
  
  delay(10000); // Wait for 10 seconds before sending the next request
}

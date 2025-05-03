#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// --- Servo Setup ---
Servo servos[4];
int angles[4] = {90, 90, 90, 0};
int minAngles[4] = {0, 50, 70, 0};
int maxAngles[4] = {180, 150, 150, 50};

// --- WiFi Credentials ---
const char* ssid = "ESP32-Servo-Control";
const char* password = "12345678";
WebServer server(80);

// --- Recording Structure ---
bool isRecording = false;
unsigned long lastMoveTime = 0;

#define MAX_MOVES 1000  // Increased max moves

struct Movement {
  uint8_t servoIndex;
  uint8_t angle;
  uint16_t timestamp;
};

Movement movements[MAX_MOVES];
int moveCount = 0;

// --- Smooth Move ---
void moveSmooth(int index, int target, int stepDelay = 1) {
  int current = angles[index];
  int step = (target > current) ? 1 : -1;
  for (int pos = current; pos != target; pos += step) {
    servos[index].write(pos);
    delay(stepDelay);
  }
  servos[index].write(target);
  angles[index] = target;
}

// --- Handle Servo Movement ---
void handleSet() {
  if (server.hasArg("servo") && server.hasArg("angle")) {
    int index = server.arg("servo").toInt();
    int val = server.arg("angle").toInt();
    if (index >= 0 && index < 4 && val >= minAngles[index] && val <= maxAngles[index]) {
      if (index < 3) moveSmooth(index, val, 1);
      else servos[index].write(val);
      angles[index] = val;

      if (isRecording && moveCount < MAX_MOVES) {
        unsigned long now = millis();
        uint16_t delta = (now - lastMoveTime > 65535) ? 65535 : (now - lastMoveTime);
        movements[moveCount++] = { (uint8_t)index, (uint8_t)val, delta };
        lastMoveTime = now;
      }

      server.send(200, "text/plain", "OK");
      return;
    }
  }
  server.send(400, "text/plain", "Invalid");
}

// --- Start Recording ---
void handleStartRecord() {
  isRecording = true;
  moveCount = 0;
  lastMoveTime = millis();
  for (int i = 0; i < 4 && moveCount < MAX_MOVES; i++) {
    movements[moveCount++] = { (uint8_t)i, (uint8_t)angles[i], 0 };
  }
  server.send(200, "text/plain", "Recording Started");
}

// --- Stop Recording ---
void handleStopRecord() {
  isRecording = false;
  for (int i = 0; i < 4 && moveCount < MAX_MOVES; i++) {
    unsigned long now = millis();
    uint16_t delta = (now - lastMoveTime > 65535) ? 65535 : (now - lastMoveTime);
    movements[moveCount++] = { (uint8_t)i, (uint8_t)angles[i], delta };
    lastMoveTime = now;
  }
  server.send(200, "text/plain", "Recording Stopped");
}

// --- Replay Movements ---
void handleReplay() {
  for (int i = 0; i < moveCount; i++) {
    uint16_t delayTime = movements[i].timestamp;
    if (delayTime > 2000) delayTime = 2000;
    delay(delayTime);

    int index = movements[i].servoIndex;
    int angle = movements[i].angle;

    if (index < 3) moveSmooth(index, angle, 1);
    else servos[index].write(angle);

    angles[index] = angle;
  }
  server.send(200, "text/plain", "Replayed");
}

// --- Detach Servos ---
void handleDetach() {
  for (int i = 0; i < 4; i++) servos[i].detach();
  server.send(200, "text/plain", "Detached");
}

// --- Reattach Servos ---
void handleAttachAndRecord() {
  servos[0].attach(13);
  servos[1].attach(12);
  servos[2].attach(14);
  servos[3].attach(27);
  delay(200);
  for (int i = 0; i < 4; i++) servos[i].write(angles[i]);
  server.send(200, "text/plain", "Reattached & Recorded");
}

// --- Show Free Heap ---
void handleFreeHeap() {
  server.send(200, "text/plain", String(ESP.getFreeHeap()));
}

// --- Web Interface ---
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Servo Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; background: #f2f2f2; text-align: center; padding: 20px; }
    .container { max-width: 420px; margin: auto; background: #fff; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }
    .rotator-control, .slider-wrapper, .button-group { margin: 20px 0; }
    .rotator-control { display: flex; justify-content: center; align-items: center; gap: 15px; }
    .slider-wrapper { display: flex; justify-content: center; gap: 30px; }
    .slider-group { display: flex; flex-direction: column; align-items: center; }
    .vertical-slider { writing-mode: bt-lr; -webkit-appearance: slider-vertical; width: 25px; height: 150px; }
    .angle-display, .heap-info { font-size: 18px; font-weight: bold; }
    button {
      padding: 10px 20px; font-size: 16px; margin: 5px; border: none;
      border-radius: 8px; background-color: #007BFF; color: white;
      cursor: pointer; box-shadow: 0 5px 15px rgba(0, 123, 255, 0.4), 0 0 8px rgba(0, 123, 255, 0.7);
    }
    button:active { transform: translateY(2px); box-shadow: 0 2px 10px rgba(0, 123, 255, 0.6); }
    button:hover { background-color: #0056b3; }
  </style>
</head>
<body>
  <div class="container">
    <h2>ESP32 Servo Controller</h2>

    <div class='rotator-control'>
      <button onmousedown="startRotate('right')" onmouseup="stopRotate()" ontouchstart="startRotate('right')" ontouchend="stopRotate()">left</button>
      <div class="angle-display" id="rotator-angle">90°</div>
      <button onmousedown="startRotate('left')" onmouseup="stopRotate()" ontouchstart="startRotate('left')" ontouchend="stopRotate()">right</button>
    </div>

    <div class="slider-wrapper">
)rawliteral";

  for (int i = 1; i < 3; i++) {
    html += "<div class='slider-group'>";
    html += (i == 1) ? "<span>Up/Down</span>" : "<span>Fwd/Back</span>";
    html += "<input class='vertical-slider' type='range' min='" + String(minAngles[i]) + "' max='" + String(maxAngles[i]) +
            "' value='" + String(angles[i]) + "' onchange='updateServo(this.value," + String(i) + ")'>";
    html += "<span id='val" + String(i) + "'>" + String(angles[i]) + "°</span></div>";
  }

  html += R"rawliteral(
    </div>

    <div class='button-group'>
      <h3>Servo 4 (Picker)</h3>
      <button onclick="setPicker('open')">Open</button>
      <button onclick="setPicker('close')">Close</button>
    </div>

    <div class='button-group'>
      <h3>Record Movement</h3>
      <button onclick="startManualRecord()">Start Record</button>
      <button onclick="stopManualRecord()">Stop Record</button>
    </div>

    <div class='button-group'>
      <h3>Playback</h3>
      <button onclick="replayMovement()">Replay</button>
    </div>

    <div class='heap-info'>
      <h3>Free Heap:</h3>
      <div id="heapDisplay">Loading...</div>
    </div>

    <script>
      let baseAngle = 90;
      let rotateInterval = null;

      function updateServo(val, index) {
        document.getElementById('val' + index).innerText = val + "°";
        fetch("/set?servo=" + index + "&angle=" + val);
      }

      function startRotate(direction) {
        if (rotateInterval) return;
        rotateInterval = setInterval(() => {
          let delta = (direction === 'left') ? -1 : 1;
          baseAngle = Math.min(180, Math.max(0, baseAngle + delta));
          document.getElementById('rotator-angle').innerText = baseAngle + "°";
          fetch("/set?servo=0&angle=" + baseAngle);
        }, 30);
      }

      function stopRotate() {
        clearInterval(rotateInterval);
        rotateInterval = null;
      }

      function setPicker(state) {
        let angle = (state === 'open') ? 50 : 0;
        fetch("/set?servo=3&angle=" + angle);
      }

      function startManualRecord() {
        fetch("/start_record").then(() => alert("⏺️ Recording started"));
      }

      function stopManualRecord() {
        fetch("/stop_record").then(() => alert("⏹️ Recording stopped"));
      }

      function replayMovement() {
        fetch("/replay");
      }

      function updateHeap() {
        fetch("/freeheap")
          .then(res => res.text())
          .then(data => document.getElementById("heapDisplay").innerText = data + " bytes");
      }

      setInterval(updateHeap, 2000); // update every 2s
      updateHeap();
    </script>
  </div>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

// --- Setup Function ---
void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP Started");
  Serial.println("Visit: http://192.168.4.1");

  servos[0].attach(13);
  servos[1].attach(12);
  servos[2].attach(14);
  servos[3].attach(27);

  for (int i = 0; i < 4; i++) servos[i].write(angles[i]);

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.on("/detach", handleDetach);
  server.on("/attach_record", handleAttachAndRecord);
  server.on("/start_record", handleStartRecord);
  server.on("/stop_record", handleStopRecord);
  server.on("/replay", handleReplay);
  server.on("/freeheap", handleFreeHeap);

  server.begin();
  Serial.println("Web server started");

  // Optional: Monitor memory in Serial
  Serial.print("Initial Free Heap: ");
  Serial.println(ESP.getFreeHeap());
}

void loop() {
  server.handleClient();
}

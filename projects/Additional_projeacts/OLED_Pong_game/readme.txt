Components Needed:
Piezo buzzer
ESP32
0.91" OLED display (I2C)
Two buttons (or a joystick) for paddle control
Wiring:
Connect the OLED display to the ESP32:

VCC to 3.3V
GND to GND
SCL to GPIO 22
SDA to GPIO 21
Connect the buttons to the ESP32:

Button 1 to GPIO 32 (for moving the paddle up)
Button 2 to GPIO 33 (for moving the paddle down)
Connect the piezo buzzer to the ESP32:

Positive (long leg) to GPIO 25
Negative (short leg) to GND

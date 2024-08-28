Components
ESP32 38-pin Microcontroller
Flame Sensor
LED 
Buzzer (Active Buzzer)



Flame Sensor:

VCC: Connect to 3.3V or (5V depending on the sensor's specifications; ESP32's 3.3V pin should be sufficient for most sensors).

GND: Connect to GND on the ESP32.
Analog Output: Connect to GPIO 34 on the ESP32 (the pin defined by FLAME_SENSOR_PIN in your code).



LED:

Anode (long leg): Connect to GPIO 2 on the ESP32 (the pin defined by LED_PIN in your code).
Cathode (short leg): Connect to GND. If using a separate LED, use a current-limiting resistor (e.g., 220Ω) in series with the LED.


Buzzer:

Positive Lead: Connect to GPIO 19 on the ESP32 (the pin defined by BUZZER_PIN in your code).
Negative Lead: Connect to GND.
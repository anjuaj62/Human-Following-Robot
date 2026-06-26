# Human Following Robot 🤖

A robot that detects and follows a person using an ultrasonic sensor
and IR sensors, built with Arduino. Features an OLED display that shows
a ❤️ when a person is detected, and a ? when searching.

## Components Used
- Arduino Uno
- Ultrasonic Sensor (HC-SR04)
- IR Sensors (x2)
- DC Motors (x2)
- Motor Driver (L298N)
- OLED Display (SSD1306 128x64)

## How It Works
- The ultrasonic sensor measures distance continuously
- If a person is detected **between 8cm and 20cm** → robot follows them
- IR sensors on left and right help the robot steer correctly
- If no one is in range → robot stops and displays **?**
- When following → OLED displays a **heart ❤️**

## Pin Configuration

**Ultrasonic Sensor**
- Pin 8 → Trigger
- Pin 9 → Echo

**IR Sensors**
- Pin 2 → Left IR Sensor
- Pin 3 → Right IR Sensor

**Motors**
- Pin 5  → Motor 1 (Forward)
- Pin 6  → Motor 1 (Backward)
- Pin 10 → Motor 2 (Forward)
- Pin 11 → Motor 2 (Backward)

## Libraries Required
- Adafruit GFX Library
- Adafruit SSD1306

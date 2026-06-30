# Human Following Robot 🤖

A robot that autonomously detects and follows a person using an ultrasonic distance sensor and two IR sensors for steering — built with Arduino Uno. Features an OLED display that shows ❤️ when following and ? when searching.

> This project combines multiple sensor inputs, real-time decision logic, and peripheral display output — my most complex embedded build to date.

---

## Demo

<img width="480" height="854" alt="human-follower-demo" src="https://github.com/user-attachments/assets/7016c876-9b30-44b9-8cbe-8d8a8854964f" />


---

## How It Works

The robot runs a continuous sensing loop and makes steering decisions based on two inputs: **distance** (ultrasonic) and **direction** (IR sensors).

```
┌─────────────────────────────────────────────────────┐
│                   SENSING LOOP                      │
│                                                     │
│  Ultrasonic ──► Distance < 8cm?  → Stop (too close) │
│                 8–20cm?          → Follow mode       │
│                 > 20cm?          → Search mode       │
│                                                     │
│  IR Left + IR Right ──► Steer correction            │
│    Left triggered  → turn left                      │
│    Right triggered → turn right                     │
│    Both clear      → move forward                   │
│                                                     │
│  OLED ──► ❤️ (following)  /  ? (searching)          │
└─────────────────────────────────────────────────────┘
```

**Why two sensor types?** Ultrasonic measures distance accurately but can't determine lateral position. The two IR sensors add left/right awareness so the robot can correct its heading — combining both gives robust person-tracking without a camera.

---

## Hardware

| Component | Quantity | Purpose |
|---|---|---|
| Arduino Uno | 1 | Microcontroller |
| HC-SR04 Ultrasonic Sensor | 1 | Distance measurement |
| IR Sensor Module | 2 | Left/right steering |
| DC Motor | 2 | Locomotion |
| L298N Motor Driver | 1 | Motor current control |
| SSD1306 OLED Display (128×64) | 1 | Status display over I2C |
| Battery pack | 1 | Power supply |

---

## Pin Configuration

```
Arduino Uno
├── Pin 8    → HC-SR04 Trigger (Output)
├── Pin 9    → HC-SR04 Echo   (Input)
├── Pin 2    → Left IR Sensor  (Digital Input)
├── Pin 3    → Right IR Sensor (Digital Input)
├── Pin 5    → Motor 1 — Forward  (PWM)
├── Pin 6    → Motor 1 — Backward (PWM)
├── Pin 10   → Motor 2 — Forward  (PWM)
├── Pin 11   → Motor 2 — Backward (PWM)
└── I2C (A4/A5) → SSD1306 OLED Display
```

---

Key decision logic:

```cpp
long distance = getUltrasonicDistance();

if (distance >= 8 && distance <= 20) {
    // Person in range — follow with IR steering correction
    showHeart();
    if (leftIR == LOW)       turnLeft();
    else if (rightIR == LOW) turnRight();
    else                     moveForward();
} else {
    // Out of range — stop and search
    showQuestion();
    stopMotors();
}
```

OLED display is driven over **I2C** using the Adafruit SSD1306 library, leaving more GPIO pins free for sensors.

---

## Libraries Required

| Library | Purpose | Install via |
|---|---|---|
| `Adafruit GFX Library` | Graphics primitives for OLED | Arduino Library Manager |
| `Adafruit SSD1306` | SSD1306 OLED I2C driver | Arduino Library Manager |

---

## What I Learned

- **Sensor fusion**: combining ultrasonic (distance) + IR (direction) for decisions no single sensor could make alone
- **I2C communication**: driving an OLED display using SDA/SCL with the Adafruit driver stack
- **PWM motor control**: independent speed control of two motors via L298N
- **Real-time decision loops**: structuring code so sensor reads and actuator updates complete within a consistent cycle time
- **Edge case handling**: what happens when the person is too close, too far, or at an angle

---
## Tools Used

`Arduino IDE` · `Proteus (simulation)` · `C++` · `I2C / SSD1306`

---

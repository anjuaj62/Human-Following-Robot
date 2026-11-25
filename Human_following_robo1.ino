// Human Following Robot - Arduino Uno
// Ultrasonic distance + two IR sensors for steering,
// SSD1306 OLED (I2C) shows a heart while following and a ? while searching.
//
// Behavior:
//   8-20 cm, left IR triggered only    -> turn left,  OLED heart
//   8-20 cm, right IR triggered only   -> turn right, OLED heart
//   8-20 cm, both or neither triggered -> forward,    OLED heart
//   under 8 cm, over 20 cm, or no echo -> stop,       OLED ?
// IR modules read LOW (0) when triggered.
//
// Libraries (Library Manager): Adafruit GFX Library, Adafruit SSD1306

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define TRIG 8
#define ECHO 9

#define IRL 2
#define IRR 3

#define M1A 5
#define M1B 6
#define M2A 10
#define M2B 11

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C          // change to 0x3D if your display does not light up

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int SPEED = 100;                          // PWM value, 0-255
const long MIN_DIST_CM = 8;
const long MAX_DIST_CM = 20;
const unsigned long ECHO_TIMEOUT_US = 30000UL;  // stops pulseIn from blocking for 1 s

bool oledOk = false;
int shownIcon = -1;             // -1 = nothing yet, 0 = question mark, 1 = heart

void showHeart() {
  if (!oledOk || shownIcon == 1) return;
  display.clearDisplay();
  display.fillCircle(52, 26, 13, SSD1306_WHITE);
  display.fillCircle(76, 26, 13, SSD1306_WHITE);
  display.fillTriangle(40, 32, 88, 32, 64, 58, SSD1306_WHITE);
  display.display();
  shownIcon = 1;
}

void showQuestion() {
  if (!oledOk || shownIcon == 0) return;
  display.clearDisplay();
  display.setTextSize(6);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(46, 8);
  display.print("?");
  display.display();
  shownIcon = 0;
}

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(IRL, INPUT);
  pinMode(IRR, INPUT);

  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);

  Serial.begin(9600);

  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    oledOk = true;
    showQuestion();
  } else {
    Serial.println("OLED not found - running without display");
  }
}

// Returns the distance in cm, or -1 if no echo came back in time.
long getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) {
    return -1;
  }
  return duration * 0.034 / 2;
}

void moveForward() {
  analogWrite(M1A, SPEED);
  analogWrite(M1B, 0);
  analogWrite(M2A, SPEED);
  analogWrite(M2B, 0);
}

void turnLeft() {
  analogWrite(M1A, 0);       // left motor stopped
  analogWrite(M1B, 0);
  analogWrite(M2A, SPEED);   // right motor forward
  analogWrite(M2B, 0);
}

void turnRight() {
  analogWrite(M1A, SPEED);   // left motor forward
  analogWrite(M1B, 0);
  analogWrite(M2A, 0);       // right motor stopped
  analogWrite(M2B, 0);
}

void stopMotors() {
  analogWrite(M1A, 0);
  analogWrite(M1B, 0);
  analogWrite(M2A, 0);
  analogWrite(M2B, 0);
}

void loop() {
  long distance = getDistance();
  int leftIR = digitalRead(IRL);
  int rightIR = digitalRead(IRR);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | IR Left: ");
  Serial.print(leftIR);
  Serial.print(" | IR Right: ");
  Serial.println(rightIR);

  if (distance >= MIN_DIST_CM && distance <= MAX_DIST_CM) {
    showHeart();
    if (leftIR == 0 && rightIR == 1) {
      turnLeft();
      Serial.println("Motors left");
    } else if (leftIR == 1 && rightIR == 0) {
      turnRight();
      Serial.println("Motors right");
    } else {
      moveForward();
      Serial.println("Motors forward");
    }
  } else {
    showQuestion();
    stopMotors();
    Serial.println("Motors stop - distance not in range");
  }

  delay(60);   // gives the ultrasonic echo time to die out between readings
}

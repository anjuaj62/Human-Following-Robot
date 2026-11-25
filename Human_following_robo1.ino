// Human Following Robot - Arduino Uno
// Ultrasonic distance + two IR sensors for steering.
//
// Behavior:
//   distance < 8 cm or > 20 cm, or no echo -> stop
//   8-20 cm, left IR triggered only        -> turn left
//   8-20 cm, right IR triggered only       -> turn right
//   8-20 cm, both or neither triggered     -> forward
// IR modules read LOW (0) when triggered.

#define TRIG 8
#define ECHO 9

#define IRL 2
#define IRR 3

#define M1A 5
#define M1B 6
#define M2A 10
#define M2B 11

const int SPEED = 100;                    // PWM value, 0-255
const long MIN_DIST_CM = 8;
const long MAX_DIST_CM = 20;
const unsigned long ECHO_TIMEOUT_US = 30000UL;  // about 5 m, stops pulseIn from blocking for 1 s

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
    stopMotors();
    Serial.println("Motors stop - distance not in range");
  }

  delay(60);   // gives the ultrasonic echo time to die out between readings
}

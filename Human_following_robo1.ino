#define trig 8
#define echo 9

#define irl 2
#define irr 3

#define m1a 5
#define m1b 6
#define m2a 10
#define m2b 11



void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  pinMode(irl, INPUT);  // FIXED: IR sensors should be INPUT
  pinMode(irr, INPUT);

  pinMode(m1a, OUTPUT);          
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT);

  Serial.begin(9600);
}

// === Distance Calculation ===
long getDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

// === Motor Control Functions ===
void moveForward() {
  analogWrite(m1a, 100);  // Motor A forward
  analogWrite(m1b, 0);
  analogWrite(m2a, 100);  // Motor B forward
  analogWrite(m2b, 0);
  delay(1000);
}

void moveBackward() {
  analogWrite(m1a, 0);
  analogWrite(m1b, 100);
  analogWrite(m2a, 0);
  analogWrite(m2b, 100);
}

void turnLeft() {
  analogWrite(m1a, 0);     // Left motor stops or reverses
  analogWrite(m1b, 0);
  analogWrite(m2a, 100);   // Right motor forward
  analogWrite(m2b, 0);
}

void turnRight() {
  analogWrite(m1a, 100);   // Left motor forward
  analogWrite(m1b, 0);
  analogWrite(m2a, 0);     // Right motor stops or reverses
  analogWrite(m2b, 0);
}

void stopMotors() {
  analogWrite(m1a, 0);
  analogWrite(m1b, 0);
  analogWrite(m2a, 0);
  analogWrite(m2b, 0);
}

// === Main Loop ===
void loop() {
  long distance = getDistance();
  int leftIR = digitalRead(irl);
  int rightIR = digitalRead(irr);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | IR Left: ");
  Serial.print(leftIR);
  Serial.print(" | IR Right: ");
  Serial.println(rightIR);

  if (distance >= 8 && distance <= 20) {
    if ((leftIR == 0 && rightIR == 0) || (leftIR == 1 && rightIR == 1)) {
      moveForward();
      Serial.println("Motors forward");
    } else if (leftIR == 0) {
      turnLeft();
      Serial.println("Motors left");
    } else if (rightIR == 0) {
      turnRight();
      Serial.println("Motors right");
    } else {
      moveForward();  // Default forward
      Serial.println("Motors forward");
    }
  } else {
    stopMotors();
    Serial.println("Motors Stop - Distance not in range");
  }

  delay(100);
}

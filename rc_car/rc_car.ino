#include <Servo.h>
#include <SoftwareSerial.h>

// ---------------- Bluetooth ----------------
SoftwareSerial BT(10, 11);  // RX, TX

// ---------------- Sensors ----------------
#define IR_PIN A0
#define TRIG_PIN 12
#define ECHO_PIN 13

// ---------------- Servo ----------------
#define SERVO_PIN 8
Servo myServo;

// ---------------- Motor (L298N) ----------------
// ⚠️ PINS NOT CHANGED
#define ENA 7
#define IN1 6
#define IN2 5
#define IN3 4
#define IN4 3
#define ENB 2

// ---------------- Speed ----------------
int FAST_SPEED = 255;
int SLOW_SPEED = 120;
int valSpeed = FAST_SPEED;

// ---------------- Variables ----------------
long duration;
int distance;
int irValue;
bool blocked = false;

char currentCmd = 'S';

// Command timeout (hold button logic)
unsigned long lastCmdTime = 0;
const unsigned long CMD_TIMEOUT = 200;

// Sensor timing
unsigned long lastSensorTime = 0;

// ---------------- Setup ----------------
void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(IR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(90);

  stopMotors();
  Serial.println("Bluetooth Car Ready");
}

// ---------------- Loop ----------------
void loop() {

  // ---------- Obstacle check ----------
  if (millis() - lastSensorTime > 80) {
    lastSensorTime = millis();

    irValue = digitalRead(IR_PIN);
    distance = getDistance();

    if (irValue == LOW || distance < 20) {
      blocked = true;
      scanAndAvoid();   // 🔥 AUTO AVOID
    } else {
      blocked = false;
    }
  }

  // ---------- Read Bluetooth ----------
  if (BT.available()) {
    currentCmd = BT.read();
    lastCmdTime = millis();
    Serial.println(currentCmd);
  }

  // ---------- Stop if button released ----------
  if (millis() - lastCmdTime > CMD_TIMEOUT) {
    currentCmd = 'S';
  }

  if (blocked && currentCmd != 'S') {
    stopMotors();
    return;
  }

  // ---------- Execute command ----------
  switch (currentCmd) {

    case 'F': forward(); break;
    case 'B': backward(); break;
    case 'L': left(); break;
    case 'R': right(); break;

    case 'G': forwardLeft(); break;
    case 'H': forwardRight(); break;
    case 'I': backLeft(); break;
    case 'J': backRight(); break;

    case 'S': stopMotors(); break;

    case 'U': myServo.write(180); break;
    case 'D': myServo.write(0); break;

    case 'V': valSpeed = FAST_SPEED; break;
    case 'v': valSpeed = SLOW_SPEED; break;
  }
}

// ---------------- AUTO AVOID FUNCTIONS ----------------

int measureAtAngle(int angle) {
  myServo.write(angle);
  delay(250);
  return getDistance();
}

void scanAndAvoid() {
  stopMotors();

  int leftDist   = measureAtAngle(150);
  int centerDist = measureAtAngle(90);
  int rightDist  = measureAtAngle(30);

  myServo.write(90);

  if (leftDist > rightDist && leftDist > 30) {
    left();
    delay(350);
  }
  else if (rightDist > 30) {
    right();
    delay(350);
  }
  else {
    backward();
    delay(300);
  }

  stopMotors();
}

// ---------------- Core Functions ----------------

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 25000);
  if (duration == 0) return 300;
  return duration * 0.034 / 2;
}

void applySpeed(int leftSpeed, int rightSpeed) {
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  applySpeed(valSpeed, valSpeed);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  applySpeed(valSpeed, valSpeed);
}

void left() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  applySpeed(valSpeed, valSpeed);
}

void right() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  applySpeed(valSpeed, valSpeed);
}

void forwardLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  applySpeed(valSpeed / 2, valSpeed);
}

void forwardRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  applySpeed(valSpeed, valSpeed / 2);
}

void backLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  applySpeed(valSpeed / 2, valSpeed);
}

void backRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  applySpeed(valSpeed, valSpeed / 2);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

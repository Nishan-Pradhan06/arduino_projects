#include <Servo.h>
#include <SoftwareSerial.h>

/* =====================================================
   BLUETOOTH (HC-05 / HC-06)
   ===================================================== */
SoftwareSerial BT(10, 11);   // RX, TX

/* =====================================================
   ULTRASONIC SENSOR (HC-SR04)
   ===================================================== */
#define TRIG_PIN 12
#define ECHO_PIN 13

/* =====================================================
   SERVO MOTOR
   ===================================================== */
#define SERVO_PIN 8
Servo myServo;

/* =====================================================
   MOTOR DRIVER (L298N)
   ===================================================== */
#define ENA 7
#define IN1 6
#define IN2 5
#define IN3 4
#define IN4 3
#define ENB 2

/* =====================================================
   SPEED SETTINGS
   ===================================================== */
int FAST_SPEED = 255;
int SLOW_SPEED = 120;
int valSpeed   = FAST_SPEED;

/* =====================================================
   VARIABLES
   ===================================================== */
long duration;
int distance;
bool blocked = false;

char currentCmd = 'S';

// Bluetooth button hold logic
unsigned long lastCmdTime = 0;
const unsigned long CMD_TIMEOUT = 200;

// Ultrasonic timing
unsigned long lastSensorTime = 0;

/* =====================================================
   SETUP
   ===================================================== */
void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  Serial.println("==== ROBOT INITIALIZING ====");

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

  Serial.println("Motors OK");
  Serial.println("Servo OK");
  Serial.println("Ultrasonic OK");

  startupScan();   // 🔥 Scan surroundings at power ON

  Serial.println("Bluetooth Car Ready (Ultrasonic Only)");
  Serial.println("=================================");
}

/* =====================================================
   LOOP
   ===================================================== */
void loop() {

  /* -------- Ultrasonic Obstacle Check -------- */
  if (millis() - lastSensorTime > 80) {
    lastSensorTime = millis();

    distance = getDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 20) {
      blocked = true;
      Serial.println("⚠ Obstacle Detected!");
      scanAndAvoid();
    } else {
      blocked = false;
    }
  }

  /* -------- Read Bluetooth Commands -------- */
  if (BT.available()) {
    currentCmd = BT.read();
    lastCmdTime = millis();

    Serial.print("Bluetooth Command: ");
    Serial.println(currentCmd);
  }

  /* -------- Stop if button released -------- */
  if (millis() - lastCmdTime > CMD_TIMEOUT) {
    currentCmd = 'S';
  }

  if (blocked && currentCmd != 'S') {
    stopMotors();
    return;
  }

  /* -------- Execute Commands -------- */
  switch (currentCmd) {

    case 'F': Serial.println("FORWARD"); forward(); break;
    case 'B': Serial.println("BACKWARD"); backward(); break;
    case 'L': Serial.println("LEFT"); left(); break;
    case 'R': Serial.println("RIGHT"); right(); break;

    case 'G': Serial.println("FORWARD LEFT"); forwardLeft(); break;
    case 'H': Serial.println("FORWARD RIGHT"); forwardRight(); break;
    case 'I': Serial.println("BACK LEFT"); backLeft(); break;
    case 'J': Serial.println("BACK RIGHT"); backRight(); break;

    case 'S': Serial.println("STOP"); stopMotors(); break;

    case 'U': Serial.println("SERVO 180"); myServo.write(180); break;
    case 'D': Serial.println("SERVO 0"); myServo.write(0); break;

    case 'V': valSpeed = FAST_SPEED; Serial.println("FAST SPEED"); break;
    case 'v': valSpeed = SLOW_SPEED; Serial.println("SLOW SPEED"); break;
  }
}

/* =====================================================
   STARTUP SERVO SCAN
   ===================================================== */
void startupScan() {
  Serial.println("Startup Scan Begin");

  for (int angle = 0; angle <= 180; angle += 15) {
    myServo.write(angle);
    delay(80);

    int d = getDistance();
    Serial.print("Angle ");
    Serial.print(angle);
    Serial.print(" -> ");
    Serial.print(d);
    Serial.println(" cm");
  }

  myServo.write(90);
  Serial.println("Startup Scan Complete");
}

/* =====================================================
   AUTO AVOID LOGIC
   ===================================================== */
int measureAtAngle(int angle) {
  myServo.write(angle);
  delay(250);
  return getDistance();
}

void scanAndAvoid() {
  stopMotors();

  Serial.println("Scanning for free path...");

  int leftDist   = measureAtAngle(150);
  int centerDist = measureAtAngle(90);
  int rightDist  = measureAtAngle(30);

  Serial.print("Left: "); Serial.print(leftDist);
  Serial.print("  Center: "); Serial.print(centerDist);
  Serial.print("  Right: "); Serial.println(rightDist);

  myServo.write(90);

  if (leftDist > rightDist && leftDist > 30) {
    Serial.println("Turning LEFT");
    left();
    delay(350);
  }
  else if (rightDist > 30) {
    Serial.println("Turning RIGHT");
    right();
    delay(350);
  }
  else {
    Serial.println("Moving BACK");
    backward();
    delay(300);
  }

  stopMotors();
}

/* =====================================================
   ULTRASONIC DISTANCE FUNCTION
   ===================================================== */
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

/* =====================================================
   MOTOR CONTROL FUNCTIONS
   ===================================================== */
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

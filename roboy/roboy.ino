#include <Servo.h>

#define trigPin 2
#define echoPin 3
#define servoPin 9

Servo headServo;

long duration;
int distance;

int angles[] = {40, 90, 140};   // left, center, right
int currentAngle = 90;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  headServo.attach(servoPin);
  headServo.write(currentAngle);

  Serial.begin(9600);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}

void smoothMove(int from, int to) {
  int step = (from < to) ? 1 : -1;
  for (int a = from; a != to; a += step) {
    headServo.write(a);
    delay(15);   // smooth movement
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    smoothMove(currentAngle, angles[i]);
    currentAngle = angles[i];

    delay(200);   // natural pause

    distance = getDistance();

    Serial.print("Angle: ");
    Serial.print(currentAngle);
    Serial.print("  Distance: ");
    Serial.println(distance);

    if (distance > 0 && distance <= 25) {
      // 👀 Robot focuses on object
      delay(2000);

      // Small head movement (life-like)
      headServo.write(currentAngle + 5);
      delay(300);
      headServo.write(currentAngle - 5);
      delay(300);
      headServo.write(currentAngle);
      delay(500);
    }
  }
}

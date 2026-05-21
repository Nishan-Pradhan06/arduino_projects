#include <Servo.h>

#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 13

Servo myServo;

long duration;
int distance;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(0); // lid closed
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

void loop() {
  int dist = getDistance();
  Serial.println(dist);

  if (dist > 0 && dist < 20) {  // hand detected
    Serial.println("Opening Lid");
    myServo.write(90);          // open lid
    delay(3000);                // wait 3 sec
    Serial.println("Closing Lid");
    myServo.write(0);           // close lid
  }

  delay(500);
}
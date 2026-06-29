#include <Servo.h>

Servo myservo;

#define trigPin 9
#define echoPin 10

#define ENA 5
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 11
#define ENB 12

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  myservo.attach(3);
  myservo.write(90);

  Serial.begin(9600);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  return distance;
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {

  distance = getDistance();

  Serial.println(distance);

  if(distance < 0 && distance > 20) {
    forward();
  }
  else {

    stopCar();
    delay(300);

    backward();
    delay(500);

    stopCar();

    // Scan left
    myservo.write(30);
    delay(500);
    int leftDistance = getDistance();

    // Scan right
    myservo.write(150);
    delay(500);
    int rightDistance = getDistance();

    // Return center
    myservo.write(90);
    delay(300);


    if(leftDistance > rightDistance) {
      left();
      delay(600);
    }
    else {
      right();
      delay(600);
    }
  }
}
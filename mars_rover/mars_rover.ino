#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// ---------------------------
// DHT11 Sensor
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------------------
// Motor Pins
#define IN1 26
#define IN2 27
#define ENA 25

#define IN3 14
#define IN4 12
#define ENB 33

// ---------------------------
// Ultrasonic Pins
#define TRIG 5
#define ECHO 18

long duration;
int distance;

// ---------------------------
WebServer server(80);

// ---------------------------
// Motor Functions
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  Serial.println("Motor Forward");
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  Serial.println("Motor Backward");
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  Serial.println("Motor Left");
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  Serial.println("Motor Right");
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Motor Stop");
}

// ---------------------------
// Ultrasonic Distance
int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

// ---------------------------
// HTTP Handlers
void handleForward() { 
  Serial.println("Command: Forward");
  server.send(200, "text/plain", "forward"); 
  forward(); 
}

void handleBackward() { 
  Serial.println("Command: Backward");
  server.send(200, "text/plain", "backward"); 
  backward(); 
}

void handleLeft() { 
  Serial.println("Command: Left");
  server.send(200, "text/plain", "left"); 
  left(); 
}

void handleRight() { 
  Serial.println("Command: Right");
  server.send(200, "text/plain", "right"); 
  right(); 
}

void handleStop() { 
  Serial.println("Command: Stop");
  server.send(200, "text/plain", "stop"); 
  stopMotor(); 
}

void handleTemp() {
  float t = dht.readTemperature();
  Serial.print("Temperature Requested: ");
  Serial.println(t);
  server.send(200, "text/plain", String(t));
}

// ---------------------------
void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopMotor();

  // Ultrasonic pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  dht.begin();

  // WiFi hotspot
  WiFi.softAP("MarsRover", "12345678");
  Serial.print("Hotspot IP: ");
  Serial.println(WiFi.softAPIP());

  // Server endpoints
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);
  server.on("/temp", handleTemp);

  server.begin();
}

// ---------------------------
void loop() {
  server.handleClient();

  // Check distance for obstacle
  distance = getDistance();
  if (distance < 20) {  // obstacle detected
    Serial.println("Obstacle detected! Moving backward slightly...");
    stopMotor();
    delay(200);     // short pause
    forward();     // move backward
    delay(500);     // duration of backward movement (adjust as needed)
    stopMotor();    // stop after backing up
  }
}
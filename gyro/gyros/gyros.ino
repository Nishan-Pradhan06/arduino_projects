#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_GYRO";
const char* password = "12345678";

WebServer server(80);

// Motor pins
int IN1 = 26;
int IN2 = 27;
int ENA = 25;  // Enable pin for motor

void controlMotor(float x, float y) {
  if (y > 0.5) {
    // Forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(ENA, HIGH);  // static speed

  } else if (y < -0.5) {
    // Backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(ENA, HIGH);  // static speed

  } else {
    // Stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(ENA, LOW);
  }
}

void handleUpdate() {
  float x = server.arg("x").toFloat();
  float y = server.arg("y").toFloat();

  Serial.print("X: "); Serial.println(x);
  Serial.print("Y: "); Serial.println(y);

  controlMotor(x, y);

  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  // Static speed → ENA HIGH initially if you want motor ready
  digitalWrite(ENA, LOW);

  // Create WiFi hotspot
  WiFi.softAP(ssid, password);
  Serial.println("Hotspot started!");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/update", handleUpdate);
  server.begin();
}

void loop() {
  server.handleClient();
}

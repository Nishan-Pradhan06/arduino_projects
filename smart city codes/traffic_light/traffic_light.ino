#include <WiFi.h>
#include <HTTPClient.h>

// ===========================
// WIFI CONFIG
// ===========================
const char* ssid = "Noo";
const char* password = "88888888";

// ⚠️ CHANGE THIS TO YOUR PC IP
const String serverUrl = "http://10.108.215.206:3000/api/light";

// ===========================
// TRAFFIC PINS
// ===========================

// NORTH
const int NR = 23;
const int NY = 22;
const int NG = 21;

// SOUTH
const int SR = 19;
const int SY = 18;
const int SG = 5;

// EAST
const int ER = 17;
const int EY = 16;
const int EG = 4;

// WEST
const int WR = 27;
const int WY = 26;
const int WG = 25;

// ===========================
// BUTTONS
// ===========================
const int BTN_N = 32;
const int BTN_S = 33;
const int BTN_E = 34;
const int BTN_W = 35;

// ===========================
// TIMING
// ===========================
const int GREEN_TIME = 8000;
const int YELLOW_TIME = 3000;
const int RED_TIME = 5000;
const int PRIORITY_TIME = 5000;

// ===========================
// STATE CONTROL
// ===========================
bool priorityMode = false;
String lastState = "";

// ======================================================
// SETUP
// ======================================================
void setup() {

  Serial.begin(115200);

  // WIFI CONNECT
  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  // PIN MODES
  pinMode(NR, OUTPUT); pinMode(NY, OUTPUT); pinMode(NG, OUTPUT);
  pinMode(SR, OUTPUT); pinMode(SY, OUTPUT); pinMode(SG, OUTPUT);
  pinMode(ER, OUTPUT); pinMode(EY, OUTPUT); pinMode(EG, OUTPUT);
  pinMode(WR, OUTPUT); pinMode(WY, OUTPUT); pinMode(WG, OUTPUT);

  pinMode(BTN_N, INPUT_PULLUP);
  pinMode(BTN_S, INPUT_PULLUP);
  pinMode(BTN_E, INPUT_PULLUP);
  pinMode(BTN_W, INPUT_PULLUP);

  allRed();
}

// ======================================================
// LOOP
// ======================================================
void loop() {

  checkButtons();

  if (!priorityMode) {

    northSouthGreen();
    smartDelay(GREEN_TIME);

    northSouthYellow();
    smartDelay(YELLOW_TIME);

    allRed();
    smartDelay(RED_TIME);

    eastWestGreen();
    smartDelay(GREEN_TIME);

    eastWestYellow();
    smartDelay(YELLOW_TIME);

    allRed();
    smartDelay(RED_TIME);
  }
}

// ======================================================
// BUTTON CHECK
// ======================================================
void checkButtons() {

  if (digitalRead(BTN_N) == LOW) priorityNorth();
  if (digitalRead(BTN_S) == LOW) prioritySouth();
  if (digitalRead(BTN_E) == LOW) priorityEast();
  if (digitalRead(BTN_W) == LOW) priorityWest();
}

// ======================================================
// SEND JSON TO SERVER
// ======================================================
void sendTrafficUpdate(String n, String s, String e, String w) {

  String json =
    "{"
    "\"north\":\"" + n + "\","
    "\"south\":\"" + s + "\","
    "\"east\":\"" + e + "\","
    "\"west\":\"" + w + "\""
    "}";

  if (json == lastState) return;
  lastState = json;

  Serial.println("\n🚦 Sending JSON:");
  Serial.println(json);

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    int code = http.POST(json);

    Serial.print("HTTP Response: ");
    Serial.println(code);

    http.end();
  }
}

// ======================================================
// TRAFFIC STATES
// ======================================================
void allRed() {

  digitalWrite(NR, HIGH); digitalWrite(NY, LOW); digitalWrite(NG, LOW);
  digitalWrite(SR, HIGH); digitalWrite(SY, LOW); digitalWrite(SG, LOW);
  digitalWrite(ER, HIGH); digitalWrite(EY, LOW); digitalWrite(EG, LOW);
  digitalWrite(WR, HIGH); digitalWrite(WY, LOW); digitalWrite(WG, LOW);

  sendTrafficUpdate("red", "red", "red", "red");
}

void northSouthGreen() {

  allRed();

  digitalWrite(NR, LOW); digitalWrite(NG, HIGH);
  digitalWrite(SR, LOW); digitalWrite(SG, HIGH);

  sendTrafficUpdate("green", "green", "red", "red");
}

void northSouthYellow() {

  digitalWrite(NG, LOW); digitalWrite(NY, HIGH);
  digitalWrite(SG, LOW); digitalWrite(SY, HIGH);

  sendTrafficUpdate("yellow", "yellow", "red", "red");
}

void eastWestGreen() {

  allRed();

  digitalWrite(ER, LOW); digitalWrite(EG, HIGH);
  digitalWrite(WR, LOW); digitalWrite(WG, HIGH);

  sendTrafficUpdate("red", "red", "green", "green");
}

void eastWestYellow() {

  digitalWrite(EG, LOW); digitalWrite(EY, HIGH);
  digitalWrite(WG, LOW); digitalWrite(WY, HIGH);

  sendTrafficUpdate("red", "red", "yellow", "yellow");
}

// ======================================================
// PRIORITY SYSTEM
// ======================================================
void priorityNorth() {
  priorityMode = true;
  allRed();
  delay(1000);

  digitalWrite(NG, HIGH);
  digitalWrite(NR, LOW);

  sendTrafficUpdate("green", "red", "red", "red");

  delay(PRIORITY_TIME);
  allRed();
  priorityMode = false;
}

void prioritySouth() {
  priorityMode = true;
  allRed();
  delay(1000);

  digitalWrite(SG, HIGH);
  digitalWrite(SR, LOW);

  sendTrafficUpdate("red", "green", "red", "red");

  delay(PRIORITY_TIME);
  allRed();
  priorityMode = false;
}

void priorityEast() {
  priorityMode = true;
  allRed();
  delay(1000);

  digitalWrite(EG, HIGH);
  digitalWrite(ER, LOW);

  sendTrafficUpdate("red", "red", "green", "red");

  delay(PRIORITY_TIME);
  allRed();
  priorityMode = false;
}

void priorityWest() {
  priorityMode = true;
  allRed();
  delay(1000);

  digitalWrite(WG, HIGH);
  digitalWrite(WR, LOW);

  sendTrafficUpdate("red", "red", "red", "green");

  delay(PRIORITY_TIME);
  allRed();
  priorityMode = false;
}

// ======================================================
// SMART DELAY
// ======================================================
void smartDelay(unsigned long ms) {

  unsigned long start = millis();

  while (millis() - start < ms) {

    checkButtons();

    if (priorityMode) return;

    delay(50);
  }
}
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <RF24.h>

#define CE_PIN 2
#define CSN_PIN 4

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

WebServer server(80);

// ESP32 hotspot credentials
const char* ap_ssid = "ESP32_REMOTE";
const char* ap_password = "12345678"; // min 8 chars

void handleOn() {
  Serial.println("📥 HTTP REQUEST: ON");

  const char text[] = "ON";
  bool ok = radio.write(&text, sizeof(text));

  if (ok) Serial.println("📡 NRF SENT: ON");
  else    Serial.println("❌ NRF SEND FAILED");

  server.send(200, "text/plain", "Light ON");
}

void handleOff() {
  Serial.println("📥 HTTP REQUEST: OFF");

  const char text[] = "OFF";
  bool ok = radio.write(&text, sizeof(text));

  if (ok) Serial.println("📡 NRF SENT: OFF");
  else    Serial.println("❌ NRF SEND FAILED");

  server.send(200, "text/plain", "Light OFF");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n===== ESP32 STARTING =====");

  // NRF setup
  Serial.println("🔧 Initializing NRF24L01...");
  if (!radio.begin()) {
    Serial.println("❌ NRF NOT DETECTED");
  } else {
    Serial.println("✅ NRF OK");
  }

  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("📡 NRF CONFIG DONE");

  // ESP32 ACCESS POINT MODE
  Serial.println("📶 Starting ESP32 Access Point...");
  WiFi.mode(WIFI_AP);
  bool apStatus = WiFi.softAP(ap_ssid, ap_password);

  if (apStatus) {
    Serial.println("✅ AP STARTED SUCCESSFULLY");
    Serial.print("📡 SSID: ");
    Serial.println(ap_ssid);
    Serial.print("🌐 AP IP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("❌ AP FAILED TO START");
  }

  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();

  Serial.println("🌍 HTTP SERVER STARTED");
  Serial.println("➡️ Open browser: http://192.168.4.1/on");
  Serial.println("➡️ Open browser: http://192.168.4.1/off");
  Serial.println("==========================\n");
}

void loop() {
  server.handleClient();
}

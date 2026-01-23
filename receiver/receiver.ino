

#include <SPI.h>
#include <RF24.h>

#define CE_PIN 2
#define CSN_PIN 4
#define LIGHT_PIN 26   // Relay / LED

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("===== RECEIVER START =====");

  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, LOW);

  Serial.println("🔧 Initializing NRF...");
  if (!radio.begin()) {
    Serial.println("❌ NRF NOT DETECTED");
    while (1);
  }

  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, address);
  radio.startListening();

  Serial.println("✅ NRF READY & LISTENING");
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    Serial.print("📩 RECEIVED: ");
    Serial.println(text);

    if (strcmp(text, "ON") == 0) {
      digitalWrite(LIGHT_PIN, HIGH);
      Serial.println("💡 LIGHT ON");
    }
    else if (strcmp(text, "OFF") == 0) {
      digitalWrite(LIGHT_PIN, LOW);
      Serial.println("💡 LIGHT OFF");
    }
  }
}





// #include <SPI.h>
// #include <RF24.h>

// #define CE_PIN 2
// #define CSN_PIN 4
// #define LIGHT_PIN 26   // Relay / LED

// RF24 radio(CE_PIN, CSN_PIN);
// const byte address[6] = "00001";

// unsigned long lastPacketTime = 0;
// const unsigned long FAILSAFE_TIME = 500; // ms

// void setup() {
//   Serial.begin(115200);
//   delay(1000);

//   Serial.println("===== RECEIVER START =====");

//   pinMode(LIGHT_PIN, OUTPUT);
//   digitalWrite(LIGHT_PIN, LOW);

//   Serial.println("🔧 Initializing NRF...");
//   if (!radio.begin()) {
//     Serial.println("❌ NRF NOT DETECTED");
//     while (1);
//   }

//   radio.setPALevel(RF24_PA_MAX);
//   radio.setDataRate(RF24_250KBPS);
//   radio.setAutoAck(true);
//   radio.openReadingPipe(0, address);
//   radio.startListening();

//   Serial.println("✅ NRF READY & LISTENING");
// }

// void loop() {
//   if (radio.available()) {
//     char text[32] = "";
//     radio.read(&text, sizeof(text));

//     lastPacketTime = millis(); // heartbeat received

//     Serial.print("📩 RECEIVED: ");
//     Serial.println(text);

//     if (strcmp(text, "ON") == 0) {
//       digitalWrite(LIGHT_PIN, HIGH);
//       Serial.println("💡 LIGHT ON");
//     }
//     else if (strcmp(text, "OFF") == 0) {
//       digitalWrite(LIGHT_PIN, LOW);
//       Serial.println("💡 LIGHT OFF");
//     }
//   }

//   // 🚨 FAILSAFE (signal lost)
//   if (millis() - lastPacketTime > FAILSAFE_TIME) {
//     digitalWrite(LIGHT_PIN, LOW); // safe state
//     Serial.println("⚠️ SIGNAL LOST → FAILSAFE");
//     lastPacketTime = millis(); // prevent spam
//   }
// }

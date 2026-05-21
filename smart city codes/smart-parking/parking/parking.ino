// #include <Servo.h>

// // ---------- IR SENSORS ----------
// #define IR1 2
// #define IR2 3
// #define IR3 4
// #define IR4 5

// // ---------- SLOT 1 LED ----------
// #define R1 6
// #define G1 7

// // ---------- SLOT 2 LED ----------
// #define R2 8
// #define G2 9

// // ---------- SLOT 3 LED ----------
// #define R3 10
// #define G3 11

// // ---------- SLOT 4 LED ----------
// #define R4 12
// #define G4 13

// // ---------- OTHER ----------
// #define BUZZER A0
// #define TRIG A1
// #define ECHO A2
// #define SERVO_PIN A3

// Servo gateServo;

// int distanceThreshold = 15;
// bool gateOpened = false;

// // ---------- ULTRASONIC ----------
// long getDistance() {
//   digitalWrite(TRIG, LOW);
//   delayMicroseconds(2);

//   digitalWrite(TRIG, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG, LOW);

//   long duration = pulseIn(ECHO, HIGH, 25000);

//   if (duration == 0) return -1;

//   return duration * 0.034 / 2;
// }

// // ---------- SLOT LED UPDATE ----------
// void updateSlot(int irPin, int redPin, int greenPin) {

//   if (digitalRead(irPin) == LOW) {
//     // CAR PRESENT
//     digitalWrite(redPin, HIGH);
//     digitalWrite(greenPin, LOW);
//   } else {
//     // EMPTY SLOT
//     digitalWrite(redPin, LOW);
//     digitalWrite(greenPin, HIGH);
//   }
// }

// // ---------- AVAILABLE SLOTS ----------
// int getAvailableSlots() {
//   int count = 0;

//   if (digitalRead(IR1) == HIGH) count++;
//   if (digitalRead(IR2) == HIGH) count++;
//   if (digitalRead(IR3) == HIGH) count++;
//   if (digitalRead(IR4) == HIGH) count++;

//   return count;
// }

// void setup() {
//   Serial.begin(9600);

//   pinMode(IR1, INPUT);
//   pinMode(IR2, INPUT);
//   pinMode(IR3, INPUT);
//   pinMode(IR4, INPUT);

//   pinMode(R1, OUTPUT); pinMode(G1, OUTPUT);
//   pinMode(R2, OUTPUT); pinMode(G2, OUTPUT);
//   pinMode(R3, OUTPUT); pinMode(G3, OUTPUT);
//   pinMode(R4, OUTPUT); pinMode(G4, OUTPUT);

//   pinMode(BUZZER, OUTPUT);
//   pinMode(TRIG, OUTPUT);
//   pinMode(ECHO, INPUT);

//   gateServo.attach(SERVO_PIN);
//   gateServo.write(0);

//   Serial.println("Smart Parking System Ready");
// }

// void loop() {

//   long distance = getDistance();
//   int availableSlots = getAvailableSlots();

//   Serial.print("Distance: ");
//   Serial.print(distance);
//   Serial.print(" | Slots Free: ");
//   Serial.println(availableSlots);

//   // ---------- UPDATE ALL SLOT LEDS ----------
//   updateSlot(IR1, R1, G1);
//   updateSlot(IR2, R2, G2);
//   updateSlot(IR3, R3, G3);
//   updateSlot(IR4, R4, G4);

//   // ---------- GATE LOGIC ----------
//   if (distance > 0 && distance <= distanceThreshold) {

//     if (!gateOpened) {

//       if (availableSlots > 0) {

//         Serial.println("Gate OPEN");

//         gateServo.write(90);
//         delay(3000);
//         gateServo.write(0);

//         gateOpened = true;
//       }
//       else {

//         Serial.println("PARKING FULL");

//         digitalWrite(BUZZER, HIGH);
//         delay(1500);
//         digitalWrite(BUZZER, LOW);

//         gateOpened = true;
//       }
//     }
//   }

//   if (distance == -1 || distance > 25) {
//     gateOpened = false;
//   }

//   delay(200);
// }

#include <Servo.h>

// ─────────────────────────────
// IR SENSORS
// ─────────────────────────────
#define IR1 2
#define IR2 3
#define IR3 4
#define IR4 5

// ─────────────────────────────
// SLOT LEDs
// ─────────────────────────────
#define R1 6
#define G1 7

#define R2 8
#define G2 9

#define R3 10
#define G3 11

#define R4 12
#define G4 13

// ─────────────────────────────
// OTHER COMPONENTS
// ─────────────────────────────
#define BUZZER A0
#define TRIG A1
#define ECHO A2
#define SERVO_PIN A3

Servo gateServo;

int distanceThreshold = 15;
bool gateOpened = false;

// ─────────────────────────────
// ULTRASONIC FUNCTION
// ─────────────────────────────
long getDistance() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 25000);

  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}

// ─────────────────────────────
// UPDATE SLOT LEDS
// ─────────────────────────────
void updateSlot(int irPin, int redPin, int greenPin) {

  if (digitalRead(irPin) == LOW) {

    // OCCUPIED
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);

  } else {

    // FREE
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
  }
}

// ─────────────────────────────
// AVAILABLE SLOTS
// ─────────────────────────────
int getAvailableSlots() {

  int count = 0;

  if (digitalRead(IR1) == HIGH) count++;
  if (digitalRead(IR2) == HIGH) count++;
  if (digitalRead(IR3) == HIGH) count++;
  if (digitalRead(IR4) == HIGH) count++;

  return count;
}

void setup() {

  Serial.begin(9600);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);

  pinMode(R1, OUTPUT);
  pinMode(G1, OUTPUT);

  pinMode(R2, OUTPUT);
  pinMode(G2, OUTPUT);

  pinMode(R3, OUTPUT);
  pinMode(G3, OUTPUT);

  pinMode(R4, OUTPUT);
  pinMode(G4, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  gateServo.attach(SERVO_PIN);

  gateServo.write(0);

  Serial.println("UNO READY");
}

void loop() {

  long distance = getDistance();

  int availableSlots = getAvailableSlots();

  // ─────────────────────────────
  // UPDATE SLOT LEDs
  // ─────────────────────────────
  updateSlot(IR1, R1, G1);
  updateSlot(IR2, R2, G2);
  updateSlot(IR3, R3, G3);
  updateSlot(IR4, R4, G4);

  // ─────────────────────────────
  // GATE CONTROL
  // ─────────────────────────────
  if (distance > 0 && distance <= distanceThreshold) {

    if (!gateOpened) {

      if (availableSlots > 0) {

        gateServo.write(90);

        delay(3000);

        gateServo.write(0);

      } else {

        digitalWrite(BUZZER, HIGH);

        delay(1500);

        digitalWrite(BUZZER, LOW);
      }

      gateOpened = true;
    }
  }

  if (distance == -1 || distance > 25) {
    gateOpened = false;
  }

  // ─────────────────────────────
  // SEND DATA TO ESP32
  // ─────────────────────────────
  Serial.print("S1:");
  Serial.print(digitalRead(IR1) == LOW ? 1 : 0);

  Serial.print(",S2:");
  Serial.print(digitalRead(IR2) == LOW ? 1 : 0);

  Serial.print(",S3:");
  Serial.print(digitalRead(IR3) == LOW ? 1 : 0);

  Serial.print(",S4:");
  Serial.print(digitalRead(IR4) == LOW ? 1 : 0);

  Serial.print(",FREE:");
  Serial.println(availableSlots);

  delay(500);
}
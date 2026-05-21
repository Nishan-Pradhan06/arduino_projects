#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set LCD address (usually 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#define IR1 8
#define IR2 9
#define LED 10

float distance = 0.1; // meters (10 cm)

unsigned long startTime = 0;
unsigned long endTime = 0;
float timeTaken;
float speed;

bool started = false;

void setup() {
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(LED, OUTPUT);

  lcd.init();        // initialize LCD
  lcd.backlight();   // turn on backlight

  lcd.setCursor(0, 0);
  lcd.print("Speed Meter");
  delay(2000);
  lcd.clear();
}

void loop() {

  if (digitalRead(IR1) == LOW && !started) {
    startTime = millis();
    started = true;
    digitalWrite(LED, HIGH);
  }

  if (digitalRead(IR2) == LOW && started) {
    endTime = millis();
    timeTaken = (endTime - startTime) / 1000.0;

    if (timeTaken > 0) {  
      speed = distance / timeTaken;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed:");

    lcd.setCursor(0, 1);
    lcd.print(speed);
    lcd.print(" m/s");

    digitalWrite(LED, LOW);

    delay(2000);
    started = false;
  }
}
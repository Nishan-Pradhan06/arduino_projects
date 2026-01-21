#define enA 10
#define in1 9
#define in2 8
#define in3 7
#define in4 6
#define enB 5

#define ir_R A0
#define ir_F A1
#define ir_L A2
#define servo A4
#define pump A5   // Relay IN pin

int Speed = 160;
int s1, s2, s3;

// 🔒 Fire lock
bool fireActive = false;

void setup() {
  Serial.begin(9600);

  pinMode(ir_R, INPUT);
  pinMode(ir_F, INPUT);
  pinMode(ir_L, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(servo, OUTPUT);
  pinMode(pump, OUTPUT);

  digitalWrite(pump, HIGH); // Pump OFF

  analogWrite(enA, Speed);
  analogWrite(enB, Speed);

  delay(500);
}

void loop() {

  s1 = analogRead(ir_R);
  s2 = analogRead(ir_F);
  s3 = analogRead(ir_L);

  Serial.print(s1); Serial.print("\t");
  Serial.print(s2); Serial.print("\t");
  Serial.println(s3);

  delay(50);

  // 🔥 RIGHT FIRE
  if (s1 < 300) {
    fireActive = true;
    forword();
    delay(150);
    Stop();
    digitalWrite(pump, LOW);

    for (int a = 90; a >= 40; a -= 3) servoPulse(servo, a);
    for (int a = 40; a <= 90; a += 3) servoPulse(servo, a);
  }

  // 🔥 FRONT FIRE
  else if (s2 < 400) {
    fireActive = true;
    forword();
    delay(200);
    Stop();
    digitalWrite(pump, LOW);

    for (int a = 90; a <= 140; a += 3) servoPulse(servo, a);
    for (int a = 140; a >= 40; a -= 3) servoPulse(servo, a);
    for (int a = 40; a <= 90; a += 3) servoPulse(servo, a);
  }

  // 🔥 LEFT FIRE
  else if (s3 < 300) {
    fireActive = true;
    forword();
    delay(150);
    Stop();
    digitalWrite(pump, LOW);

    for (int a = 90; a <= 140; a += 3) servoPulse(servo, a);
    for (int a = 140; a >= 90; a -= 3) servoPulse(servo, a);
  }

  // 🚗 MOVE TOWARD FIRE (NO BACKWARD)
  else if (!fireActive && (s1 < 700 || s2 < 800 || s3 < 700)) {
    digitalWrite(pump, HIGH);
    forword();
  }

  // 🛑 STOP
  else {
    digitalWrite(pump, HIGH);
    Stop();
  }

  // 🔓 RESET FIRE LOCK
  if (s1 > 700 && s2 > 800 && s3 > 700) {
    fireActive = false;
  }

  delay(10);
}

// ---------------- SERVO ----------------
void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 500;
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(40);
}

// ---------------- MOTOR ----------------
void forword() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

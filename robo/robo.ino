#include <Servo.h>

// -------- Servos --------
Servo head;
Servo leftHand;
Servo rightHand;

// -------- Positions --------
int headPos  = 90;
int leftPos  = 90;
int rightPos = 90;

// -------- Angle Limits --------
const int CENTER = 90;
const int LEFT   = 60;
const int RIGHT  = 120;

// -------- Setup --------
void setup() {
  head.attach(8);
  leftHand.attach(9);
  rightHand.attach(10);

  head.write(CENTER);
  leftHand.write(CENTER);
  rightHand.write(CENTER);

  randomSeed(analogRead(A0));
}

// -------- SMOOTH MOVE --------
void smoothMove(Servo &servo, int &current, int target) {
  int step = (current < target) ? 1 : -1;

  for (int pos = current; pos != target; pos += step) {
    int d = map(abs(target - pos), 0, abs(target - current), 25, 8);
    servo.write(pos);
    delay(d);
  }
  servo.write(target);
  current = target;
}

// -------- RANDOM SERVO ACTION --------
void randomMove(Servo &servo, int &pos, int a, int b, int c, int minDelay, int maxDelay) {
  int action = random(3);

  if (action == 0)      smoothMove(servo, pos, a);
  else if (action == 1) smoothMove(servo, pos, b);
  else                  smoothMove(servo, pos, c);

  delay(random(minDelay, maxDelay));
}

// -------- MAIN LOOP --------
void loop() {

  // Head movement
  randomMove(head, headPos, LEFT, RIGHT, CENTER, 800, 2000);

  // Left hand (sometimes)
  if (random(2))
    randomMove(leftHand, leftPos, RIGHT, LEFT, CENTER, 600, 1800);

  // Right hand (sometimes, mirrored)
  if (random(2))
    randomMove(rightHand, rightPos, LEFT, RIGHT, CENTER, 600, 1800);
}

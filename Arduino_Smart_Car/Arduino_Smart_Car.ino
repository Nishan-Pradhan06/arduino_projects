#include <WiFi.h>
#include <WebServer.h>

// ================= MOTOR PINS =================
#define ENA 27
#define IN1 26
#define IN2 25

#define ENB 14
#define IN3 32
#define IN4 33

// ================= PWM SETTINGS =================
#define PWM_FREQ 1000
#define PWM_RESOLUTION 8

int motorSpeed = 150;
int turn = 60;

// ================= WIFI ACCESS POINT =================
const char* ssid = "SoccerCar1";
const char* password = "12345678";

WebServer server(80);

// ================= MOTOR FUNCTIONS =================

void setSpeed(int leftSpeed, int rightSpeed) {
  ledcWrite(ENA, constrain(leftSpeed, 0, 255));
  ledcWrite(ENB, constrain(rightSpeed, 0, 255));
}

void fwd() {
  setSpeed(motorSpeed, motorSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void rev() {
  setSpeed(motorSpeed, motorSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void lft() {
  setSpeed(motorSpeed - turn, motorSpeed + turn);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void rght() {
  setSpeed(motorSpeed + turn, motorSpeed - turn);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stp() {
  setSpeed(0, 0);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

// ================= HANDLE COMMAND =================

void handleCommand() {
  String command = server.uri();
  command.replace("/", "");

  Serial.println("Command: " + command);

  if (command == "F") fwd();
  else if (command == "R") rev();
  else if (command == "TL") lft();
  else if (command == "TR") rght();
  else if (command == "S") stp();
  else if (command.length() > 0) {
    motorSpeed = command.toInt();
  }

  server.send(200, "text/plain", "OK");
}

// ================= SETUP =================

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Setup PWM (ESP32 Core 3.x style)
  ledcAttach(ENA, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(ENB, PWM_FREQ, PWM_RESOLUTION);

  // Start ESP32 as WiFi Hotspot
  WiFi.softAP(ssid, password);

  Serial.println("WiFi Hotspot Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.onNotFound(handleCommand);
  server.begin();
}

// ================= LOOP =================

void loop() {
  server.handleClient();
}

// int motorLpin1=2;
// int motorLpin2=3;
// int motorRpin1=4;
// int motorRpin2=5;
// int motorLpwm=10;
// int motorRpwm=11;

// int motorSpeed=125;
// int turn=50;

// void setup() {
//   Serial.begin(9600);
//   Serial.flush();
//   pinMode(motorLpin1,OUTPUT);
//   pinMode(motorLpin2,OUTPUT);
//   pinMode(motorRpin1,OUTPUT);
//   pinMode(motorRpin2,OUTPUT);
//   pinMode(motorLpwm,OUTPUT);
//   pinMode(motorRpwm,OUTPUT);
// }

// void loop() {
//   String input="";
//   while(Serial.available()){
//     input+=(char)Serial.read();
//     delay(5);
//   }
  
//   if(input=="n"){
//     stp();
//   }
//   else if(input=="F"){
//     fwd();
//   }
//   else if(input=="R"){
//     rev();
//   }
//   else if(input.indexOf("TL")>-1){
//     lft();
//   }
//   else if(input.indexOf("TR")>-1){
//     rght();
//   }
//   else if(input!=""){
//     motorSpeed=input.toInt();
//   }
// }

// void fwd(){
//   analogWrite(motorLpwm,motorSpeed);
//   analogWrite(motorRpwm,motorSpeed);
//   digitalWrite(motorLpin1,1);
//   digitalWrite(motorLpin2,0);
//   digitalWrite(motorRpin1,1);
//   digitalWrite(motorRpin2,0);
// }

// void rev(){
//   analogWrite(motorLpwm,motorSpeed);
//   analogWrite(motorRpwm,motorSpeed);
//   digitalWrite(motorLpin1,0);
//   digitalWrite(motorLpin2,1);
//   digitalWrite(motorRpin1,0);
//   digitalWrite(motorRpin2,1);
// }

// void lft(){
//   analogWrite(motorLpwm,motorSpeed-turn);
//   analogWrite(motorRpwm,motorSpeed+turn);
//   digitalWrite(motorLpin1,0);
//   digitalWrite(motorLpin2,1);
//   digitalWrite(motorRpin1,1);
//   digitalWrite(motorRpin2,0);
// }

// void rght(){
//   analogWrite(motorLpwm,motorSpeed+turn);
//   analogWrite(motorRpwm,motorSpeed-turn);
//   digitalWrite(motorLpin1,1);
//   digitalWrite(motorLpin2,0);
//   digitalWrite(motorRpin1,0);
//   digitalWrite(motorRpin2,1);
// }

// void stp(){
//   analogWrite(motorLpwm,0);
//   analogWrite(motorRpwm,0);
//   digitalWrite(motorLpin1,1);
//   digitalWrite(motorLpin2,1);
//   digitalWrite(motorRpin1,1);
//   digitalWrite(motorRpin2,1);
// }

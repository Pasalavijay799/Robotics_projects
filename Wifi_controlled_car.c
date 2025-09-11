#define BLYNK_TEMPLATE_ID "TMPL38O9fUK5v"
#define BLYNK_TEMPLATE_NAME "wificar"
#define BLYNK_AUTH_TOKEN "0O7OeQ5qdqT8NYYP3oK4PZfShFYg1PS4"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

char auth[] = "0O7OeQ5qdqT8NYYP3oK4PZfShFYg1PS4";
char ssid[] = "vijay";
char pass[] = "12345678";

#define MOTOR1_IN1 D1  // Motor A Forward
#define MOTOR1_IN2 D2  // Motor A Backward
#define MOTOR2_IN1 D3  // Motor B Forward
#define MOTOR2_IN2 D4  // Motor B Backward
#define ENA D5         // Motor A Speed Control
#define ENB D6         // Motor B Speed Control

int motorSpeed = 150; // Default speed (Range: 0 - 1023)

void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Blynk.begin(auth, ssid, pass);

  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void setSpeed(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void moveForward() {
  Serial.println("Moving Forward");
  setSpeed(motorSpeed);
  digitalWrite(MOTOR1_IN1, HIGH);
  digitalWrite(MOTOR1_IN2, LOW);
  digitalWrite(MOTOR2_IN1, HIGH);
  digitalWrite(MOTOR2_IN2, LOW);
}

void moveBackward() {
  Serial.println("Moving Backward");
  setSpeed(motorSpeed);
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, HIGH);
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, HIGH);
}

void turnLeft() {
  Serial.println("Turning Left");
  setSpeed(motorSpeed);
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, HIGH);
  digitalWrite(MOTOR2_IN1, HIGH);
  digitalWrite(MOTOR2_IN2, LOW);
}

void turnRight() {
  Serial.println("Turning Right");
  setSpeed(motorSpeed);
  digitalWrite(MOTOR1_IN1, HIGH);
  digitalWrite(MOTOR1_IN2, LOW);
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, HIGH);
}

void stopCar() {
  Serial.println("Stopping Car");
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, LOW);
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, LOW);
  setSpeed(0);  // Stop motors completely
}

// Blynk Virtual Pins for movement
BLYNK_WRITE(V1) {
  int state = param.asInt();
  Serial.print("V1 received: "); Serial.println(state);
  if (state == 255) moveForward();
  else stopCar();
}

BLYNK_WRITE(V2) {
  int state = param.asInt();
  Serial.print("V2 received: "); Serial.println(state);
  if (state == 255) moveBackward();
  else stopCar();
}

BLYNK_WRITE(V3) {
  int state = param.asInt();
  Serial.print("V3 received: "); Serial.println(state);
  if (state == 255) turnLeft();
  else stopCar();
}

BLYNK_WRITE(V4) {
  int state = param.asInt();
  Serial.print("V4 received: "); Serial.println(state);
  if (state == 255) turnRight();
  else stopCar();
}

// Blynk Virtual Pin for Speed Control (V5)
BLYNK_WRITE(V5) {
  motorSpeed = param.asInt();
  Serial.print("Speed set to: "); Serial.println(motorSpeed);
}

void loop() {
  Blynk.run();
}

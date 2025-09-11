#include <Keypad.h>

#define motor1Pin1 8  // IN1 of L298N
#define motor1Pin2 9  // IN2 of L298N
#define motor2Pin1 6  // IN3 of L298N
#define motor2Pin2 7  // IN4 of L298N
#define ENA 10
#define ENB 11

const byte ROWS = 4;  // Four rows
const byte COLS = 3;  // Three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {A0, A1, A2, A3};  
byte colPins[COLS] = {4, 12, 13};  

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
char lastKey = '\0';  // To track last pressed key

void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void moveBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void turnLeft() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void turnRight() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void stopCar() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();

  if (key && key != lastKey) {  // Only execute when a new key is pressed
    Serial.println(key);  
    lastKey = key;  

    switch (key) {
      case '2': moveForward(); break;
      case '8': moveBackward(); break;
      case '4': turnLeft(); break;
      case '6': turnRight(); break;
      case '5': stopCar(); break;  // Stops only when '5' is pressed
    }
  }
}

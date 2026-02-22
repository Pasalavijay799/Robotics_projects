#include <Servo.h>

// Motor driver connections (L298N)
#define ENA 3  // Enable A pin for motor speed control
#define IN1 2   // Motor A direction pin
#define IN2 4   // Motor A direction pin
#define IN3 5   // Motor B direction pin
#define IN4 7   // Motor B direction pin
#define ENB 6  // Enable B pin for motor speed control
#define led 13


// Ultrasonic Sensor Pins
#define TRIG 10
#define ECHO 11

// IR Sensor Pins
#define IR_LEFT 8
//#define IR_RIGHT 9

Servo servoMotor; // Create Servo object
#define SERVO_PIN 12  // Servo motor connected to pin 12

void setup() {
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    pinMode(IR_LEFT, INPUT);
    //pinMode(IR_RIGHT, INPUT);

    pinMode(led,OUTPUT);
    servoMotor.attach(SERVO_PIN);  
    servoMotor.write(90);  // Start with servo at 90 degrees (facing forward)

    Serial.begin(9600);
}

// Function to move the car forward
void moveForward(int speed) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, speed);
    analogWrite(ENB, speed + 20);
}

// Function to move the car backward
void moveBackward(int speed) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, speed);
    analogWrite(ENB, speed + 20);
}

// Function to move the car left
void moveLeft(int speed) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, speed);
    analogWrite(ENB, speed + 20);
}

// Function to move the car right
void moveRight(int speed) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, speed);
    analogWrite(ENB, speed + 20);
}

// Function to stop the car
void stopCar() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}

// Function to measure distance using Ultrasonic Sensor
long getDistance() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    
    long duration = pulseIn(ECHO, HIGH, 30000); // 30ms timeout (~500cm)
    if (duration == 0) {
        return 999; // Return a large value to indicate no reading
    }
    return duration * 0.034 / 2;  // Convert to cm
}


// Function to scan left (-90°) and right (+90°) and return to center (90°)
char scanEnvironment() {
    servoMotor.write(0);  
    delay(1000);
    long rightDistance = getDistance();

    servoMotor.write(180);  
    delay(1000);
    long leftDistance = getDistance();

    servoMotor.write(90);  
    delay(1000);
    long forwardDistance = getDistance();  

    Serial.print("Left: ");
    Serial.print(leftDistance);
    Serial.print(" cm | Right: ");
    Serial.print(rightDistance);
    Serial.print(" cm | Forward: ");
    Serial.println(forwardDistance);

    return (leftDistance > rightDistance) ? 'L' : 'R';
}

void loop() {
  moveForward(65);
    long distance = getDistance();
    int leftIR = digitalRead(IR_LEFT);
    //int rightIR = digitalRead(IR_RIGHT);

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print("cm | Left IR: ");
    Serial.print(leftIR);
    //Serial.print(" | Right IR: ");
    //Serial.println(rightIR);

    // *Stop if IR sensor detects a black line*
    if (leftIR == 1) {  
        delay(1000);
        stopCar();
        digitalWrite(led,HIGH);
        Serial.println("Black line detected! Stopping permanently.");

        while (true) {
            // Stay in an infinite loop, keeping the car stopped
            delay(1000);
        }
      
    }
    digitalWrite(led,LOW);

    // *Obstacle Avoidance*
    if (distance < 27 ) {  
        stopCar();
        delay(500);  

        if (distance < 10) {
            moveBackward(60);
            delay(400);
            stopCar();
        }

        char direction = scanEnvironment();  
        if (direction == 'L') {
            moveRight(65);
            delay(300);
        } else if (direction == 'R') {
            moveLeft(65);
            delay(300);
        }

        moveForward(65);
    } 
        
    
}

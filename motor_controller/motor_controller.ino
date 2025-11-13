/*
  RoverForge Motor Controller
  Main sketch: motor control + ultrasonic sensing + serial telemetry
*/

#include <Arduino.h>
// motor_driver library removed per user request.
// Implement your own motor helper functions below (setMotorSpeeds, stopMotors, etc.).

const int trigPin = A0;
const int echoPin = A1;

// Left Module
const int ENA_LM = 3; // EMA -> 3~ pin on arduino
const int IN1_LM = 2;
const int IN2_LM = 4;
const int IN3_LM = 7;
const int IN4_LM = 8;
const int ENB_LM = 5;

// Right Module
const int ENA_RM = 6;
const int IN1_RM = 10;
const int IN2_RM = 11;
const int IN3_RM = 12;
const int IN4_RM = 13;
const int ENB_RM = 9;

enum MotorState {
  STOP,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

enum MotorState motorState = STOP; // Initial state


// Macanum wheel motor control functions
void leftWheelFront(MotorState state, int speed);
void rightWheelFront(MotorState state, int speed);
void leftWheelBack(MotorState state, int speed);
void rightWheelBack(MotorState state, int speed);

void stopMotors(int speed = 0);
void forward(int speed = 200);
void backward(int speed = 200);
void rotateLeft(int speed = 200);
void rotateRight(int speed = 200);
void strafeLeft(int speed = 200);
void strafeRight(int speed = 200);
void testForward();

// Reads the HC-SR04 and returns distance in cm, or -1 on timeout/no-echo.
long readUltrasonicCm(int trig, int echo, unsigned long timeout = 30000UL);
void distanceSensor();

void setup() {
  Serial.begin(9600);
  Serial.println("RoverForge Motor Controller v0.1.0");
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize motor driver
    pinMode(ENA_LM, OUTPUT);
    pinMode(IN1_LM, OUTPUT);
    pinMode(IN2_LM, OUTPUT);
    pinMode(IN3_LM, OUTPUT);
    pinMode(IN4_LM, OUTPUT);
    pinMode(ENB_LM, OUTPUT);
    pinMode(ENA_RM, OUTPUT);
    pinMode(IN1_RM, OUTPUT);
    pinMode(IN2_RM, OUTPUT);
    pinMode(IN3_RM, OUTPUT);
    pinMode(IN4_RM, OUTPUT);
    pinMode(ENB_RM, OUTPUT);
}

void loop() {
    // Check for serial commands
    if (Serial.available() > 0) {
      char cmd = Serial.read();
      
      if (cmd == 't' || cmd == 'T') {
        testForward();
      } else if (cmd == 'z' || cmd == 'Z') {
        testWheels();
      } else if (cmd == 'w' || cmd == 'W') {
        Serial.println("CMD: forward");
        forward(70);
      } else if (cmd == 's' || cmd == 'S') {
        Serial.println("CMD: stop");
        stopMotors();
      } else if (cmd == 'x' || cmd == 'X') {
        Serial.println("CMD: backward");
        backward(70);
      } else if (cmd == 'a' || cmd == 'A') {
        Serial.println("CMD: left");
        rotateLeft(70);
      } else if (cmd == 'd' || cmd == 'D') {
        Serial.println("CMD: right");
        rotateRight(70);
      } else if (cmd == 'q' || cmd == 'Q') {
        Serial.println("CMD: strafe left");
        strafeLeft(70);
      } else if (cmd == 'e' || cmd == 'E') {
        Serial.println("CMD: strafe right");
        strafeRight(70);
      }
    }

    delay(500); // wait half a second between measurements
}

// Implementation: triggers the HC-SR04, measures echo pulse width (microseconds)
// and converts to centimeters. Returns -1 if no echo was received within
// the timeout (microseconds).
long readUltrasonicCm(int trig, int echo, unsigned long timeout) {
  // Make sure trigger is low
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  // Send a 10us HIGH pulse to trigger the measurement
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Read the echo pulse width (in microseconds)
  unsigned long duration = pulseIn(echo, HIGH, timeout);
  if (duration == 0) {
    // timeout, no echo received
    return -1;
  }

  // Speed of sound ~343.0 m/s => 0.0343 cm/us. Distance is half the round-trip.
  float distanceCm = (duration * 0.0343f) / 2.0f;
  // Return rounded integer centimeters for simpler downstream logic
  return (long)(distanceCm + 0.5f);
}

void distanceSensor() {
  long distance = readUltrasonicCm(trigPin, echoPin);

  if (distance < 0) {
    Serial.println("Distance: no echo");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}

// Implement motor control functions here (setMotorSpeeds, stopMotors, etc.)

void leftWheelFront(MotorState state, int speed) {
  if (state == FORWARD) {
    // Set left front wheel to move forward at given speed
    analogWrite(ENA_LM, speed);
    digitalWrite(IN1_LM, HIGH);
    digitalWrite(IN2_LM, LOW);
  } else if (state == BACKWARD) {
    analogWrite(ENA_LM, speed);
    digitalWrite(IN1_LM, LOW);
    digitalWrite(IN2_LM, HIGH);
  } else {
    analogWrite(ENA_LM, 0);
    digitalWrite(IN1_LM, LOW);
    digitalWrite(IN2_LM, LOW);
  }
}

void rightWheelFront(MotorState state, int speed) {
  if (state == FORWARD) {
    // Set right front wheel to move forward at given speed
    analogWrite(ENB_RM, speed);
    digitalWrite(IN3_RM, HIGH);
    digitalWrite(IN4_RM, LOW);
  } else if (state == BACKWARD) {
    analogWrite(ENB_RM, speed);
    digitalWrite(IN3_RM, LOW);
    digitalWrite(IN4_RM, HIGH);
  } else {
    analogWrite(ENB_RM, 0);
    digitalWrite(IN3_RM, LOW);
    digitalWrite(IN4_RM, LOW);
  }
}

void leftWheelBack(MotorState state, int speed) {
  if (state == FORWARD) {
    // Set left back wheel to move forward at given speed
    analogWrite(ENB_LM, speed);
    digitalWrite(IN3_LM, HIGH);
    digitalWrite(IN4_LM, LOW);
  } else if (state == BACKWARD) {
    analogWrite(ENB_LM, speed);
    digitalWrite(IN3_LM, LOW);
    digitalWrite(IN4_LM, HIGH);
  } else {
    analogWrite(ENB_LM, 0);
    digitalWrite(IN3_LM, LOW);
    digitalWrite(IN4_LM, LOW);
  }
}

void rightWheelBack(MotorState state, int speed) {
  if (state == FORWARD) {
    // Set right back wheel to move forward at given speed
    analogWrite(ENA_RM, speed);
    digitalWrite(IN1_RM, HIGH);
    digitalWrite(IN2_RM, LOW);
  } else if (state == BACKWARD) {
    analogWrite(ENA_RM, speed);
    digitalWrite(IN1_RM, LOW);
    digitalWrite(IN2_RM, HIGH);
  } else {
    analogWrite(ENA_RM, 0);
    digitalWrite(IN1_RM, LOW);
    digitalWrite(IN2_RM, LOW);
  }
}

void stopMotors(int speed) {
  leftWheelFront(STOP, speed);
  rightWheelFront(STOP, speed);
  leftWheelBack(STOP, speed);
  rightWheelBack(STOP, speed);
}

void forward(int speed) {
  leftWheelFront(FORWARD, speed);
  rightWheelFront(FORWARD, speed);
  leftWheelBack(FORWARD, speed);
  rightWheelBack(FORWARD, speed);
}

void backward(int speed) {
  leftWheelFront(BACKWARD, speed);
  rightWheelFront(BACKWARD, speed);
  leftWheelBack(BACKWARD, speed);
  rightWheelBack(BACKWARD, speed);
}

void rotateLeft(int speed) {
  leftWheelFront(BACKWARD, speed);
  rightWheelFront(FORWARD, speed);
  leftWheelBack(BACKWARD, speed);
  rightWheelBack(FORWARD, speed);
}

void rotateRight(int speed) {
  leftWheelFront(FORWARD, speed);
  rightWheelFront(BACKWARD, speed);
  leftWheelBack(FORWARD, speed);
  rightWheelBack(BACKWARD, speed);
}

void strafeLeft(int speed) {
  leftWheelFront(BACKWARD, speed);
  rightWheelFront(FORWARD, speed);
  leftWheelBack(FORWARD, speed);
  rightWheelBack(BACKWARD, speed);
}

void strafeRight(int speed) {
  leftWheelFront(FORWARD, speed);
  rightWheelFront(BACKWARD, speed);
  leftWheelBack(BACKWARD, speed);
  rightWheelBack(FORWARD, speed);
}

// Test function: move forward for 2 seconds then stop
void testForward() {
  Serial.println("Test: Moving forward for 2 seconds");
  forward(120);
  delay(2000);
  stopMotors();
  Serial.println("Test: Done");
}

// Test individual wheels to verify wiring
void testWheels() {
  Serial.println("Testing individual wheels...");
  
  Serial.println("Left Front - Forward");
  leftWheelFront(FORWARD, 150);
  delay(1000);
  stopMotors();
  delay(500);
  
  Serial.println("Right Front - Forward");
  rightWheelFront(FORWARD, 150);
  delay(1000);
  stopMotors();
  delay(500);
  
  Serial.println("Left Back - Forward");
  leftWheelBack(FORWARD, 150);
  delay(1000);
  stopMotors();
  delay(500);
  
  Serial.println("Right Back - Forward");
  rightWheelBack(FORWARD, 150);
  delay(1000);
  stopMotors();
  delay(500);
  
  Serial.println("Test complete!");
}
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

void leftWheelFront();
void rightWheelFront();
void leftWheelBack();
void rightWheelBack();

// Reads the HC-SR04 and returns distance in cm, or -1 on timeout/no-echo.
long readUltrasonicCm(int trig, int echo, unsigned long timeout = 30000UL);
void distanceSensor();

void setup() {
  Serial.begin(9600);
  Serial.println("RoverForge Motor Controller v0.1.0");
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize motor driver
  motorBegin(ENA_LM, IN1_LM, IN2_LM, ENB_LM, ENA_RM, IN1_RM, IN2_RM, ENB_RM);
}

void loop() {
  distanceSensor();
  moveForward();
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

// High-level movement functions using motor_driver API
const int DEFAULT_SPEED = 200;

void moveForward() {
  setMotorSpeeds(DEFAULT_SPEED, DEFAULT_SPEED);
}

void moveBackward() {
  setMotorSpeeds(-DEFAULT_SPEED, -DEFAULT_SPEED);
}

void turnLeft() {
  setMotorSpeeds(-DEFAULT_SPEED, DEFAULT_SPEED);
}

void turnRight() {
  setMotorSpeeds(DEFAULT_SPEED, -DEFAULT_SPEED);
}

// Legacy wheel helpers (kept for compatibility)
void leftWheelFront() {
  setMotorSpeeds(DEFAULT_SPEED, 0);
}

void rightWheelFront() {
  setMotorSpeeds(0, DEFAULT_SPEED);
}

void leftWheelBack() {
  setMotorSpeeds(-DEFAULT_SPEED, 0);
}

void rightWheelBack() {
  setMotorSpeeds(0, -DEFAULT_SPEED);
}

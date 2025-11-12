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

void leftWheelFront(MotorState state, int speed);
void rightWheelFront(MotorState state, int speed);
void leftWheelBack(MotorState state, int speed);
void rightWheelBack(MotorState state, int speed);

void stopMotors();
void forward();
void backward();
void left();
void right();

// --- Teleop / runtime settings ---
int DEFAULT_SPEED = 120;            // starting speed magnitude (0-255)
const int MIN_SPEED = 80;           // minimum useful PWM to overcome stall
unsigned long lastCmdTs = 0;
const unsigned long CMD_TIMEOUT_MS = 600; // auto-stop if no command
unsigned long lastUltraTs = 0;
const unsigned long ULTRA_CHECK_MS = 150;

void handleSerialChar(char c);
void setMotorSpeeds(int left, int right);

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
    // Non-blocking main loop: handle serial teleop, periodic ultrasonic, and safety timeout
    // Read serial single-byte commands (non-blocking)
    while (Serial.available() > 0) {
      char c = Serial.read();
      lastCmdTs = millis();
      handleSerialChar(c);
    }

    // Periodic ultrasonic checks
    if (millis() - lastUltraTs >= ULTRA_CHECK_MS) {
      lastUltraTs = millis();
      distanceSensor();
    }

    // Safety: stop if no command received recently
    if (millis() - lastCmdTs > CMD_TIMEOUT_MS) {
      stopMotors();
    }

    // small yield
    delay(5);
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

// Simple wrapper: set signed speeds for left/right (-255..255)
void setMotorSpeeds(int left, int right) {
  int l = abs(left);
  int r = abs(right);
  if (l > 0 && l < MIN_SPEED) l = MIN_SPEED;
  if (r > 0 && r < MIN_SPEED) r = MIN_SPEED;

  if (left > 0) {
    leftWheelFront(FORWARD, l);
    leftWheelBack(FORWARD, l);
  } else if (left < 0) {
    leftWheelFront(BACKWARD, l);
    leftWheelBack(BACKWARD, l);
  } else {
    leftWheelFront(STOP, 0);
    leftWheelBack(STOP, 0);
  }

  if (right > 0) {
    rightWheelFront(FORWARD, r);
    rightWheelBack(FORWARD, r);
  } else if (right < 0) {
    rightWheelFront(BACKWARD, r);
    rightWheelBack(BACKWARD, r);
  } else {
    rightWheelFront(STOP, 0);
    rightWheelBack(STOP, 0);
  }
}

// Serial command handler (single-byte commands)
void handleSerialChar(char c) {
  switch (c) {
    case 'w': // forward
    case 'W':
      setMotorSpeeds(DEFAULT_SPEED, DEFAULT_SPEED);
      Serial.println("CMD: forward");
      break;
    case 's': // stop
    case 'S':
      stopMotors();
      Serial.println("CMD: stop");
      break;
    case 'x': // backward
    case 'X':
      setMotorSpeeds(-DEFAULT_SPEED, -DEFAULT_SPEED);
      Serial.println("CMD: backward");
      break;
    case 'a': // left
    case 'A':
      setMotorSpeeds(-DEFAULT_SPEED, DEFAULT_SPEED);
      Serial.println("CMD: left");
      break;
    case 'd': // right
    case 'D':
      setMotorSpeeds(DEFAULT_SPEED, -DEFAULT_SPEED);
      Serial.println("CMD: right");
      break;
    case '+':
      DEFAULT_SPEED = min(DEFAULT_SPEED + 20, 255);
      Serial.print("Speed: "); Serial.println(DEFAULT_SPEED);
      break;
    case '-':
      DEFAULT_SPEED = max(DEFAULT_SPEED - 20, MIN_SPEED);
      Serial.print("Speed: "); Serial.println(DEFAULT_SPEED);
      break;
    case 'e':
    case 'E':
      stopMotors();
      Serial.println("CMD: E-STOP");
      break;
    default:
      // ignore unknown bytes (like newline)
      break;
  }
}

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

void stopMotors() {
  leftWheelFront(STOP, 0);
  rightWheelFront(STOP, 0);
  leftWheelBack(STOP, 0);
  rightWheelBack(STOP, 0);
}

void forward() {
  leftWheelFront(FORWARD, 60);
  rightWheelFront(FORWARD, 60);
  leftWheelBack(FORWARD, 60);
  rightWheelBack(FORWARD, 60);
}

void backward() {
  leftWheelFront(BACKWARD, 60);
  rightWheelFront(BACKWARD, 60);
  leftWheelBack(BACKWARD, 60);
  rightWheelBack(BACKWARD, 60);
}

void left() {
  leftWheelFront(BACKWARD, 60);
  rightWheelFront(FORWARD, 60);
  leftWheelBack(BACKWARD, 60);
  rightWheelBack(FORWARD, 60);
}

void right() {
  leftWheelFront(FORWARD, 60);
  rightWheelFront(BACKWARD, 60);
  leftWheelBack(FORWARD, 60);
  rightWheelBack(BACKWARD, 60);
}
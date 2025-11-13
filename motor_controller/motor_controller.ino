/*
  RoverForge Motor Controller
  Main sketch: motor control + ultrasonic sensing + serial telemetry
*/

#include <Arduino.h>
#include "motor_control.hpp"
#include "hc_sr04.hpp"
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

// Use MotorController types and API

// HC-SR04 sensor instance (uses class in hc_sr04.hpp)
hc_sr04 ultrasonic(trigPin, echoPin);

MotorController motorController;

void setup() {
  Serial.begin(9600);
  Serial.println("RoverForge Motor Controller v0.1.0");

  motorController.begin(ENA_LM, IN1_LM, IN2_LM, IN3_LM, IN4_LM, ENB_LM,
                         ENA_RM, IN1_RM, IN2_RM, IN3_RM, IN4_RM, ENB_RM);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  int speed = 250;
    // Check for serial commands
    if (Serial.available() > 0) {
      char cmd = Serial.read();
      
      if (cmd == 't' || cmd == 'T') {
        // quick forward test using class
        Serial.println("Test: Moving forward for 2 seconds");
        motorController.move(MotorController::FORWARD, speed);
        delay(2000);
        motorController.stop();
        Serial.println("Test: Done");
      } else if (cmd == 'z' || cmd == 'Z') {
        // test individual wheels via public setters
        Serial.println("Testing individual wheels...");
        Serial.println("Left Front - Forward");
        motorController.setLeftFront(MotorController::FORWARD, speed);
        delay(1000);
        motorController.stop();
        delay(500);
        Serial.println("Right Front - Forward");
        motorController.setRightFront(MotorController::FORWARD, speed);
        delay(1000);
        motorController.stop();
        delay(500);
        Serial.println("Left Back - Forward");
        motorController.setLeftBack(MotorController::FORWARD, speed);
        delay(1000);
        motorController.stop();
        delay(500);
        Serial.println("Right Back - Forward");
        motorController.setRightBack(MotorController::FORWARD, speed);
        delay(1000);
        motorController.stop();
        delay(500);
        Serial.println("Test complete!");
      } else if (cmd == 'w' || cmd == 'W') {
        Serial.println("CMD: forward");
        motorController.move(MotorController::FORWARD, speed);
      } else if (cmd == 's' || cmd == 'S') {
        Serial.println("CMD: stop");
        motorController.stop();
      } else if (cmd == 'x' || cmd == 'X') {
        Serial.println("CMD: backward");
        motorController.move(MotorController::BACKWARD, speed);
      } else if (cmd == 'a' || cmd == 'A') {
        Serial.println("CMD: left");
        // rotate left: left wheels backward, right wheels forward
        motorController.setLeftFront(MotorController::BACKWARD, speed);
        motorController.setLeftBack(MotorController::BACKWARD, speed);
        motorController.setRightFront(MotorController::FORWARD, speed);
        motorController.setRightBack(MotorController::FORWARD, speed);
      } else if (cmd == 'd' || cmd == 'D') {
        Serial.println("CMD: right");
        motorController.setLeftFront(MotorController::FORWARD, speed);
        motorController.setLeftBack(MotorController::FORWARD, speed);
        motorController.setRightFront(MotorController::BACKWARD, speed);
        motorController.setRightBack(MotorController::BACKWARD, speed);
      } else if (cmd == 'q' || cmd == 'Q') {
        Serial.println("CMD: strafe left");
        // mecanum strafe left pattern
        motorController.setLeftFront(MotorController::BACKWARD, speed);
        motorController.setRightFront(MotorController::FORWARD, speed);
        motorController.setLeftBack(MotorController::FORWARD, speed);
        motorController.setRightBack(MotorController::BACKWARD, speed);
      } else if (cmd == 'e' || cmd == 'E') {
        Serial.println("CMD: strafe right");
        motorController.setLeftFront(MotorController::FORWARD, speed);
        motorController.setRightFront(MotorController::BACKWARD, speed);
        motorController.setLeftBack(MotorController::BACKWARD, speed);
        motorController.setRightBack(MotorController::FORWARD, speed);
      }
    }

    // Print filtered distance telemetry every loop
    long distance = ultrasonic.readFiltered(3, 30);
    if (distance < 0) {
      Serial.println("Distance: no echo");
    } else {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    }

    delay(500); // wait half a second between measurements
}

// Implementation: triggers the HC-SR04, measures echo pulse width (microseconds)
// and converts to centimeters. Returns -1 if no echo was received within
// the timeout (microseconds).
// Distance reading handled by `hc_sr04` instance. See loop() for telemetry.

// Motor control implementations moved to motor_control.cpp (MotorController class)
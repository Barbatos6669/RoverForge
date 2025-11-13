#pragma once

#include <Arduino.h>
#include "motor_control.hpp"

MotorController::MotorController() : currentMode(manual) {}

void MotorController::begin(int ENA_LM, int IN1_LM, int IN2_LM, int IN3_LM, int IN4_LM, int ENB_LM,
              int ENA_RM, int IN1_RM, int IN2_RM, int IN3_RM, int IN4_RM, int ENB_RM) {
  // store pins in instance
  ENA_LM_pin = ENA_LM;
  IN1_LM_pin = IN1_LM;
  IN2_LM_pin = IN2_LM;
  IN3_LM_pin = IN3_LM;
  IN4_LM_pin = IN4_LM;
  ENB_LM_pin = ENB_LM;

  ENA_RM_pin = ENA_RM;
  IN1_RM_pin = IN1_RM;
  IN2_RM_pin = IN2_RM;
  IN3_RM_pin = IN3_RM;
  IN4_RM_pin = IN4_RM;
  ENB_RM_pin = ENB_RM;

  // initialize pins
  pinMode(ENA_LM_pin, OUTPUT);
  pinMode(IN1_LM_pin, OUTPUT);
  pinMode(IN2_LM_pin, OUTPUT);
  pinMode(IN3_LM_pin, OUTPUT);
  pinMode(IN4_LM_pin, OUTPUT);
  pinMode(ENB_LM_pin, OUTPUT);

  pinMode(ENA_RM_pin, OUTPUT);
  pinMode(IN1_RM_pin, OUTPUT);
  pinMode(IN2_RM_pin, OUTPUT);
  pinMode(IN3_RM_pin, OUTPUT);
  pinMode(IN4_RM_pin, OUTPUT);
  pinMode(ENB_RM_pin, OUTPUT);
}

void MotorController::setMode(ControllerMode mode) {
    currentMode = mode;
}

void MotorController::move(MotorState state, int speed) {
    leftWheelFront(state, speed);
    rightWheelFront(state, speed);
    leftWheelBack(state, speed);
    rightWheelBack(state, speed);
}

void MotorController::stop(int speed) {
    leftWheelFront(STOP, speed);
    rightWheelFront(STOP, speed);
    leftWheelBack(STOP, speed);
    rightWheelBack(STOP, speed);
}

void MotorController::leftWheelFront(MotorState state, int speed) {
  if (state == FORWARD) {
    // Set left front wheel to move forward at given speed
    analogWrite(ENA_LM_pin, speed);
    digitalWrite(IN1_LM_pin, HIGH);
    digitalWrite(IN2_LM_pin, LOW);
  } else if (state == BACKWARD) {
    analogWrite(ENA_LM_pin, speed);
    digitalWrite(IN1_LM_pin, LOW);
    digitalWrite(IN2_LM_pin, HIGH);
  } else {
    analogWrite(ENA_LM_pin, 0);
    digitalWrite(IN1_LM_pin, LOW);
    digitalWrite(IN2_LM_pin, LOW);
  }
}

void MotorController::rightWheelFront(MotorState state, int speed) {
  if (state == FORWARD) {
    // Set right front wheel to move forward at given speed
    analogWrite(ENB_RM_pin, speed);
    digitalWrite(IN3_RM_pin, HIGH);
    digitalWrite(IN4_RM_pin, LOW);
  } else if (state == BACKWARD) {
    analogWrite(ENB_RM_pin, speed);
    digitalWrite(IN3_RM_pin, LOW);
    digitalWrite(IN4_RM_pin, HIGH);
  } else {
    analogWrite(ENB_RM_pin, 0);
    digitalWrite(IN3_RM_pin, LOW);
    digitalWrite(IN4_RM_pin, LOW);
  }
}

void MotorController::leftWheelBack(MotorState state, int speed) {
  if (state == FORWARD) {
    // Set left back wheel to move forward at given speed
    analogWrite(ENB_LM_pin, speed);
    digitalWrite(IN3_LM_pin, HIGH);
    digitalWrite(IN4_LM_pin, LOW);
  } else if (state == BACKWARD) {
    analogWrite(ENB_LM_pin, speed);
    digitalWrite(IN3_LM_pin, LOW);
    digitalWrite(IN4_LM_pin, HIGH);
  } else {
    analogWrite(ENB_LM_pin, 0);
    digitalWrite(IN3_LM_pin, LOW);
    digitalWrite(IN4_LM_pin, LOW);
  }
}

void MotorController::rightWheelBack(MotorState state, int speed) {
  if (state == FORWARD) {
    // Set right back wheel to move forward at given speed
    analogWrite(ENA_RM_pin, speed);
    digitalWrite(IN1_RM_pin, HIGH);
    digitalWrite(IN2_RM_pin, LOW);
  } else if (state == BACKWARD) {
    analogWrite(ENA_RM_pin, speed);
    digitalWrite(IN1_RM_pin, LOW);
    digitalWrite(IN2_RM_pin, HIGH);
  } else {
    analogWrite(ENA_RM_pin, 0);
    digitalWrite(IN1_RM_pin, LOW);
    digitalWrite(IN2_RM_pin, LOW);
  }
}

// Public wrappers for per-wheel control
void MotorController::setLeftFront(MotorState state, int speed) {
  leftWheelFront(state, speed);
}

void MotorController::setRightFront(MotorState state, int speed) {
  rightWheelFront(state, speed);
}

void MotorController::setLeftBack(MotorState state, int speed) {
  leftWheelBack(state, speed);
}

void MotorController::setRightBack(MotorState state, int speed) {
  rightWheelBack(state, speed);
}




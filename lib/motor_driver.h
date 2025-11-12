#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

// Initialize motor driver pins. Use -1 for unused pins.
void motorBegin(int enaL, int in1L, int in2L, int enbL, int enaR, int in1R, int in2R, int enbR);

// Set left/right motor speeds: -255..255 (sign indicates direction)
void setMotorSpeeds(int left, int right);

// Immediate stop (sets PWM to 0, direction pins LOW)
void stopMotors();

#endif // MOTOR_DRIVER_H

#include "motor_driver.h"

static int enaL_pin = -1, in1L_pin = -1, in2L_pin = -1;
static int enaR_pin = -1, in1R_pin = -1, in2R_pin = -1;
static const int MIN_SPEED = 80; // minimum PWM to overcome stall

void motorBegin(int enaL, int in1L, int in2L, int enbL, int enaR, int in1R, int in2R, int enbR) {
  // Note: enbL/enbR reserved for future multi-channel drivers; not used here
  (void)enbL; (void)enbR;
  
  enaL_pin = enaL; in1L_pin = in1L; in2L_pin = in2L;
  enaR_pin = enaR; in1R_pin = in1R; in2R_pin = in2R;

  if (enaL_pin >= 0) pinMode(enaL_pin, OUTPUT);
  if (in1L_pin >= 0) pinMode(in1L_pin, OUTPUT);
  if (in2L_pin >= 0) pinMode(in2L_pin, OUTPUT);

  if (enaR_pin >= 0) pinMode(enaR_pin, OUTPUT);
  if (in1R_pin >= 0) pinMode(in1R_pin, OUTPUT);
  if (in2R_pin >= 0) pinMode(in2R_pin, OUTPUT);

  stopMotors();
}

static void setDirectionAndPwm(int inA, int inB, int ena, int value) {
  if (inA < 0 || inB < 0 || ena < 0) return;
  
  int pwm = abs(value);
  // Apply minimum speed if non-zero
  if (pwm > 0 && pwm < MIN_SPEED) pwm = MIN_SPEED;
  pwm = constrain(pwm, 0, 255);
  
  if (value >= 0) {
    digitalWrite(inA, HIGH);
    digitalWrite(inB, LOW);
  } else {
    digitalWrite(inA, LOW);
    digitalWrite(inB, HIGH);
  }
  analogWrite(ena, pwm);
}

void setMotorSpeeds(int left, int right) {
  setDirectionAndPwm(in1L_pin, in2L_pin, enaL_pin, left);
  setDirectionAndPwm(in1R_pin, in2R_pin, enaR_pin, right);
}

void stopMotors() {
  if (enaL_pin >= 0) analogWrite(enaL_pin, 0);
  if (enaR_pin >= 0) analogWrite(enaR_pin, 0);
  // Set direction pins low for safety
  if (in1L_pin >= 0) digitalWrite(in1L_pin, LOW);
  if (in2L_pin >= 0) digitalWrite(in2L_pin, LOW);
  if (in1R_pin >= 0) digitalWrite(in1R_pin, LOW);
  if (in2R_pin >= 0) digitalWrite(in2R_pin, LOW);
}

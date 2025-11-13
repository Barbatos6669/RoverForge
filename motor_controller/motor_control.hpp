#pragma once

#include <Arduino.h>

class MotorController {
public:
    enum ControllerMode {
        manual,
        autonomous
    };

    // Use simple, conventional motor states matching existing implementation
    enum MotorState {
        STOP,
        FORWARD,
        BACKWARD
    };

    MotorController();
    void begin(int ena_lm, int in1_lm, int in2_lm, int in3_lm, int in4_lm, int enb_lm,
               int ena_rm, int in1_rm, int in2_rm, int in3_rm, int in4_rm, int enb_rm);
    void setMode(ControllerMode mode);
    void move(MotorState state, int speed);
    void stop(int speed = 0);
    // per-wheel setters for diagnostics/tests
    void setLeftFront(MotorState state, int speed);
    void setRightFront(MotorState state, int speed);
    void setLeftBack(MotorState state, int speed);
    void setRightBack(MotorState state, int speed);

private:
    ControllerMode currentMode;

    // store pins in the instance so implementation uses these instead of globals
    int ENA_LM_pin;
    int IN1_LM_pin;
    int IN2_LM_pin;
    int IN3_LM_pin;
    int IN4_LM_pin;
    int ENB_LM_pin;

    int ENA_RM_pin;
    int IN1_RM_pin;
    int IN2_RM_pin;
    int IN3_RM_pin;
    int IN4_RM_pin;
    int ENB_RM_pin;

    void leftWheelFront(MotorState state, int speed);
    void rightWheelFront(MotorState state, int speed);
    void leftWheelBack(MotorState state, int speed);
    void rightWheelBack(MotorState state, int speed);
};
# Wiring guide (placeholder)

This page describes the recommended wiring for RoverForge's reference build.

## Motor driver (L298N) - single dual-motor driver
- Left motor (Motor A):
  - IN1 -> D2
  - IN2 -> D4
  - ENA (PWM) -> D3
- Right motor (Motor B):
  - IN3 -> D11
  - IN4 -> D12
  - ENB (PWM) -> D6

> Note: The project's sketch currently uses a specific mapping in `motor_controller/motor_controller.ino`. Verify pins before powering up.

## HC-SR04 Ultrasonic
- TRIG -> A0
- ECHO -> A1
- VCC -> 5V
- GND -> GND

## Power
- Motors: external motors power supply (observe voltage/current of motors)
- Logic: 5V regulated to Arduino UNO R4 WiFi
- Common ground required between motor supply and Arduino

## E-STOP and bump switches
- E-STOP (momentary normally-closed recommended) -> digital input with pullup; wire so a pressed button triggers LOW (or configure accordingly)

Add images or diagrams here later.

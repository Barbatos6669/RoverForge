# RoverForge Architecture

This document summarizes the recommended architecture for RoverForge.

Layers


Current focus:
- Arduino (real-time): motor control, sensors, safety watchdog, minimal command protocol.

Extensions (future):
- Raspberry Pi (optional): camera processing, high-level behaviors, web UI server, logging.
- Remote UI: browser-based teleop and telemetry display.

Key decisions

- Keep safety-critical code on Arduino (stop motors on timeout or emergency).
- Use single-byte commands for teleop and structured ASCII telemetry for parsing.
- Use a small webserver on the Pi that forwards websocket commands to Arduino via serial.

Files

- `motor_controller/` contains the Arduino sketch and Makefile.
- `lib/` is for Arduino helper modules (future sensors).

Extensions

- Replace HC-SR04 with VL53L0X for more robust range sensing.
- Add a systemd unit on Pi to run the server on boot.
- Implement higher-level autonomy on the Pi and send high-level commands to Arduino.

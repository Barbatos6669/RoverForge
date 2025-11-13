Roadmap

Short-term milestones:
- Teleop + safety watchdog on Arduino
- Stable ultrasonic sampling + filtering
- Simple web UI on Pi for teleop

Mid-term milestones:
- Camera integration and basic vision on Pi
- Telemetry logging and replay

Long-term:
- Waypoint navigation and mapping
- Modular plugin system for sensors and behaviors

Progressive Development Plan
===========================

1. Core Movement & Safety
   - [ ] Test each drive direction (forward, backward, left, right, strafe left/right) individually
   - [ ] Tune PWM values for smoothest reliable movement
   - [ ] Verify all wheels spin in correct direction (use testWheels function)
   - [ ] Implement a timed stop (e.g., stop after 5 seconds of no command)
   - [ ] Add E-STOP: wire a button, write code to stop all motors if pressed
   - [ ] Add bump switches: wire, debounce in code, and test response

2. Sensor Integration
   - [ ] Mount ultrasonic sensor securely, verify clear field of view
   - [ ] Write code to sample distance every 500ms and print to serial
   - [ ] Filter noisy readings (e.g., ignore outliers, use moving average)
   - [ ] Test sensor at different distances and angles
   - [ ] Add a second sensor (side or rear) and update code to read both
   - [ ] Integrate sensor data into movement (e.g., auto-stop if obstacle < 20cm)

3. Autonomous Behaviors
   - [ ] Write a simple loop: drive forward until obstacle, then stop
   - [ ] Add backup-and-turn routine if obstacle detected
   - [ ] Implement line following (if sensor added): tune PID, test on track
   - [ ] Add random-walk or patrol mode

4. Remote Control Expansion
   - [ ] Research and select remote control method (serial, Bluetooth, WiFi, gamepad)
   - [ ] Prototype PC-to-Arduino serial control (send 'w', 'a', 's', 'd' from PC)
   - [ ] If using Pi: write Python script to relay commands from network to Arduino
   - [ ] Test latency and reliability of remote commands
   - [ ] Add feedback: print status or sensor data back to PC

5. User Interface & Telemetry (Future)
   - [ ] Design a simple command-line or web interface for control
   - [ ] Log battery voltage, distance, and events to file or over network
   - [ ] Add live telemetry display (e.g., web dashboard or serial plotter)

6. Advanced Features
   - [ ] Add camera to Pi, stream video to PC
   - [ ] Implement basic vision (color tracking, object detection)
   - [ ] Add waypoint navigation: store and follow a list of coordinates
   - [ ] Modularize code for easy addition of new sensors/behaviors

> Break each step into small experiments. Test, observe, and document as you go. Add new ideas and lessons learned!

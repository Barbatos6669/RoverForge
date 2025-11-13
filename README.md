# RoverForge

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![CI](https://github.com/Barbatos6669/RoverForge/workflows/CI/badge.svg)](https://github.com/Barbatos6669/RoverForge/actions)
[![Version](https://img.shields.io/badge/version-0.1.0-blue.svg)](VERSION)

**RoverForge** is a production-ready modular robotics platform for building autonomous rovers with real-time motor control, sensor integration, and remote operation capabilities.

A professional three-layer architecture (Arduino real-time control, Raspberry Pi middleware, browser-based UI) enables rapid development of robust robotic vehicles for education, research, and hobby projects.

## Features

- ⚡ **Real-time Control**: Arduino-based motor control with microsecond precision and safety features (min-speed enforcement, watchdog timer)
- 🌐 **Web Interface**: Full-featured browser UI with WebSocket control, telemetry streaming, and keyboard support
- 🤖 **Modular Architecture**: Three-layer design with clean separation (Arduino/Pi/Browser) and hardware abstraction libraries
- 📡 **Telemetry Streaming**: Real-time sensor data with structured JSON parsing and visualization
- 🛠️ **Production Infrastructure**: Comprehensive CI/CD, systemd services, deployment automation, and release management
- 🔧 **Developer Experience**: Per-project Makefiles, tmux workflows, examples library, and detailed documentation

## Quick Start

### Prerequisites
- Arduino UNO R4 WiFi (or compatible board with sufficient PWM pins)
- Raspberry Pi 3/4 (for middleware server)
- L298N dual H-bridge motor driver
- HC-SR04 ultrasonic distance sensor
- Python 3.8+ with pip

### Hardware Setup
1. Wire motors to L298N driver according to `docs/WIRING.md`
2. Connect L298N control pins to Arduino (ENA/ENB to PWM pins, IN1-4 to digital pins)
3. Connect HC-SR04 sensor (TRIG=A0, ECHO=A1)
4. Connect Arduino to Raspberry Pi via USB

### Arduino Firmware
```bash
cd motor_controller
make compile      # Compiles firmware
make upload       # Uploads to board
make monitor      # Opens serial monitor (9600 baud)
```

### Test Examples
```bash
cd examples
make compile SKETCH=motor_test        # Test single motor
make upload SKETCH=ultrasonic_test    # Test distance sensor
make list                             # Show all available examples
```

### Raspberry Pi Server

Note: The previous Pi webserver and deployment helper scripts were removed to simplify the project. Pi-based features and a lightweight server can be added later as optional components — see `README_ROADMAP.md` for planned extensions.

## Repository Structure

```
motor_controller/       # Main Arduino firmware
lib/                    # Hardware abstraction libraries (future sensor libs)
examples/               # Standalone test sketches (motor_test, ultrasonic_test)
docs/                   # Architecture, wiring guides, and startup documentation
releases/               # Release notes and version artifacts
.github/                # Issue templates, PR templates, CI workflows
```

## Architecture

RoverForge is designed to be modular and simple. The current focus is on the Arduino firmware for motor and sensor control. Higher-level features (like web UI or Pi integration) can be added later as you learn and grow the project.

See `docs/ARCHITECTURE.md` for detailed design decisions.

## Hardware Support

### Motors
- Dual L298N H-bridge motor driver with 6-pin per-motor control (ENA/ENB, IN1-4)
- PWM speed control (0-255) with MIN_SPEED enforcement to overcome stall torque
- Signed speed API for intuitive forward/reverse control

### Sensors
- HC-SR04 ultrasonic distance sensor with timeout handling
- Arducam OV2640 camera (Pi integration planned)
- Extensible sensor library architecture

## Development

### Adding New Features



**Sensor Integration:**
```cpp
int distance = readUltrasonicCm(trigPin, echoPin, 30000);
```

**Web UI Commands:**
```javascript
ws.send('w');  // Forward
ws.send('s');  // Stop
```

### Testing
```bash
# Run CI checks locally
cd motor_controller && make compile

# Hardware smoke tests
cd examples
make upload SKETCH=motor_test
```

## Contributing

Contributions are welcome! Please read `CONTRIBUTING.md` for guidelines and submit PRs following the template in `.github/PULL_REQUEST_TEMPLATE.md`.

See also: `CODE_OF_CONDUCT.md`

## License

This project is MIT licensed — see `LICENSE` for details.

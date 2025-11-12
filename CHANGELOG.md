# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Changed
- Refactored motor control into `motor_driver` library with clean API (`motorBegin`, `setMotorSpeeds`, `stopMotors`)
- Simplified main sketch `setup()` from 12 pinMode calls to single `motorBegin()` call
- Added high-level movement functions (`moveForward`, `moveBackward`, `turnLeft`, `turnRight`)
- Restructured examples with per-sketch folders and unified Makefile
- Enhanced server telemetry parsing with structured JSON output for `T,*` format lines
- Updated README.md with production-ready messaging and detailed quickstart

### Added
- `lib/motor_driver.h` and `lib/motor_driver.cpp` - Motor control abstraction with MIN_SPEED enforcement
- `examples/Makefile` - Unified build system for all example sketches
- Telemetry parsing function in `server.py` for structured data streaming

## [0.1.0] - 2025-11-12
### Added
- Initial project scaffold (RoverForge)
	- Arduino sketch: `motor_controller`
	- Pi control server and web UI
	- Project docs, systemd and tmux helpers, CI skeleton

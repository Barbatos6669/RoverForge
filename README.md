# RoverForge

RoverForge is a modular robot project scaffold for the Arduino UNO R4 WiFi + a Raspberry Pi controller. It includes motor control, ultrasonic sensing, and a simple web UI for teleoperation.

Version: 0.1.0

Badges: [![CI](https://img.shields.io/badge/ci-github-blue.svg)](https://github.com/) [![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

Overview
--------
RoverForge is a template project for quickly bootstrapping a small ground rover using an Arduino and Raspberry Pi. It contains a reference Arduino sketch, a small Pi webserver for teleop, examples, and project templates for issues/PRs and releases.

Quick start
-----------
1. Compile/upload the Arduino sketch:

```bash
make -C motor_controller compile
make -C motor_controller upload
```

2. On the Pi, start the web UI server (developer mode) or enable the systemd service:

Developer (tmux):
```bash
bash tools/tmux/start_server.sh
tmux attach -t roverforge
```

Production (systemd):
```bash
sudo bash tools/sysd-service/install_service.sh
systemctl status roverforge.service
```

Repository structure
--------------------
```
motor_controller/       # Arduino sketch and Makefile
pi_control/             # Pi webserver and static UI
lib/                    # Arduino helper stubs
examples/               # Small example sketches for testing
docs/                   # design and wiring docs
tools/                  # helper scripts (tmux, systemd unit)
releases/               # release notes and artifacts
```

Using as a template
--------------------
To use this as a GitHub template repository, click "Use this template" on GitHub and then customize the files and wiring for your hardware.

License
-------
This project is MIT licensed — see `LICENSE`.

This repository is a starter template — expand the `lib/` and `pi_control/` components as you add features.

Quick start

1. Compile/upload the Arduino sketch in `motor_controller/` with the provided Makefile.
2. Optionally run the Pi webserver in `pi_control/` to get a browser-based remote control.

See `docs/ARCHITECTURE.md` for design notes and wiring guidance.

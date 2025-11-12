tmux attach -t roverforge
Starting the Pi server (recommended for production)

We recommend running the Pi webserver as a systemd service so it starts automatically on boot and is restarted on failure.

The repository includes a systemd unit at `tools/sysd-service/roverforge.service` and an installer script `tools/sysd-service/install_service.sh`.

Install and enable the service (runs the server using the project's virtualenv Python):

```bash
sudo bash /home/larry/Development/RoverForge/tools/sysd-service/install_service.sh
```

Then check status and logs:

```bash
systemctl status roverforge.service --no-pager
journalctl -u roverforge.service -f
```

Developer workflow (tmux)

For development it's convenient to run inside a `tmux` session so you can attach/detach and see logs live. The project provides helper scripts that will create a virtualenv and start the server in `tmux`.

```bash
# Start server (creates venv if needed, runs server.py in tmux)
bash tools/tmux/start_server.sh

# Attach to running tmux session
tmux attach -t roverforge

# Stop server
bash tools/tmux/stop_server.sh
```

Notes
- The start script creates a venv at `~/.venvs/roverforge` and installs packages from `requirements.txt` if needed.
- The systemd unit uses the venv python at `/home/larry/.venvs/roverforge/bin/python` by default. If you install the venv to a different path, edit `tools/sysd-service/roverforge.service` accordingly.
- The server expects the Arduino at `/dev/ttyACM0` and default baud 9600; you can override these via environment variables in the systemd unit or by editing `pi_control/server.py`.

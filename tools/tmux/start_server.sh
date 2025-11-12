#!/usr/bin/env bash
set -e

# Start RoverForge Pi server inside a tmux session with a virtualenv.
# - Creates a venv under ~/.venvs/roverforge if missing and installs requirements
# - Runs server.py in a detached tmux session, logs to server.log

VENV_DIR="$HOME/.venvs/roverforge"
REPO="$HOME/Development/RoverForge"
PI_CTRL="$REPO/pi_control"
PYTHON=$(which python3)
SESSION=roverforge

if [ -z "$PYTHON" ]; then
  echo "python3 not found in PATH"
  exit 1
fi

# create venv and install deps if necessary
if [ ! -d "$VENV_DIR" ]; then
  echo "Creating virtualenv at $VENV_DIR"
  $PYTHON -m venv "$VENV_DIR"
  "$VENV_DIR/bin/pip" install --upgrade pip
  "$VENV_DIR/bin/pip" install -r "$REPO/requirements.txt"
fi

# ensure tmux is installed
if ! command -v tmux >/dev/null 2>&1; then
  echo "tmux is required but not installed. Install with: sudo apt install tmux"
  exit 1
fi

# start tmux session with server if not already running
if ! tmux has-session -t $SESSION 2>/dev/null; then
  echo "Starting server in tmux session '$SESSION'"
  tmux new-session -d -s $SESSION
  tmux send-keys -t $SESSION "cd $PI_CTRL" C-m
  tmux send-keys -t $SESSION "$VENV_DIR/bin/python server.py 2>&1 | tee server.log" C-m
  echo "Started. Attach with: tmux attach -t $SESSION"
else
  echo "Session '$SESSION' already running. Attach with: tmux attach -t $SESSION"
fi

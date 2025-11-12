#!/usr/bin/env bash
set -e

SESSION=roverforge

if tmux has-session -t $SESSION 2>/dev/null; then
  echo "Stopping tmux session '$SESSION'"
  # send Ctrl-C to the session then kill it
  tmux send-keys -t $SESSION C-c
  sleep 1
  tmux kill-session -t $SESSION
  echo "Stopped."
else
  echo "No tmux session named '$SESSION' was running."
fi

#!/bin/bash
# Install RoverForge systemd service
set -e
SERVICE_NAME=roverforge.service
SRC_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC="$SRC_DIR/$SERVICE_NAME"
DST="/etc/systemd/system/$SERVICE_NAME"

if [ "$EUID" -ne 0 ]; then
  echo "This script must be run with sudo to install the service. Run: sudo $0"
  exit 1
fi

if [ ! -f "$SRC" ]; then
  echo "Service file not found: $SRC"
  exit 1
fi

cp "$SRC" "$DST"
chmod 644 "$DST"

systemctl daemon-reload
systemctl enable --now $SERVICE_NAME

echo "Installed and started $SERVICE_NAME"

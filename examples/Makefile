# Makefile for RoverForge examples
# Usage:
#   make compile SKETCH=motor_test
#   make upload SKETCH=ultrasonic_test
#   make monitor SKETCH=motor_test

FQBN = arduino:renesas_uno:unor4wifi
PORT = /dev/ttyACM0
BAUD = 9600

# Default sketch if not specified
SKETCH ?= motor_test

# Derived paths
SKETCH_DIR = $(CURDIR)/$(SKETCH)

.PHONY: compile upload monitor clean list help

compile:
	@echo "Compiling $(SKETCH)..."
	arduino-cli compile --fqbn $(FQBN) $(SKETCH_DIR)

upload: compile
	@echo "Uploading $(SKETCH) to $(PORT)..."
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) $(SKETCH_DIR)

monitor:
	@echo "Opening serial monitor for $(SKETCH) at $(BAUD) baud..."
	arduino-cli monitor -p $(PORT) -c baudrate=$(BAUD)

clean:
	@echo "Cleaning build artifacts for $(SKETCH)..."
	rm -rf $(SKETCH_DIR)/build

list:
	@echo "Available example sketches:"
	@ls -d */ | sed 's|/||' | grep -v "^build$$"

help:
	@echo "RoverForge Examples Makefile"
	@echo ""
	@echo "Usage:"
	@echo "  make compile SKETCH=<sketch_name>  - Compile the specified example"
	@echo "  make upload SKETCH=<sketch_name>   - Upload the specified example"
	@echo "  make monitor SKETCH=<sketch_name>  - Open serial monitor"
	@echo "  make clean SKETCH=<sketch_name>    - Clean build artifacts"
	@echo "  make list                          - List available examples"
	@echo ""
	@echo "Available sketches:"
	@$(MAKE) --no-print-directory list
	@echo ""
	@echo "Examples:"
	@echo "  make compile SKETCH=motor_test"
	@echo "  make upload SKETCH=ultrasonic_test"

FQBN = arduino:renesas_uno:unor4wifi
PORT = /dev/ttyACM0

compile:
	arduino-cli compile --fqbn $(FQBN) .

upload:
	arduino-cli compile --fqbn $(FQBN) .
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) .

monitor:
	arduino-cli monitor -p $(PORT) -c baudrate=9600

clean:
	rm -rf build/

help:
	@echo "Make targets: compile, upload, monitor, clean"

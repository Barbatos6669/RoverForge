# Top-level helper Makefile for repository maintenance

.PHONY: all build test format

all: build

build:
	make -C motor_controller compile

format:
	@echo "No-op; formatters not configured"

test:
	@echo "No tests configured yet"

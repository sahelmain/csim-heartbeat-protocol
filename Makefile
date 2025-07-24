# Heartbeat Protocol Simulation - Makefile
# Project #2: Azzam, Sahel

CC = gcc
CSIM_CC = ./csim.gcc
CFLAGS = -Wall -g
TARGET_MAIN = proj2_azzam_sahel
TARGET_LOCAL = local_test
TARGET_V2 = v2_test

# CSIM source files
MAIN_SRC = proj2_azzam_sahel.c
LOCAL_SRC = local.c
V2_SRC = V2.c

# Default target
all: help

# Help target
help:
	@echo "Heartbeat Protocol Simulation - Build Options"
	@echo "============================================="
	@echo "make hpcc       - Build main project for HPCC (requires CSIM)"
	@echo "make local      - Build local test version"
	@echo "make v2         - Build simplified V2 test version"
	@echo "make graphs     - Generate performance graphs"
	@echo "make clean      - Clean all compiled files"
	@echo "make run-local  - Build and run local test"
	@echo "make run-v2     - Build and run V2 test"

# HPCC build (requires CSIM environment)
hpcc: $(MAIN_SRC)
	@echo "Building for HPCC with CSIM..."
	$(CSIM_CC) $(MAIN_SRC) -o $(TARGET_MAIN)
	@echo "Build complete: $(TARGET_MAIN)"

# Local test build
local: $(LOCAL_SRC)
	@echo "Building local test version..."
	$(CC) $(CFLAGS) $(LOCAL_SRC) -o $(TARGET_LOCAL) -lm
	@echo "Build complete: $(TARGET_LOCAL)"

# V2 simplified build
v2: $(V2_SRC)
	@echo "Building V2 simplified version..."
	$(CC) $(CFLAGS) $(V2_SRC) -o $(TARGET_V2) -lm
	@echo "Build complete: $(TARGET_V2)"

# Generate graphs
graphs:
	@echo "Generating performance graphs..."
	python3 create_graphs.py
	@echo "Graphs generated: heartbeat_protocol_graphs.png"

# Run local test
run-local: local
	@echo "Running local test simulation..."
	./$(TARGET_LOCAL)

# Run V2 test
run-v2: v2
	@echo "Running V2 test simulation..."
	./$(TARGET_V2)

# Clean compiled files
clean:
	@echo "Cleaning compiled files..."
	rm -f $(TARGET_MAIN) $(TARGET_LOCAL) $(TARGET_V2)
	rm -f *.out *.o
	rm -rf *.dSYM
	@echo "Clean complete."

# Install dependencies (for local development)
install-deps:
	@echo "Installing Python dependencies..."
	pip3 install matplotlib numpy
	@echo "Dependencies installed."

.PHONY: all help hpcc local v2 graphs run-local run-v2 clean install-deps 
# Makefile for BlackJack project using CMake and MinGW

.PHONY: all configure build run clean

# Default target runs 'build'
all: build

# 1. Configure step: Generate MinGW Makefiles inside the build directory
configure:
	cmake -B build -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=make

# 2. Build step: Compile the project
build: configure
	cmake --build build

# 3. Run step: Build first, then execute the binary inside build directory
run: build
	./build/OrangMinyak.exe

# 4. Clean step: Clean compiled object files
clean:
	cmake --build build --target clean

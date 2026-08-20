# Makefile for BlackJack project using CMake and MinGW

.PHONY: all configure build run clean

all: buildrun

configure:
	cmake -B build -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=make

build: configure
	cmake --build build

run:
	./build/OrangMinyak.exe

buildrun: build
	./build/OrangMinyak.exe

clean:
	cmake --build build --target clean

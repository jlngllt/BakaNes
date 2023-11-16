#!/bin/bash

g++ -o emu main.cpp $(sdl2-config --cflags --libs)
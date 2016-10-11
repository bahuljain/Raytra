#!/usr/bin/env bash
rm -rf CMakeFiles/ Raytra CMakeCache.txt cmake_install.cmake raytra_render.exr prog_out

g++ -g main.cc -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -pthread -std=c++11 -o prog_out

#!/usr/bin/env bash
g++ -g tests/*.cc Vector.cc include/Vector.h -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -pthread -std=c++11 -o test_out

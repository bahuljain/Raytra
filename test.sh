#!/usr/bin/env bash
g++ -g specs/*.cc Point.cc include/Point.h Vector.cc include/Vector.h -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -pthread -std=c++11 -o test_out
./test_out
rm test_out

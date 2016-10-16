default:
	g++ -g *.cc -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -pthread -std=c++11 -o prog_out

clean:
	rm -rf CMakeFiles/ Raytra CMakeCache.txt cmake_install.cmake raytra_render.exr prog_out

test:
	g++ -g specs/*.cc Point.cc include/Point.h Vector.cc include/Vector.h Ray.cc include/Ray.h -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -pthread -std=c++11 -o test_out
	./test_out
	rm test_out

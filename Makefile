default:
	g++ -O3 *.cc -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -pthread -std=c++11 -o prog_out

debug:
	g++ -g *.cc -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -pthread -std=c++11 -o prog_out

profile:
	g++ -g -pg *.cc -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -pthread -std=c++11 -o prog_out

clean:
	rm -rf CMakeFiles/ Raytra CMakeCache.txt cmake_install.cmake raytra_render.exr prog_out

test:
	g++ -g specs/*.cc Point.cc include/Point.h Vector.cc include/Vector.h Ray.cc include/Ray.h BoundingBox.cc include/BoundingBox.h Surface.cc include/Surface.h Triangle.cc include/Triangle.h Sphere.cc include/Sphere.h -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -pthread -std=c++11 -o test_out
	./test_out
	rm test_out

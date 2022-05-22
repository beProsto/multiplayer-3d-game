CC  = gcc
CXX = g++
G = Ninja
J = 16

compile:
	cmake -B ./build -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug "-DCMAKE_C_COMPILER:FILEPATH=$(CC)" "-DCMAKE_CXX_COMPILER:FILEPATH=$(CXX)" -G "$(G)" & cmake --build ./build -j $(J)

clean:
	cd build & git clean -d -f -x
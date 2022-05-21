CC  = gcc
CXX = g++

all: clean
	make compile

compile:
	cmake -B ./build -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug "-DCMAKE_C_COMPILER:FILEPATH=$(CC)" "-DCMAKE_CXX_COMPILER:FILEPATH=$(CXX)" -G "Unix Makefiles" & cmake --build ./build -j30

clean:
	cd build & git clean -d -f -x
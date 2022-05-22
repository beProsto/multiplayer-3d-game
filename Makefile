CC  = gcc
CXX = g++
G = Ninja
J = 16

ifeq ($(OS), Windows_NT)
icon = build/icon.res
else
icon = 
endif

compile: $(icon)
	cmake -B ./build -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug "-DCMAKE_C_COMPILER:FILEPATH=$(CC)" "-DCMAKE_CXX_COMPILER:FILEPATH=$(CXX)" -G "$(G)" & cmake --build ./build -j $(J)

build/icon.res: src/icon.rc
	windres src/icon.rc -O coff build/icon.res

clean:
	cd build & git clean -d -f -x
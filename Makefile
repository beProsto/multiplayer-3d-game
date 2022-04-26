CC = clang
CXX = clang++
AR = llvm-ar

CXX_FLAGS = -std=c++17 -Wall -Wpedantic -O3
CC_FLAGS = -O3

ifeq ($(OS),Windows_NT)
EXE = app.exe
LINK_OWL = -lopengl32 -lglu32 -lgdi32 -luser32 -lcomctl32 -lkernel32 -lshell32 -lxinput
OWL_LIB_FILE = ext/OWL/build/OWL.lib
OPENAL_LIB_FILE = ext/openal-soft/build/Release/OpenAL32.lib
else
EXE = app
LINK_OWL = -lX11 -lGL -lGLX
OWL_LIB_FILE = ext/OWL/build/libOWL.a
OPENAL_LIB_FILE = ext/openal-soft/build/Release/libOpenAL32.a
endif

OWL_INCLUDE = -Iext/OWL/include
GLAD_INCLUDE = -Iext/glad/include
OPENAL_INCLUDE = -Iext/openal-soft/include
STB_INCLUDE = -Iext/stb/include

OWL_FLAGS = $(OWL_INCLUDE) $(LINK_OWL) $(OWL_LIB_FILE)
GLAD_FLAGS = $(GLAD_INCLUDE)
OPENAL_FLAGS = $(OPENAL_INCLUDE) $(OPENAL_LIB_FILE)
STB_FLAGS = $(STB_INCLUDE)

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC_DIR = src
SRCS = $(call rwildcard,$(SRC_DIR),*.cpp)
HDRS = $(call rwildcard,$(SRC_DIR),*.hpp)

OBJ_DIR = objs
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJS) objs/glad.o $(OWL_LIB_FILE)
	$(CXX) $(OBJS) objs/glad.o $(OWL_FLAGS) $(GLAD_FLAGS) $(OPENAL_FLAGS) $(STB_FLAGS) $(CXX_FLAGS) -o $(EXE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS)
	$(CXX) $(word 1, $^) $(OWL_INCLUDE) $(GLAD_INCLUDE) $(OPENAL_INCLUDE) $(STB_INCLUDE) $(CXX_FLAGS) -c -o $@

objs/glad.o: ext/glad/src/glad.c
	$(CC) ext/glad/src/glad.c $(GLAD_FLAGS) $(CC_FLAGS) -c -o objs/glad.o

$(OWL_LIB_FILE):
	cd ext & cd OWL & cd build & make justlibrary CC=$(CC) CXX=$(CXX) AR=$(AR)


clean:
	-rm $(EXE)
	-rm -r objs/*.o
	-rm -r ext/OWL/build/*.o
	-rm -r ext/OWL/build/*.lib
	-del $(EXE)
	-del /s /q objs\*.o
	-del /s /q ext\OWL\build\*.o
	-del /s /q ext\OWL\build\*.lib
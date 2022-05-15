CC = clang
CXX = clang++
AR = llvm-ar
WINDRES = windres

CXX_FLAGS = -std=c++17 -Wall -Wpedantic -O3
CC_FLAGS = -O3

ifeq ($(OS),Windows_NT)
SRV = server.exe
EXE = game.exe
LINK_OWL = -lopengl32 -lglu32 -lgdi32 -luser32 -lcomctl32 -lkernel32 -lshell32 -lxinput -lws2_32 -lmswsock -ladvapi32
OWL_LIB_FILE = ext/OWL/build/OWL.lib
OPENAL_LIB_FILE = ext/openal-soft/build/Release/OpenAL32.lib
RES = app.res
else
SRV = server
EXE = game
LINK_OWL = -lX11 -lGL -lGLX
OWL_LIB_FILE = ext/OWL/build/libOWL.a
OPENAL_LIB_FILE = ext/openal-soft/build/Release/libOpenAL32.a
RES = 
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

OBJ_DIR = objs
SRC_DIR = src

SRC_GAME_DIR = src/game
SRCS_GAME = $(call rwildcard,$(SRC_GAME_DIR),*.cpp)
OBJ_GAME_DIR = objs/game
OBJS_GAME = $(SRCS_GAME:$(SRC_GAME_DIR)/%.cpp=$(OBJ_GAME_DIR)/%.o)

SRC_SRVR_DIR = src/server
SRCS_SRVR = $(call rwildcard,$(SRC_SRVR_DIR),*.cpp)
OBJ_SRVR_DIR = objs/server
OBJS_SRVR = $(SRCS_SRVR:$(SRC_SRVR_DIR)/%.cpp=$(OBJ_SRVR_DIR)/%.o)

HDRS = $(call rwildcard,$(SRC_DIR),*.hpp)

run: $(EXE) $(SRV)
	start $(EXE) & $(SRV) 

$(SRV): $(OBJS_SRVR) $(RES)
	$(CXX) $(OBJS_SRVR) $(RES) $(LINK_OWL) $(CXX_FLAGS) -o $(SRV)

$(OBJ_SRVR_DIR)/%.o: $(SRC_SRVR_DIR)/%.cpp $(HDRS)
	$(CXX) $(word 1, $^) $(CXX_FLAGS) -c -o $@


$(EXE): $(OBJS_GAME) objs/glad.o $(OWL_LIB_FILE) $(RES)
	$(CXX) $(OBJS_GAME) objs/glad.o $(RES) $(OWL_FLAGS) $(GLAD_FLAGS) $(OPENAL_FLAGS) $(STB_FLAGS) $(CXX_FLAGS) -o $(EXE)

$(OBJ_GAME_DIR)/%.o: $(SRC_GAME_DIR)/%.cpp $(HDRS)
	$(CXX) $(word 1, $^) $(OWL_INCLUDE) $(GLAD_INCLUDE) $(OPENAL_INCLUDE) $(STB_INCLUDE) $(CXX_FLAGS) -c -o $@


objs/glad.o: ext/glad/src/glad.c
	$(CC) ext/glad/src/glad.c $(GLAD_FLAGS) $(CC_FLAGS) -c -o objs/glad.o


app.res: app.rc
	$(WINDRES) app.rc -O coff -o app.res


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
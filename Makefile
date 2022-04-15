CC = clang
CXX = clang++
CXX_FLAGS = -std=c++17 -Wall -Wpedantic

ifeq ($(OS),Windows_NT)
EXE = app.exe
LINK_OWL = -lopengl32 -lglu32 -lgdi32 -luser32 -lcomctl32 -lkernel32 -lshell32 -lxinput
else
EXE = app
LINK_OWL = -lX11 -lGL -lGLX
endif

OWL_FLAGS = -Iext/OWL/include $(LINK_OWL)
GLAD_FLAGS = -Iext/glad/include

run: $(EXE)
	./$(EXE)

$(EXE): main.cpp objs/glad.o
	$(CXX) main.cpp objs/glad.o $(GLAD_FLAGS) $(OWL_FLAGS) $(CXX_FLAGS) -o $(EXE)

objs/glad.o: ext/glad/src/glad.c
	$(CC) ext/glad/src/glad.c $(GLAD_FLAGS) $(CC_FLAGS) -c -o objs/glad.o

clean:
	-rm $(EXE)
	-rm objs/glad.o
	-del $(EXE)
	-del objs\glad.o
## Mutliplayer 3D Game

This aims to be a project using [`OWL`](https://github.com/beProsto/OWL) with modern OpenGL (3.3 to be exact), using [`GLAD`](https://glad.dav1d.de/) as a binding loader.
Using GLAD instead of GLEW makes the project free of any dll files, additionally making it more lightweight.

Current roadmap:
1. ~~Windowing~~
2. Graphics
3. Assimp
4. OpenAL ( [https://github.com/xiph/vorbis](https://github.com/xiph/vorbis) this also ) 
5. Networking


To clone the project:
```
git clone --recurse-submodules https://github.com/beProsto/multiplayer-3d-game
```
Then to compile and run the project:
```
make
```

Given you have [`make`](http://gnuwin32.sourceforge.net/packages/make.htm) and the [`clang`](https://clang.llvm.org/get_started.html) `C`/`C++` compiler installed, the project should run!
If you have any problems, please tell me about them!
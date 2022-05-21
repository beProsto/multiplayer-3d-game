cmake_minimum_required(VERSION 3.0.0)
project(Multiplayer3DGame VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(game src/game/main.cpp)
target_link_options(game PRIVATE -static)

target_include_directories(game PRIVATE ext/glad/include)
target_include_directories(game PRIVATE ext/OWL/include)
target_include_directories(game PRIVATE ext/stb/include)
target_include_directories(game PRIVATE ext/openal-soft/include)

# Linking OpenAL
target_link_directories(game PRIVATE ext/openal-soft/build/Release)
target_link_libraries(game PRIVATE OpenAL32)
target_compile_definitions(game PRIVATE AL_LIBTYPE_STATIC)

# Compiling GLAD
target_sources(game PRIVATE ext/glad/src/glad.c)

# Linking OWL
target_sources(game PRIVATE ext/OWL/src/OWL.cpp)
target_link_libraries(game PRIVATE opengl32 glu32 gdi32 user32 comctl32 kernel32 shell32 xinput ws2_32 mswsock advapi32 winmm ole32)

# Project's sources
file(GLOB_RECURSE game_SRC
	"src/game/*.h"
	"src/game/*.cpp"
)
target_sources(game PRIVATE ${game_SRC})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_minimum_required(VERSION 3.0.0)
project(Multiplayer3DGame VERSION 0.1.0)

include(CTest)
enable_testing()

# ////
# GAME
# ////

# Game Compilation
add_executable(game src/game/main.cpp)
target_link_options(game PRIVATE -static)

target_include_directories(game PRIVATE ext/glad/include)
target_include_directories(game PRIVATE ext/OWL/include)
target_include_directories(game PRIVATE ext/stb/include)
target_include_directories(game PRIVATE ext/openal-soft/include)

# Compile OpenAL
set(LIBTYPE STATIC)
set(CMAKE_BUILD_TYPE Release)
add_subdirectory(ext/openal-soft)

# Linking OpenAL
target_link_libraries(game PRIVATE OpenAL)
target_compile_definitions(game PRIVATE AL_LIBTYPE_STATIC)

# Compile OWL
set(BUILD_EXAMPLES NO)
add_subdirectory(ext/OWL)

# Linking OWL
target_link_libraries(game PRIVATE OWL)

# Compiling GLAD
target_sources(game PRIVATE ext/glad/src/glad.c)

# Linking WIN32
target_link_libraries(game PRIVATE 
	opengl32 
	glu32 
	gdi32 
	user32 
	comctl32 
	kernel32 
	shell32 
	xinput 
	ws2_32 
	mswsock 
	advapi32 
	winmm 
	ole32
)

# Game's sources
file(GLOB_RECURSE game_srcs
	"src/game/*.h"
	"src/game/*.cpp"
)
target_sources(game PRIVATE ${game_srcs})

# icon resource
if(WIN32)
target_link_options(game PRIVATE icon.res)
endif()


# ////
# SERVER
# ////

add_executable(server src/server/main.cpp)
target_link_options(server PRIVATE -static)
target_link_libraries(server PRIVATE ws2_32 mswsock)

# server's sources
file(GLOB_RECURSE server_srcs
	"src/server/*.h"
	"src/server/*.cpp"
)
target_sources(server PRIVATE ${server_srcs})

# icon resource
if(WIN32)
target_link_options(server PRIVATE icon.res)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

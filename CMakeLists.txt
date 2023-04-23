cmake_minimum_required(VERSION 3.24)
project(roads)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the Raylib and Raygui include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the Raylib library
add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/libraylib.a)

# Add the executable and source files
add_executable(roads src/main.cpp include/raymath.h src/AStar.cpp src/AStar.h src/model.h src/utility.cpp src/utility.h src/ui.cpp src/ui.h)

# Link the Raylib library to your executable
target_link_libraries(roads raylib)

# Add necessary frameworks for macOS
if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(OPENGL_LIBRARY OpenGL)
    target_link_libraries(roads ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY} ${OPENGL_LIBRARY})
endif()

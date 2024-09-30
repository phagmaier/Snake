cmake_minimum_required(VERSION 3.10)
project(raylib_example)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add maximum optimization flags
if(MSVC)
    add_compile_options(/O2 /W4)
else()
    add_compile_options(-O3 -Wall -Wextra -Wpedantic)
endif()

# Find raylib package
find_package(raylib REQUIRED)

# Add the main executable
add_executable(runme src/main.cpp)

# Link raylib to your executable
target_link_libraries(runme raylib)

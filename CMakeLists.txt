cmake_minimum_required(VERSION 3.10)
project(raylib_example)

# Set the C++ standard to C++11 (you can change this to C++14, C++17, etc., if needed)
set(CMAKE_CXX_STANDARD 11)

# Find raylib package
find_package(raylib REQUIRED)

# Add the main executable
add_executable(runme src/main.cpp)

# Link raylib to your executable
target_link_libraries(runme raylib)

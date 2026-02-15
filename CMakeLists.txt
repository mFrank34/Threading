cmake_minimum_required(VERSION 3.10)

project(Threading)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all .cpp files from src/
file(GLOB SRC_FILES "src/*.cpp")

add_executable(Threading
        src/Main.cpp
        src/basic/Basic.cpp
)

target_include_directories(Threading PRIVATE
        src/basic
)
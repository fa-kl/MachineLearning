# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

project(MachineLearning)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Unit tests
include(CTest)
enable_testing()
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG origin/main)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
include(ClangTidy)

# Subdirectories and libraries
add_subdirectory(Utils)
add_subdirectory(Test)

# Main executable
add_executable(Main Main.cpp)
target_link_libraries(Main PRIVATE Utils)
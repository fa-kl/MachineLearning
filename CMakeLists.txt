# ---------------------------------------------------------------------------------------
#
# @file: CMakeLists.txt
#
# @brief: The project's main CMakeLists.txt file
#
# @author: fakl
# @date: November 2025
#
# ---------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------------
# Setup & Settings
# ---------------------------------------------------------------------------------------
set(PROJECT_NAME "MachineLearning")
set(VERSION "1.0.0")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


cmake_minimum_required(VERSION 3.20)
project(${PROJECT_NAME} VERSION ${VERSION} LANGUAGES CXX)

# ---------------------------------------------------------------------------------------
# Coverage Options
# ---------------------------------------------------------------------------------------
option(ENABLE_COVERAGE "Enable code coverage" OFF)

# ---------------------------------------------------------------------------------------
# Warning Settings
# ---------------------------------------------------------------------------------------
# Enabling all possible warnings
if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # GCC/Clang: Enable extra warnings
    add_compile_options(
        -Wall                   # Enable common warnings
        -Wextra                 # Enable extra warnings
        -Wpedantic              # Enable strict compliance warnings
        -Werror                 # Treat warnings as errors
        -Wshadow                # Warn about variable shadowing
        -Wformat=2              # Check for format string issues (use `-Wformat=2` for stricter)
        -Wconversion            # Warn on implicit type conversions
        -Wsign-conversion       # Warn about signed/unsigned comparisons
        -Wuninitialized         # Warn if a variable is used before initialization
        -Wno-unused-parameter   # Optional: disable unused parameter warnings if needed
    )
elseif(MSVC)
    # MSVC: Enable as many warnings as possible
    add_compile_options(
        /W4               # Enable high warning level
        /WX               # Treat warnings as errors
        /wd4800           # Ignore warning about 'forcing value to bool' (optional)
    )
elseif (APPLE)
    # Apple-specific flags (Clang)
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wuninitialized
    )
endif()

# ---------------------------------------------------------------------------------------
# Coverage Flags
# ---------------------------------------------------------------------------------------
if(ENABLE_COVERAGE AND (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    message(STATUS "Code coverage enabled")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

# ---------------------------------------------------------------------------------------
# External Dependencies
# ---------------------------------------------------------------------------------------
# Add MathLib as a subdirectory
add_subdirectory(external/MathLib)

# ---------------------------------------------------------------------------------------
# Library Sources & Headers
# ---------------------------------------------------------------------------------------
# Static Library
file(GLOB PROJECT_SOURCES src/*.cpp)
add_library("${PROJECT_NAME}" STATIC ${PROJECT_SOURCES})
target_include_directories("${PROJECT_NAME}" PUBLIC inc/)
target_link_libraries("${PROJECT_NAME}" PUBLIC MathLib)

# ---------------------------------------------------------------------------------------
# Main Executable for Debugging
# ---------------------------------------------------------------------------------------
set(MAIN_TARGET "${PROJECT_NAME}Main")
add_executable(${MAIN_TARGET} main.cpp)
target_link_libraries(${MAIN_TARGET} PRIVATE "${PROJECT_NAME}")

# ---------------------------------------------------------------------------------------
# Testing
# ---------------------------------------------------------------------------------------
include(CTest)
enable_testing()
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG origin/main)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

set(TEST_TARGET "${PROJECT_NAME}Tests")
file(GLOB TEST_SOURCES test/*.cpp)
add_executable(${TEST_TARGET} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET} PRIVATE "${PROJECT_NAME}" gtest_main)

# Disable self-move warning for tests (we intentionally test self-move behavior)
if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${TEST_TARGET} PRIVATE -Wno-self-move)
elseif(MSVC)
    # MSVC doesn't have this specific warning
endif()

gtest_discover_tests(${TEST_TARGET})

# ---------------------------------------------------------------------------------------
# Coverage Target
# ---------------------------------------------------------------------------------------
if(ENABLE_COVERAGE)
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)
    
    if(LCOV AND GENHTML)
        add_custom_target(coverage
            COMMAND ${CMAKE_COMMAND} -E echo "Running tests..."
            COMMAND ${TEST_TARGET} || true
            
            COMMAND ${CMAKE_COMMAND} -E echo "Capturing coverage data..."
            COMMAND ${LCOV} --capture --directory . --output-file coverage.info --ignore-errors mismatch,gcov --rc geninfo_unexecuted_blocks=1
            
            COMMAND ${CMAKE_COMMAND} -E echo "Filtering coverage data..."
            COMMAND ${LCOV} --remove coverage.info 
                '/usr/*' 
                '*/test/*' 
                '*/googletest-*/*'
                '*/build/_deps/*'
                --output-file coverage.info --ignore-errors unused
            
            COMMAND ${CMAKE_COMMAND} -E echo "Generating HTML report..."
            COMMAND ${GENHTML} coverage.info --output-directory coverage_html --ignore-errors source
            
            COMMAND ${CMAKE_COMMAND} -E echo "Coverage report generated in coverage_html/index.html"
            
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating code coverage report"
        )
        
        add_custom_target(coverage-clean
            COMMAND ${CMAKE_COMMAND} -E remove_directory coverage_html
            COMMAND ${CMAKE_COMMAND} -E remove coverage.info
            COMMAND find . -name "*.gcda" -delete
            COMMAND find . -name "*.gcno" -delete
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Cleaning coverage data"
        )
    else()
        message(WARNING "lcov and/or genhtml not found. Coverage target not available.")
    endif()
endif()

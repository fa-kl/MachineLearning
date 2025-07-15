# LinAlg/CMakeLists.txt

# link other directories
link_directories(../Utils)

# test executables
add_executable(ComparisonTest Utils/ComparisonTest.cpp)

# link libraries
target_link_libraries(ComparisonTest PRIVATE Utils PRIVATE gtest_main)

# discover tests
gtest_discover_tests(ComparisonTest)
# LinAlg/CMakeLists.txt

# LinAlg library/target
add_library(Utils src/Utils.cpp src/Comparison.cpp)
target_include_directories(Utils PUBLIC include)
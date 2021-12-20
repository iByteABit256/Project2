add_library(
    common_lib
    input.cpp maths.cpp output.cpp
    parser.cpp point.cpp global.cpp
)

add_executable(Distance_unittest Distance_unittest.cpp)
target_link_libraries(Distance_unittest gtest_main common_lib)
gtest_discover_tests(Distance_unittest)

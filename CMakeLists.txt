cmake_minimum_required(VERSION 3.10.0)

project(Project2)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

enable_testing()

add_executable(my_test my_test.cpp)

target_link_libraries(my_test gtest_main)

include(GoogleTest)
gtest_discover_tests(my_test)

FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
add_subdirectory(lib)

add_library(
    search_lib
    hash.cpp hashtable.cpp 
    Hypercube/HyperImpl.cpp Hypercube/Hypercube.cpp 
    LSH/LSHimpl.cpp LSH/LSH.cpp
    Brute/Brute.cpp
)

add_executable(LSH_unittest LSH/LSH_unittest.cpp)
target_link_libraries(LSH_unittest gtest_main search_lib common_lib)
gtest_discover_tests(LSH_unittest)

add_executable(Hypercube_unittest Hypercube/Hypercube_unittest.cpp)
target_link_libraries(Hypercube_unittest gtest_main search_lib common_lib)
gtest_discover_tests(Hypercube_unittest)

add_executable(Frechet_unittest LSH/Frechet_unittest.cpp)
target_link_libraries(Frechet_unittest gtest_main search_lib common_lib)
gtest_discover_tests(Frechet_unittest)

add_executable(Brute_unittest Brute/Brute_unittest.cpp)
target_link_libraries(Brute_unittest gtest_main search_lib common_lib)
gtest_discover_tests(Brute_unittest)

add_library(
    lib assignment.cpp cluster.cpp 
    hash.cpp hashtable.cpp HyperImpl.cpp 
    kmeans.cpp LSHimpl.cpp maths.cpp 
    parser.cpp point.cpp LSH.cpp
    Hypercube.cpp Brute.cpp
)

add_executable(LSH_unittest LSH_unittest.cpp)
target_link_libraries(LSH_unittest gtest_main lib)
gtest_discover_tests(LSH_unittest)

add_executable(Hypercube_unittest Hypercube_unittest.cpp)
target_link_libraries(Hypercube_unittest gtest_main lib)
gtest_discover_tests(Hypercube_unittest)

add_executable(Frechet_unittest Frechet_unittest.cpp)
target_link_libraries(Frechet_unittest gtest_main lib)
gtest_discover_tests(Frechet_unittest)

add_executable(Brute_unittest Brute_unittest.cpp)
target_link_libraries(Brute_unittest gtest_main lib)
gtest_discover_tests(Brute_unittest)

add_library(
    lib assignment.cpp cluster.cpp 
    hash.cpp hashtable.cpp HyperImpl.cpp 
    kmeans.cpp LSHimpl.cpp maths.cpp 
    parser.cpp point.cpp LSH.cpp
)

add_executable(LSH_unittest LSH_unittest.cpp)
target_link_libraries(LSH_unittest gtest_main lib)
gtest_discover_tests(LSH_unittest)

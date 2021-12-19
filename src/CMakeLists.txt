add_executable(search search.cpp)
target_link_libraries(search lib)

add_executable(cluster cluster.cpp)
target_link_libraries(cluster lib)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

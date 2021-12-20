add_executable(search search.cpp)
target_link_libraries(search search_lib common_lib)

add_executable(cluster cluster.cpp)
target_link_libraries(cluster search_lib common_lib cluster_lib)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

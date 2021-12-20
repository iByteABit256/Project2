add_library(
    cluster_lib
    assignment.cpp cluster.cpp kmeans.cpp
)

target_link_libraries(cluster cluster_lib search_lib common_lib)

add_executable(lsh LSH.cpp)
target_link_libraries(lsh lib)

add_executable(hyper hypercube.cpp)
target_link_libraries(hyper lib)

add_executable(cluster Clustering.cpp)
target_link_libraries(cluster lib)

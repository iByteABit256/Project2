add_executable(lsh LSH_driver.cpp)
target_link_libraries(lsh lib)

add_executable(hyper Hypercube_driver.cpp)
target_link_libraries(hyper lib)

add_executable(cluster Clustering.cpp)
target_link_libraries(cluster lib)

add_executable(search search.cpp)
target_link_libraries(search lib)

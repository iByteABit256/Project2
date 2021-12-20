    Software development for algorithmic problems - Project 2
-----------------------------------------------------------------

                            Main Parts:

                            1) Search
                            2) Cluster



Search
---------------

Searches for nearest time curves with LSH, Hypercube, 
Discrete Frechet and Continuous Frechet methods.


Cluster
-----------

Constructs clusters of time curves with
assignment by Frechet-LSH, Vector-LSH, Hypercube and Lloyd's,
and updates using mean vector or mean curve.




Project structure
------------------

    Directories
   -------------

   -> src: Driver programs for cluster and search.
   -> lib: Algorithm implementations, helper functions, etc.
      -> cluster: Files unique to cluster
      -> common: Files for common use
      -> search: Files for search 
   -> test: Input/query files.
   -> build: Location where CMake project is built.
   -> .: README, cluster config file.
   




Instructions
-------------

Build project
-> cmake -S . -B build/

Make executables with CMake
-> cmake --build build/ (--target search|cluster|all|clean|[unit_test])

Make executables with Makefiles
-> cd build
-> make (search|cluster|all|clean|[unit_test])


Run Search
-> ./search –i <input file> –q <query file> –k <int> -L <int> -M <int> -probes
    <int> -ο <output file> -a <LSH or Hypercube or Frechet> 
    -m <discrete or continuous | only for –algorithm Frechet> -d <double>


Run Cluster
-> ./cluster –i <input file> –c <configuration file> -o <output file> 
    -u <Mean Frechet or Mean Vector> –a <Classic or LSH or Hypercube or LSH_Frechet>
    -C <optional> -s <optional>


Unit tests
-----------

Each of the following test suites has 2 test cases.

Distance_unittest, LSH_unittest, Frechet_unittest,
Hypercube_unittest, Brute_unittest.




Implementation details
-----------------------

Time curve snapping
--------------------

Time curves are snapped to grid before being hashed.
Then the curves are stored in their vector form in the hashtable.

Each one of the L grids is used to snap the curve for
the corresponding LSH hashtable.






                                         Made by

            - Μάριος Γκότζαϊ [sdi1800031]            - Παύλος Σμιθ [sdi1800181]

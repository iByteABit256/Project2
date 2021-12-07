    Software development for algorithmic problems - Project 2
-----------------------------------------------------------------

8==========D

This project consists of two main parts:

1) Classification with KNN or Range Search
2) Clustering



Classification
---------------

KNN and Range Search have been implemented with both the LSH
and the Hypercube implementations, which both aim to improve
the algorithm's efficiency for big data.


Clustering
-----------

Clustering uses the K-Means++ algorithm for cluster initialization,
it assigns the points to clusters using Lloyd's algorithm or
LSH/Hypercube for reverse assignment, and updates the clusters
based on the mean of the points assigned to them, until there
is no significant change to the clusters.


Project structure
------------------

    Directories
   -------------

   -> src: Driver programs for LSH, Hypercube and Clustering.
   -> lib: Algorithm implementations, helper functions, etc.
   -> include: Header files for every file in /lib.
   -> test: Input/query files.
   -> build: Location where object files are stored.
   -> .: Executables, README, config files, Makefile.
   

    assignment.cpp
   ----------------

    Assignment algorithms for Clustering.
    

    cluster.h/cluster.cpp
   -----------------------

    Defines the Cluster class and the 
    Config struct for storing all the information
    from the config file for Clustering.
    

    hash.h/hash.cpp
   -----------------
   
    Defines the Hash class and the Hash Handler
    class, which is used to handle the multiple
    hash functions used in LSH.
    

    hashtable.cpp
   ---------------
   
    Implementations for all the hashtable related
    functions for LSH and Hypercube.
    

    HyperImpl.cpp
   ---------------
   
    Implementation of KNN and Range Search algorithms
    for Hypercube.
        

    kmeans.cpp
   ------------
   
    K-Means++ spread-out implementation for
    clustering initialization.
    

    LSHimpl.cpp
   -------------

    Implementation of KNN and Range Search algorithms
    for LSH. 
       

    maths.cpp
   -----------

    Helper math functions implementation.

    
    parser.cpp
   ------------
   
    Parser for input/query files and config file.
   

    point.h/point.cpp
   -------------------
  
    Defines the Point class.
    

    Clustering.cpp
   ----------------

    Clustering driver.
    

    hypercube.cpp
   ---------------
   
    Hypercube driver.
    
    
    LSH.cpp
   ---------
    
    LSH driver.
    

    cluster.conf
   --------------
   
    Default config file for Clustering.


Instructions
-------------

-> 'make [lsh|cube|cluster]' for LSH, Hypercube and Clustering accordingly.

-> 'make clean' to delete object files and executables.

-> './lsh  –i  <input  file>  –q  <query  file>  –k  <int>  -L  <int>  -ο  <output  file>  -Ν 
    <number of nearest> -R <radius>' to run LSH

-> './cube  –i  <input  file>  –q  <query  file>  –k  <int>  -M  <int>  -p  <int>  -ο 
    <output file> -Ν <number of nearest> -R <radius>' to run Hypercube

    (The p flag is for probes.)

-> './cluster  –i  <input  file>  –c  <configuration  file>  -o  <output  file>  -C 
    <optional> -m <method: Classic OR LSH or Hypercube>' to run Clustering

    (The C flag is for complete.)
    





                                         Made by

            - Μάριος Γκότζαϊ [sdi1800031]            - Παύλος Σμιθ [sdi1800181]

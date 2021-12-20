#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

#include "input.h"

struct searchInfo getSearchInfo(int argc, char *argv[]){
    struct searchInfo info;
    info.M = 10, info.probes = 2, info.L = 5, info.d = 0, info.k = 4;
    info.delta = 0.69;
    info.continuous = false;

	// Command line parameters
	int opt;

	while ((opt = getopt(argc, argv, "i:q:k:L:R:o:M:p:a:m:d:")) != -1) {
        switch (opt) {
        //Input File
        case 'i':
            info.inputfile = optarg;
            break;
        // Query File
        case 'q':
            info.queryfile = optarg;
            break;
        // hash functions
        case 'k':
            info.k = atoi(optarg);
            break;
        // g functions
        case 'L':
            info.L = atoi(optarg);
            break;
        // Output File
        case 'o':
            info.outputfile = optarg;
            break;
        // Limit for hypercube
        case 'M':
            info.M = atoi(optarg);
            break;
        // Number of probes
        case 'p':
            info.probes = atoi(optarg);
            break;
        // Algorithm
        case 'a':
            info.algorithm = optarg;
            if(info.algorithm != "LSH" && info.algorithm != "Hypercube" && info.algorithm != "Frechet"){
                std::cerr << "No info.algorithm '" << info.algorithm << "', exiting program." << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        // Metric for Frechet
        case 'm':
            if(optarg == "discrete"){
                info.continuous = false;
            }else if(optarg == "continuous"){
                info.continuous = true;
            }else{
                std::cerr << "No metric '" << optarg << "', exiting program." << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        // Delta
        case 'd':
            info.delta = atoi(optarg);
            break;
        default:
        	std::cerr << "Wrong parameters passed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return info;
}

struct clusterInfo getClusterInfo(int argc, char *argv[]){
    struct clusterInfo info;
    info.assignment_method = "Classic";
    info.update_method = "Mean Vector";
    info.complete = false, info.silhouettes_enabled = false;

	int opt;

	while ((opt = getopt(argc, argv, "i:c:o:Csa:u:")) != -1) {
        switch (opt) {
		// Input file
        case 'i':
            info.inputfile = optarg;
            break;
		// Output file
        case 'o':
            info.outputfile = optarg;
            break;
		// Config file
		case 'c':
			info.configurationfile = optarg;
			break;
		// Complete flag
		case 'C':
			info.complete = true;
			break;
		// Silhouette flag
		case 's':
			info.silhouettes_enabled = true;
			break;
		// Assignment Method
		case 'a':
			info.assignment_method = optarg;
			if(info.assignment_method != "Classic" && info.assignment_method != "LSH" && info.assignment_method != "Hypercube"){
				std::cerr << "Invalid assignment_method passed" << std::endl;
				exit(EXIT_FAILURE);
			}
			break;
		// Update assignment_method
		case 'u':
			info.update_method = optarg;
			if(info.update_method != "Mean Frechet" && info.update_method != "Mean Vector"){
				std::cerr << "Invalid assignment_method passed" << std::endl;
				exit(EXIT_FAILURE);
			}
			break;
        default:
        	std::cerr << "Wrong parameters passed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

	if(info.inputfile.empty()){
		std::cerr << "Error: No input file specified" << std::endl;
        exit(EXIT_FAILURE);
	}

	if(info.assignment_method == "Hypercube" && info.update_method == "Mean Frechet"){
		std::cerr << "Error: Can't use Frechet update method with Hypercube assignment method" << std::endl;
		exit(EXIT_FAILURE);
	}

	if(info.configurationfile.empty()){
		std::cerr << "Error: No config file specified" << std::endl;
		exit(EXIT_FAILURE);
	}

    return info;
}

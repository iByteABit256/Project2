#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

#include "../lib/LSH.h"
#include "../lib/parser.h"

using namespace std;

// Hash function window
int w = 200;

int main(int argc, char *argv[]){

	string inputfile;
	string outputfile;
	string queryfile;
    string algorithm;
    string metric;

	int M = 10, probes = 2, L = 5, d = 0, n, k = 4;
    double delta;
    bool continuous;
	
	vector<Point *> querypoints;
	vector<Point *>::iterator queries;
	vector<Point *> points;
	vector<Point *> brute;

	// Command line parameters
	int opt;

	while ((opt = getopt(argc, argv, "i:q:k:L:R:o:M:p:a:m:d:")) != -1) {
        switch (opt) {
        //Input File
        case 'i':
            inputfile = optarg;
            break;
        // Query File
        case 'q':
            queryfile = optarg;
            break;
        // hash functions
        case 'k':
            k = atoi(optarg);
            break;
        // g functions
        case 'L':
            L = atoi(optarg);
            break;
        // Output File
        case 'o':
            outputfile = optarg;
            break;
        // Limit for hypercube
        case 'M':
            M = atoi(optarg);
            break;
        // Number of probes
        case 'p':
            probes = atoi(optarg);
            break;
        // Algorithm
        case 'a':
            algorithm = optarg;
            if(algorithm != "LSH" && algorithm != "Hypercube" && algorithm != "Frechet"){
                cerr << "No algorithm '" << algorithm << "', exiting program." << endl;
                return 1;
            }
            break;
        // Metric for Frechet
        case 'm':
            if(optarg == "discrete"){
                continuous = false;
            }else if(optarg == "continuous"){
                continuous = true;
            }else{
                cerr << "No metric '" << optarg << "', exiting program." << endl;
                return 1;
            }
            break;
        // Delta
        case 'd':
            delta = atoi(optarg);
            break;
        default:
        	cerr << "Wrong parameters passed" << endl;
            exit(EXIT_FAILURE);
        }
    }
    
    if(algorithm.empty()){
        cerr << "No algorithm specified." << endl;
        return 1;
    }

	// Parse input file
    points = parseinputfile(inputfile,d,n);
    if(points.empty()){
    	cerr << "Input file had an error, closing program" << endl;
		return 1;
    }
	
	// Set window to max(n/5,w) so that it scales with input
	w = max(n/5,w);

	// Parse query file
	int d_temp, n_temp;
	querypoints = parseinputfile(queryfile, d_temp, n_temp);
    if(querypoints.empty()){
    	cerr << "Query file had an error, closing program" << endl;
		return 1;
    }
    
    vector<vector<Point *>> res;
    if(algorithm == "LSH"){
        struct LSH_Info info = LSH_Initialize(points, L, k, d);
        res = LSH_KNN(points, querypoints, info, 1);
    }/*else if(algorithm == "Hypercube"){
        res = Hypercube_KNN();
    }else if(algorithm == "Frechet"){
        res = Frechet_KNN();
    }*/
    
	ostringstream ss; // Intermediary output buffer

    for(int i = 0; i < res.size(); i++){
		// Output
		ss << "Query: " << endl; 
        ss << "Nearest neighbor: " << res[i][0]->ID << endl;
		ss << "distanceLSH: " << querypoints[i]->distance(*res[i][0]) << endl;
    }

	ofstream output;
	output.open(outputfile);

	if(output.is_open()){
		output << ss.str();
		output.close();
	}else{
		cout << ss.str();
	}
	
	// Cleanup

	vector<Point *>::iterator it;
	for(it = points.begin(); it != points.end(); it++){
		delete *it;
	}

	for(it = querypoints.begin(); it != querypoints.end(); it++){
		delete *it;
	}

	return 0;
}

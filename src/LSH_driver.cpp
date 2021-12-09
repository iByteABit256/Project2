#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include <random>
#include <stdlib.h>
#include <unistd.h>
#include <climits>
#include <fstream>
#include <sstream>

#include "../lib/LSHimpl.h"
#include "../lib/parser.h"
#include "../lib/hash.h"
#include "../lib/point.h"
#include "../lib/hashtable.h"
#include "../lib/maths.h"

using namespace std;

// Hash function window
int w = 200;

int main(int argc, char *argv[]){

	string inputfile;
	string outputfile;
	string queryfile;
	float R = 10000;
	int N = 1, L = 5, d = 0, n, k = 4;
	
	vector<Point *> querypoints;
	vector<Point *>::iterator queries;
	vector<Point *> points;
	vector<Point *> brute;

	// Command line parameters
	int opt;

	while ((opt = getopt(argc, argv, "i:q:k:L:N:R:o:")) != -1) {
        switch (opt) {
        case 'i':
            inputfile = optarg;
            break;
        case 'q':
            queryfile = optarg;
            break;
        case 'k':
            k = atoi(optarg);
            break;
        case 'L':
            L = atoi(optarg);
            break;
        case 'N':
            N = atoi(optarg);
            break;
        case 'R':
           	R = strtof(optarg,NULL);
            break;
        case 'o':
            outputfile = optarg;
            break;
        default:
        	cerr << "Wrong parameters passed" << endl;
            exit(EXIT_FAILURE);
        }
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
	
	// Initialize LSH parameters
	vector<vector<int>> r(L, vector<int>(k)); // r's for each g 
	vector<vector<int>> h(L, vector<int>(k)); // selection of h_i for each g
	vector<Hash> hashes; // Hash functions
	

	// Generator with time seed 
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
	srand(seed);

    normal_distribution<float> normal(0.0,1.0);
    uniform_real_distribution<float> uniform(0.0,(float)w);
	uniform_int_distribution<int> uniform_int(INT_MIN, INT_MAX);

    for(int i = 0; i < k; i++){
		vector<float> v(d);
		float t;

	    // Initialize random vector
	    for(int j = 0; j < d; j++){
	        v[j] = normal(generator);
	    }

	    // t in [0, w)
	    t = uniform(generator);
		
		// Insert new hash function
		Hash h(v,t);
		hashes.push_back(h);
	}

	HashHandler handler = HashHandler(hashes);

	for(int i = 0; i < L; i++){
		for(int j = 0; j < k; j++){
			h[i][j] = rand()%k; // add random h to g_i
			r[i][j] = uniform_int(generator); // random r in [MIN_INT, MAX_INT)
		}
	}
	
	int tableSize;

	// Calculate L hashtables
	vector<vector<vector<Point *>>> hashtables = createHashtables(points,r,h,handler,tableSize);

	ostringstream ss; // Intermediary output buffer

	// Run algorithm for every query
	for(queries = querypoints.begin(); queries != querypoints.end(); queries++){

		Point q = **queries;

		// Calculate amplified hash for each hashtable
		vector<int> gindices = hashQuery(&q,r,h,handler,tableSize);

		// kNN
		auto knn_start = chrono::high_resolution_clock::now();
		vector<Point *> knn = kNN(&q,hashtables,gindices,N,EUCLIDIAN);
		auto knn_stop = chrono::high_resolution_clock::now();
		auto knn_duration = chrono::duration_cast<chrono::milliseconds>(knn_stop - knn_start);

		// Brute Force
		auto brute_start = chrono::high_resolution_clock::now();
		brute = bruteKNN(&q,points,N);
		auto brute_stop = chrono::high_resolution_clock::now();
		auto brute_duration = chrono::duration_cast<chrono::milliseconds>(brute_stop - brute_start);

		// Range Search
		vector<Point *> neighborsinrange = rangeSearch(&q,hashtables,gindices,R);
		
		// Output
		ss << "Query: " << q.ID << endl; 

		for(int i = 0; i < N; i++){
			if(i < knn.size()){
				ss << "Nearest neighbor-" << i+1 << ": " << knn[i]->ID << endl;
				ss << "distanceLSH: " << q.distance(*knn[i]) << endl;
			}else{
				ss << "Nearest neighbor-" << i+1 << ": " << brute[i]->ID << endl;
			}
			ss << "distanceTrue: " << q.distance(*brute[i]) << endl;
		}
		
		ss << "tLSH: " << knn_duration.count() << " ms" << endl;
		ss << "tTrue: " << brute_duration.count() << " ms" << endl;

		ss << R << "-near neighbors:" << endl;
		for(int i = 0; i < neighborsinrange.size(); i++){
			ss << neighborsinrange[i]->ID << endl;
		}
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

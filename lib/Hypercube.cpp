#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <random>
#include <stdlib.h>
#include <unistd.h>
#include <climits>

#include "Hypercube.h"
#include "hashtable.h"
#include "HyperImpl.h"


struct Hypercube_Info Hypercube_Initialize(vector<Point *> points, int k, int d, int probes, int M){
    
    struct Hypercube_Info info;

	// Initialize Hypercube parameters
	info.f = vector<unordered_map<int, char>>(k); // f values for each output of each h_i
	vector<Hash> hashes;
    
    info.probes = probes;
    info.M = M;

	// Generator with time seed 
    uint32_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
	srand(seed);

    normal_distribution<float> normal(0.0,1.0);
    uniform_real_distribution<float> uniform(0.0,(float)w);

    for(int i = 0; i < k; i++){
		vector<float> v(d);
		float t;

	    // Initialize random vector
	    for(int j = 0; j < d; j++){
	    	v[j] = normal(generator);
	    }

	    // t in [0, w)
	    t = uniform(generator);
		
		Hash h(v,t);
		hashes.push_back(h);
	}
	
	info.handler = HashHandler(hashes);

	// Calculate hashtable
	info.hashtable = createHashtable(points,info.f,info.handler);

    return info;
}


vector<vector<Point *>> Hypercube_KNN(vector<Point *> points, vector<Point *> querypoints, struct Hypercube_Info info, \
int N, float &average_duration, distance_type type){
    
    vector<vector<Point *>> res;
	unordered_map<uint32_t, vector<uint32_t>> neighbours;
	average_duration = 0;

	// Run algorithm for every query
	for(vector<Point *>::iterator queries = querypoints.begin(); queries != querypoints.end(); queries++){
		auto knn_start = chrono::high_resolution_clock::now();

		Point q = **queries;

		// Calculate hash 
		int ind = hashQuery(&q,info.f,info.handler);

		// kNN
		res.push_back(hypercubekNN(&q,ind,neighbours,info.hashtable,N,info.probes,info.M,type));
		auto knn_stop = chrono::high_resolution_clock::now();
		auto knn_duration = chrono::duration_cast<chrono::milliseconds>(knn_stop - knn_start);
		average_duration += knn_duration.count();
	}
	average_duration /= (float)querypoints.size();

    return res;
}

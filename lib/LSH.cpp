#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <random>
#include <stdlib.h>
#include <unistd.h>
#include <climits>

#include "LSH.h"
#include "hashtable.h"
#include "LSHimpl.h"


struct LSH_Info LSH_Initialize(vector<Point *> points, int L, int k, int d,distance_type type){
    
    struct LSH_Info info;

	// Initialize LSH parameters
	info.r = vector<vector<int>>(L, vector<int>(k)); // r's for each g 
	info.h = vector<vector<int>>(L, vector<int>(k)); // selection of h_i for each g
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

	info.handler = HashHandler(hashes);

	for(int i = 0; i < L; i++){
		for(int j = 0; j < k; j++){
			info.h[i][j] = rand()%k; // add random h to g_i
			info.r[i][j] = uniform_int(generator); // random r in [MIN_INT, MAX_INT)
		}
	}
	
	// Calculate L hashtables
	info.hashtables = createHashtables(points,info.r,info.h,info.handler,info.tableSize,type);
    
    return info;
}


vector<vector<Point *>> LSH_KNN(vector<Point *> points, vector<Point *> querypoints, struct LSH_Info info, \
int N, float &average_duration, distance_type type){

    vector<vector<Point *>> res;
	average_duration = 0;

	// Run algorithm for every query
	for(vector<Point *>::iterator queries = querypoints.begin(); queries != querypoints.end(); queries++){

		auto knn_start = chrono::high_resolution_clock::now();

		Point q = **queries;

		// Calculate amplified hash for each hashtable
		vector<int> gindices = hashQuery(&q,info.r,info.h,info.handler,info.tableSize);

		// kNN
		res.push_back(kNN(&q,info.hashtables,gindices,N,type));

		auto knn_stop = chrono::high_resolution_clock::now();
		auto knn_duration = chrono::duration_cast<chrono::milliseconds>(knn_stop - knn_start);
		average_duration += knn_duration.count();
	}
	average_duration /= (float)querypoints.size();
    
    return res;
}


vector<vector<Point *>> LSH_Frechet_KNN( vector<Point *> querypoints, struct LSH_Info info, \
int N, float &average_duration, distance_type type){

    vector<vector<Point *>> res;
	average_duration = 0;

	// Run algorithm for every query
	for(vector<Point *>::iterator queries = querypoints.begin(); queries != querypoints.end(); queries++){

		auto knn_start = chrono::high_resolution_clock::now();

		Point q = **queries;

		// Calculate amplified hash for each hashtable
		vector<int> gindices = hashQuery(&q,info.r,info.h,info.handler,info.tableSize);

		// kNN
		res.push_back(kNN(&q,info.hashtables,gindices,N,type));

		auto knn_stop = chrono::high_resolution_clock::now();
		auto knn_duration = chrono::duration_cast<chrono::milliseconds>(knn_stop - knn_start);
		average_duration += knn_duration.count();
	}
	average_duration /= (float)querypoints.size();
    
    return res;
}

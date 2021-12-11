#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <random>
#include <stdlib.h>
#include <unistd.h>
#include <climits>


#include "hashtable.h"
#include "LSHimpl.h"


vector<vector<Point *>> Brute_KNN(vector<Point *> points, vector<Point *> querypoints, int N, float &average_duration, distance_type type){

    vector<vector<Point *>> res;
	average_duration = 0;

	// Run algorithm for every query
	for(vector<Point *>::iterator queries = querypoints.begin(); queries != querypoints.end(); queries++){

		auto knn_start = chrono::high_resolution_clock::now();

		Point q = **queries;

		// kNN
		res.push_back(bruteKNN(&q,points,N,type));

		auto knn_stop = chrono::high_resolution_clock::now();
		auto knn_duration = chrono::duration_cast<chrono::milliseconds>(knn_stop - knn_start);
		average_duration += knn_duration.count();
	}
	average_duration /= (float)querypoints.size();
    
    return res;
}

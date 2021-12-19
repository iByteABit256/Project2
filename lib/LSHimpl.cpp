#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <tuple>
#include <algorithm>

#include "hash.h"
#include "point.h"
#include "maths.h"

using namespace std;


vector<tuple<float,Point *>> findNearestNeighbors(Point *q,vector<vector<vector<Point *>>> ghashtables, vector<int> gindices, int k, distance_type type, bool empty){
	vector<tuple<float,Point *>> nearestneighbors;

	int L = ghashtables.size();

	// For each hashtable g
	for(int i = 0; i < L; i++){
		vector<vector<Point *>> ghashtable = ghashtables[i];
		int g_i = gindices[i];

		// For each element in bucket
		for(int j = 0; j < ghashtable[g_i].size(); j++){
			Point *p = ghashtable[g_i][j];

			// Ignore if LSH ID is different
			if(type == FRECHET || empty || q->LSH_ID[i] == p->LSH_ID[i]){
				nearestneighbors.push_back(make_tuple(q->distance(*p,type),p));
			}

			// Stop if results too large 
			if(nearestneighbors.size() > 10*L){
				break;
			}
		}
	}

	return nearestneighbors;
}

vector<Point *> kNN(Point *q,vector<vector<vector<Point *>>> ghashtables, vector<int> gindices, int k, distance_type type){
	vector<tuple<float,Point *>> nearestneighbors;
	vector<tuple<float,Point *>>::iterator it;

	vector<Point *> res;

	nearestneighbors = findNearestNeighbors(q,ghashtables,gindices,k,type,false);
	if(nearestneighbors.empty()){ // Run again ignoring LSH IDs if no results given
		nearestneighbors = findNearestNeighbors(q,ghashtables,gindices,k,type,true);
	}

	sort(nearestneighbors.begin(), nearestneighbors.end(), compareDist);

	if(k > nearestneighbors.size()){
		cerr << "k is too large" << endl;
		k = nearestneighbors.size();
	}

	// First k
	for(it = nearestneighbors.begin(); it != nearestneighbors.begin()+k; it++){
		res.push_back(get<1>(*it));
	}
	
	return res;
}

vector<Point *> rangeSearch(Point *q, vector<vector<vector<Point *>>> ghashtables, vector<int> gindices, float R, distance_type type){
	vector<Point *> neighborsinrange;

	int L = ghashtables.size();

	float dist;

	// For each hashtable g
	for(int i = 0; i < L; i++){
		vector<vector<Point *>> ghashtable = ghashtables[i];
		int g_i = gindices[i];

		// For each bucket
		for(int j = 0; j < ghashtable[g_i].size(); j++){

			Point *p = ghashtable[g_i][j];

			if(type != FRECHET){
				// Ignore if different LSH ID
				if(q->LSH_ID[i] != p->LSH_ID[i]){
					continue;
				}
			}

			// Add if in range
			if((dist=q->distance(*p,type)) < R){
				neighborsinrange.push_back(p);
			}

			// Stop if results too large
			if(neighborsinrange.size() > 20*L){
				break;
			}
		}
	}

	return neighborsinrange;
}

vector<Point *> bruteKNN(Point *q, vector<Point *> points, int k, distance_type type){
	vector<tuple<float,Point *>> nearestneighbors;
	vector<tuple<float,Point *>>::iterator it;

	vector<Point *> res;
    vector<Point *>::iterator p_it;

	float dist;

	// Add every point to vector
    for(p_it = points.begin(); p_it != points.end(); p_it++){
        Point *p = *p_it;

        nearestneighbors.push_back(make_tuple(q->distance(*p, type),p));
    }

	// Sort by distance
	sort(nearestneighbors.begin(), nearestneighbors.end(), compareDist);

	if(k > nearestneighbors.size()){
		cerr << "k is too large" << endl;
		k = nearestneighbors.size();
	}

	// Get first k
	for(it = nearestneighbors.begin(); it != nearestneighbors.begin()+k; it++){
		res.push_back(get<1>(*it));
	}
	
	return res;
}

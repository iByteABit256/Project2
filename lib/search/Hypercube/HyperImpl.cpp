#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <stdint.h>

#include "../hash.h"
#include "../hashtable.h"
#include "../../common/point.h"
#include "../../common/maths.h"

using namespace std;

vector<Point*> hypercubekNN(Point* q, uint32_t qbitstring, unordered_map<uint32_t, vector<uint32_t>> &neighbours, vector<vector<Point*>> hashtable, int k, int probedist, int M, distance_type type){
	unordered_map<uint32_t, vector<uint32_t>>::iterator help;
	vector<tuple<float,Point *>> nearestneighbors;
	vector<tuple<float,Point *>>::iterator it;
	vector<Point *> res;

	vector<uint32_t> vect;

	//Create the neighbours vector of vertice qbitstring if it doesn't exist
	if( (help=(neighbours.find(qbitstring))) == neighbours.end() ){  
		neighbours.insert(make_pair<uint32_t, vector<uint32_t>> ((uint32_t)qbitstring,(vector<uint32_t>)vect) );
		//Find neighbours of vertice qbitstring with hamming distance <= probedist
		vector<uint32_t> neigh=findNeighbors(qbitstring, probedist, hashtable.size());
		help=neighbours.find(qbitstring);
		//Insert them at the neighbours vector of vertice qbitstring
		for(int j=0;j<neigh.size();j++){
			(help->second).push_back(neigh[j]);
		}
	
	}

	vector<uint32_t>::iterator neighbour;
	vector<Point*>::iterator point;

	for(neighbour=(help->second).begin();neighbour!=(help->second).end();neighbour++){
		vector<Point*> points = hashtable[(*neighbour)];
		for(point=points.begin();point!=points.end();point++){
			nearestneighbors.push_back(make_tuple(q->distance(**point,type),*point));
		}
		// Stop if results too large 
		if(nearestneighbors.size() > M){
			break;
		}
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

vector<Point*> hyperrangeSearch(Point* q, uint32_t qbitstring, unordered_map<uint32_t, vector<uint32_t>> &neighbours, vector<vector<Point*>> hashtable, float R, int probedist, int M, distance_type type){
	unordered_map<uint32_t, vector<uint32_t>>::iterator help;
	vector<Point *> neighborsinrange;

	vector<uint32_t> vect;
	float dist;

	//Create the neighbours vector of vertice qbitstring if it doesn't exist
	if( (help=(neighbours.find(qbitstring))) == neighbours.end() ){  
		neighbours.insert(make_pair<uint32_t, vector<uint32_t>> ((uint32_t)qbitstring,(vector<uint32_t>)vect) );
		//Find neighbours of vertice qbitstring with hamming distance <= probedist
		vector<uint32_t> neigh=findNeighbors(qbitstring,probedist,hashtable.size());
		help=neighbours.find(qbitstring);
		//Insert them at the neighbours vector of vertice qbitstring
		for(int j=0;j<neigh.size();j++){
			(help->second).push_back(neigh[j]);
		}
	}

	vector<uint32_t>::iterator neighbour;
	vector<Point*>::iterator point;
	
	for(neighbour=(help->second).begin();neighbour!=(help->second).end();neighbour++){
		vector<Point*> points = hashtable[(*neighbour)];
		for(point=points.begin();point!=points.end();point++){
			// Add if in range
			if((dist=q->distance(**point,type)) < R){
				neighborsinrange.push_back(*point);
			}
		}
		// Stop if results too large 
		if(neighborsinrange.size() > M){
			break;
		}
	}

	return neighborsinrange;
}

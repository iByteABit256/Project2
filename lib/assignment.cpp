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
#include <algorithm>

#include "LSHimpl.h"
#include "HyperImpl.h"
#include "parser.h"
#include "hash.h"
#include "point.h"
#include "hashtable.h"
#include "maths.h"
#include "cluster.h"
#include "assignment.h"

using namespace std;

// Maximum range search radius for reverse assignment
const int R = 1500;

// Hash function window
int w = 200;

// Wrapper function for different methods
void assignment(vector<Point *> points, vector<Cluster *> &clusters, string method, struct Config config){
	if(method == "Classic"){
		classicassignment(points,clusters);
	}else if(method == "LSH"){
		lshassignment(points,clusters,config.number_of_vector_hash_tables,config.number_of_vector_hash_functions);
	}else if(method == "Hypercube"){
		hypercubeassignment(points,clusters,config.max_number_M_hypercube,config.number_of_hypercube_dimensions,config.number_of_probes);
	}else{
		cerr << "Invalid method given!" << endl;
	}
}

// Lloyd's assignment
void classicassignment(vector<Point*> points,vector<Cluster*> &clusters){
	int n = points.size();
	float dist;
	Cluster *closestcluster;

	for(int i = 0; i < n; i++){
		float min = numeric_limits<float>::max();
		for(int j = 0; j < clusters.size(); j++){
			if((dist = points[i]->distance(*(clusters[j]->centroid), EUCLIDIAN)) < min){
				min = dist;
				closestcluster = clusters[j];
			}
		}
		closestcluster->addPoint(points[i]);
	}
}

// Reverse assignment with LSH
void lshassignment(vector<Point*> points,vector<Cluster*> &clusters,int k,int L){


	// Initialize LSH parameters
	vector<vector<int>> r(L, vector<int>(k)); // r's for each g 
	vector<vector<int>> h(L, vector<int>(k)); // selection of h_i for each g
	vector<Hash> hashes; // Hash functions
	int d = points[0]->d;
	int n = points.size();
	
	// Set window to max(n/5,w) so that it scales with input
	w = max(n/5,w);

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


	vector<Cluster*>::iterator cluster;

	int i,j;

	float radius = numeric_limits<float>::max();
	float dist;


	//Find min distance between centers
	for(int i=0;i < clusters.size();i++){
		for(int j=i+1;j < clusters.size();j++){
			if((dist=(clusters[i]->centroid)->distance(*(clusters[i]->centroid))) <  radius ){
				radius=dist;
			}
		}
	}

	//Initial value of radius
	radius=radius/2;

	float unchangedballs=0;

	unordered_map<Point*, vector<int>> matchedclusters;
	unordered_map<Point*, vector<int>>::iterator help;

	vector<int> vect;

	vector<long unsigned int> currentclustersizes(clusters.size());
	vector<vector<Point *>> newpoints (clusters.size());


	while(radius <= R &&  (unchangedballs/clusters.size() >= 0.8) && (matchedclusters.size()!=points.size()) ){
		matchedclusters.clear();

		// Run algorithm for every centroid
		for(int j=0;j<clusters.size();j++){

			Cluster* q = clusters[i];

			currentclustersizes[j] = q->points.size();

			// Calculate amplified hash for each hashtable
			vector<int> gindices = hashQuery(q->centroid,r,h,handler,tableSize);

			// Range Search
			vector<Point *> neighborsinrange = rangeSearch(q->centroid,hashtables,gindices,radius);

			//Mark points
			for(int i=0;i < neighborsinrange.size();i++){
				if( (help=(matchedclusters.find(neighborsinrange[i]))) == matchedclusters.end() ){
					matchedclusters.insert(make_pair<Point*, vector<int>> ((Point*)neighborsinrange[i],(vector<int>)vect) );
				}
				if( find((help->second).begin(),(help->second).end(),j) == (help->second).end() ){
					(help->second).push_back(j);
				}			
			}
		}

		int minindex;

		for(auto& it: matchedclusters){
			//If the point has been marked by more than 1 clusters find closest cluster
		 	if((it.second).size() > 1){
				float min=numeric_limits<float>::max();
				for(int i=0;i<(it.second).size();i++){
					if( (dist = (it.first)->distance(*(clusters[(it.second)[i]]->centroid))) < min ){
						min=dist;
						minindex=i;
					}
				}
				newpoints[(it.second)[minindex]].push_back(it.first);
		 	}else{
		 		newpoints[(it.second)[0]].push_back(it.first);
		 	}

		}

		for(int i=0;i<newpoints.size();i++){
			unchangedballs+=((newpoints[i].size()-currentclustersizes[i])==0);
			//Assign new points to cluster
			clusters[i]->points=newpoints[i]; 
		}

		//Double radius for next iteration
		radius*=2;

	}

	Cluster* closestcluster;

	//For every unassigned point find closest centroid
	for(int i=0;i<points.size();i++){
		if((matchedclusters.find(points[i])) == matchedclusters.end()){
			float min=numeric_limits<float>::max();
			for(int j=0;j<clusters.size();j++){
				if( (dist = points[i]->distance(*(clusters[j]->centroid))) < min ){
					min=dist;
					closestcluster=clusters[j];
				}
			}
			closestcluster->addPoint(points[i]);
		}
	}
}

// Reverse assignment with Hypercube
void hypercubeassignment(vector<Point*> points,vector<Cluster*> &clusters,int M,int k,int probes){
	int d = points[0]->d;
	int n = points.size();
	
	// Set window to max(n/5,w) so that it scales with input
	w = max(n/5,w);
	
	// Initialize Hypercube parameters
	vector<unordered_map<int, char>> f(k); // f values for each output of each h_i
	vector<Hash> hashes;

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
	
	HashHandler handler = HashHandler(hashes);

	// Calculate hashtable
	vector<vector<Point *>> hashtable = createHashtable(points,f,handler);

	unordered_map<uint32_t, vector<uint32_t>> neighbours;

	vector<Cluster*>::iterator cluster;

	int i,j;

	float radius = numeric_limits<float>::max();
	float dist;

	//Find min distance between centers
	for(int i=0;i < clusters.size();i++){
		for(int j=i+1;j < clusters.size();j++){
			if((dist=(clusters[i]->centroid)->distance(*(clusters[i]->centroid))) < radius){
				radius=dist;
			}
		}
	}

	//Initial value of radius
	radius=radius/2;

	float unchangedballs=0;

	unordered_map<Point*, vector<int>> matchedclusters;
	unordered_map<Point*, vector<int>>::iterator help;

	vector<int> vect;

	vector<long unsigned int> currentclustersizes(clusters.size());
	vector<vector<Point *>> newpoints (clusters.size());

	while(radius <= R &&  (unchangedballs/clusters.size() >= 0.8) && (matchedclusters.size()!=points.size()) ){
		matchedclusters.clear();

		// Run algorithm for every centroid
		for(int j=0;j<clusters.size();j++){

			Cluster* q = clusters[i];

			currentclustersizes[j] = q->points.size();

			// Calculate hash
			int ind = hashQuery(q->centroid,f,handler);

			// Range Search
			vector<Point *> neighborsinrange = hyperrangeSearch(q->centroid,ind,neighbours,hashtable,radius,probes,M);

			//Mark points
			for(int i=0;i < neighborsinrange.size();i++){
				if( (help=(matchedclusters.find(neighborsinrange[i]))) == matchedclusters.end() ){
					matchedclusters.insert(make_pair<Point*, vector<int>> ((Point*)neighborsinrange[i],(vector<int>)vect) );
				}
				if( find((help->second).begin(),(help->second).end(),j) == (help->second).end() ){
					(help->second).push_back(j);
				}			
			}
		}

		int minindex;

		for(auto& it: matchedclusters){
			//If the point has been marked by more than 1 clusters find closest cluster
		 	if((it.second).size() > 1){
				float min=numeric_limits<float>::max();
				for(int i=0;i<(it.second).size();i++){
					if( (dist = (it.first)->distance(*(clusters[(it.second)[i]]->centroid))) < min ){
						min=dist;
						minindex=i;
					}
				}
				newpoints[(it.second)[minindex]].push_back(it.first);
		 	}else{
		 		newpoints[(it.second)[0]].push_back(it.first);
		 	}

		}

		for(int i=0;i<newpoints.size();i++){
			unchangedballs+=((newpoints[i].size()-currentclustersizes[i])==0);
			//Assign new points to cluster
			clusters[i]->points=newpoints[i]; 
		}

		//Double radius for next iteration
		radius*=2;

	}

	Cluster* closestcluster;

	//For every unassigned point find closest centroid
	for(int i=0;i<points.size();i++){
		if((matchedclusters.find(points[i])) == matchedclusters.end()){
			float min=numeric_limits<float>::max();
			for(int j=0;j<clusters.size();j++){
				if( (dist = points[i]->distance(*(clusters[j]->centroid))) < min ){
					min=dist;
					closestcluster=clusters[j];
				}
			}
			closestcluster->addPoint(points[i]);
		}
	}
}

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

#include "Hypercube.h"
#include "LSH.h"
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
void assignment(vector<Point *> points, vector<Cluster *> &clusters, string assignment_method, struct Config config, string update_method){
	distance_type type = EUCLIDEAN;
	if(update_method == "Mean Curve"){
		type = FRECHET;
	}
	if(assignment_method == "Classic"){
		classicassignment(points,clusters,type);
	}else if(assignment_method == "LSH"){
		lshassignment(points,clusters,config.number_of_vector_hash_tables,config.number_of_vector_hash_functions,type);
	}else if(assignment_method == "Hypercube"){
		hypercubeassignment(points,clusters,config.max_number_M_hypercube,config.number_of_hypercube_dimensions,config.number_of_probes,type);
	}else{
		cerr << "Invalid assignment_method given!" << endl;
	}
}

// Lloyd's assignment
void classicassignment(vector<Point*> points,vector<Cluster*> &clusters, distance_type type){
	int n = points.size();
	float dist;
	Cluster *closestcluster;

	for(int i = 0; i < n; i++){
		float min = numeric_limits<float>::max();
		for(int j = 0; j < clusters.size(); j++){
			if((dist = points[i]->distance(*(clusters[j]->centroid), type)) < min){
				min = dist;
				closestcluster = clusters[j];
			}
		}
		closestcluster->addPoint(points[i]);
	}
}

// Reverse assignment with LSH
void lshassignment(vector<Point*> points,vector<Cluster*> &clusters,int k,int L, distance_type type){

	int d = points[0]->d;
	struct LSH_Info info = LSH_Initialize(points, L, k, d);

	// Calculate L hashtables
	vector<vector<vector<Point *>>> hashtables = createHashtables(points,&info,type);

	vector<Cluster*>::iterator cluster;

	int i,j;

	float radius = numeric_limits<float>::max();
	float dist;


	//Find min distance between centers
	for(int i=0;i < clusters.size();i++){
		for(int j=i+1;j < clusters.size();j++){
			if((dist=(clusters[i]->centroid)->distance(*(clusters[i]->centroid), type)) <  radius ){
				radius=dist;
			}
		}
	}

	//Initial value of radius
	radius=radius/2;

	int unchangedballs=0;

	unordered_map<Point*, vector<int>> matchedclusters;
	unordered_map<Point*, vector<int>>::iterator help;

	vector<int> vect;

	vector<long unsigned int> currentclustersizes(clusters.size());
	vector<vector<Point *>> newpoints(clusters.size());


	while(radius <= R &&  ((float)unchangedballs/clusters.size() >= 0.8) && (matchedclusters.size()!=points.size()) ){
		matchedclusters.clear();

		// Run algorithm for every centroid
		for(int j=0;j<clusters.size();j++){

			Cluster* q = clusters[j];

			currentclustersizes[j] = q->points.size();

			// Calculate amplified hash for each hashtable
			vector<int> gindices = hashQuery(q->centroid,info,type);

			// Range Search
			vector<Point *> neighborsinrange = rangeSearch(q->centroid,hashtables,gindices,radius,type);

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
					if( (dist = (it.first)->distance(*(clusters[(it.second)[i]]->centroid),type)) < min ){
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
		unchangedballs = 0;
	}

	Cluster* closestcluster;

	//For every unassigned point find closest centroid
	for(int i=0;i<points.size();i++){
		if((matchedclusters.find(points[i])) == matchedclusters.end()){
			float min=numeric_limits<float>::max();
			for(int j=0;j<clusters.size();j++){
				if( (dist = points[i]->distance(*(clusters[j]->centroid),type)) < min ){
					min=dist;
					closestcluster=clusters[j];
				}
			}
			closestcluster->addPoint(points[i]);
		}
	}
}

// Reverse assignment with Hypercube
void hypercubeassignment(vector<Point*> points,vector<Cluster*> &clusters,int M,int k,int probes, distance_type type){

	if(type == FRECHET){
		cerr << "Cannot use Frechet distance with Hypercube assignment" << endl;
		return;
	}

	int d = points[0]->d;
	struct Hypercube_Info info = Hypercube_Initialize(points, k, d, probes, M);
	
	// Calculate hashtable
	vector<vector<Point *>> hashtable = createHashtable(points,info.f,info.handler);

	unordered_map<uint32_t, vector<uint32_t>> neighbours;

	vector<Cluster*>::iterator cluster;

	int i,j;

	float radius = numeric_limits<float>::max();
	float dist;

	//Find min distance between centers
	for(int i=0;i < clusters.size();i++){
		for(int j=i+1;j < clusters.size();j++){
			if((dist=(clusters[i]->centroid)->distance(*(clusters[i]->centroid),type)) < radius){
				radius=dist;
			}
		}
	}

	//Initial value of radius
	radius=radius/2;

	int unchangedballs=0;

	unordered_map<Point*, vector<int>> matchedclusters;
	unordered_map<Point*, vector<int>>::iterator help;

	vector<int> vect;

	vector<long unsigned int> currentclustersizes(clusters.size());
	vector<vector<Point *>> newpoints (clusters.size());

	while(radius <= R &&  ((float)unchangedballs/clusters.size() >= 0.8) && (matchedclusters.size()!=points.size()) ){
		matchedclusters.clear();

		// Run algorithm for every centroid
		for(int j=0;j<clusters.size();j++){

			Cluster* q = clusters[i];

			currentclustersizes[j] = q->points.size();

			// Calculate hash
			int ind = hashQuery(q->centroid,info.f,info.handler,type);

			// Range Search
			vector<Point *> neighborsinrange = hyperrangeSearch(q->centroid,ind,neighbours,hashtable,radius,probes,M,type);

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
					if( (dist = (it.first)->distance(*(clusters[(it.second)[i]]->centroid),type)) < min ){
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
		unchangedballs = 0;
	}

	Cluster* closestcluster;

	//For every unassigned point find closest centroid
	for(int i=0;i<points.size();i++){
		if((matchedclusters.find(points[i])) == matchedclusters.end()){
			float min=numeric_limits<float>::max();
			for(int j=0;j<clusters.size();j++){
				if( (dist = points[i]->distance(*(clusters[j]->centroid),type)) < min ){
					min=dist;
					closestcluster=clusters[j];
				}
			}
			closestcluster->addPoint(points[i]);
		}
	}
}

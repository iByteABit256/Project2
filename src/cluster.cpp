#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include <random>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <limits>
#include <unordered_map>
#include <stdint.h>

#include "../lib/search/LSH/LSHimpl.h"
#include "../lib/search/hash.h"
#include "../lib/search/hashtable.h"
#include "../lib/search/Hypercube/HyperImpl.h"
#include "../lib/cluster/cluster.h"
#include "../lib/cluster/kmeans.h"
#include "../lib/cluster/assignment.h"
#include "../lib/common/maths.h"
#include "../lib/common/parser.h"
#include "../lib/common/point.h"
#include "../lib/common/output.h"
#include "../lib/common/input.h"

using namespace std;

const int MIN_DIFF = 200;

int main(int argc, char *argv[]){

	struct clusterInfo cInfo = getClusterInfo(argc, argv);

	vector<Point *> points;

	// Parse input file
    points = parseinputfile(cInfo.inputfile,cInfo.d,cInfo.n);
    if(points.empty()){
    	cerr << "Input file had an error, closing program" << endl;
		return 1;
    }

    struct Config config = parseConfig(cInfo.configurationfile);

	srand(time(NULL));

	auto start = chrono::high_resolution_clock::now();

    vector<Cluster *> clusters = initializeClusters(points, config.number_of_clusters); 

    double dist;

	// Assignment loop

   	while(1){
		// Assign points to current clusters
	   	assignment(points,clusters,cInfo.assignment_method,config,cInfo.update_method);

		// Update all clusters
	   	dist = 0;
	   	for(int i = 0; i < clusters.size(); i++){
			// cerr << "Cluster " << i << " size: " << clusters[i]->points.size() << endl;
	   		dist += clusters[i]->update(cInfo.update_method);
	   	}

		// Stop if clusters changed very little
	   	if(dist/clusters.size() < MIN_DIFF){
	   		break;
	   	}

	   	for(int i = 0; i < clusters.size(); i++){
			clusters[i]->points.clear();
	   	}
	}	

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop-start);
	
	vector<double> silhouette;
	if(cInfo.silhouettes_enabled){
		for(int i = 0; i < clusters.size(); i++){
			double cs = clusterSilhouette(clusters[i], clusters);
			silhouette.push_back(cs);
		}
		
		silhouette.push_back(totalSilhouette(clusters));
	}

	// Output

	clusterOutput(cInfo.assignment_method, clusters, duration, silhouette, cInfo.silhouettes_enabled, cInfo.complete, cInfo.outputfile);
	
	// Cleanup

	vector<Point *>::iterator it;
	for(it = points.begin(); it != points.end(); it++){
		delete *it;
	}
	
	vector<Cluster *>::iterator it2;
	for(it2 = clusters.begin(); it2 != clusters.end(); it2++){
		delete (*it2)->centroid;
		delete *it2;
	}
}

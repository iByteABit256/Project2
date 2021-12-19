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

#include "../lib/LSHimpl.h"
#include "../lib/parser.h"
#include "../lib/hash.h"
#include "../lib/point.h"
#include "../lib/hashtable.h"
#include "../lib/maths.h"
#include "../lib/HyperImpl.h"
#include "../lib/cluster.h"
#include "../lib/kmeans.h"
#include "../lib/assignment.h"
#include "../lib/output.h"
#include "../lib/input.h"

using namespace std;

const int MIN_DIFF = 200;

int main(int argc, char *argv[]){

	struct clusterInfo cInfo = getClusterInfo(argc, argv);

	if(cInfo.assignment_method == "Hypercube" && cInfo.update_method == "Mean Frechet"){
		cerr << "Error: Can't use Frechet update method with Hypercube assignment method" << endl;
		return 1;
	}

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

	// Initialization metric

	if(cInfo.complete){
		vector<double> shortest_distances;
		for(vector<Cluster *>::iterator c1 = clusters.begin(); c1 != clusters.end(); c1++){
			double min_dist = numeric_limits<double>::max();
			Point *centroid1 = (*c1)->centroid;

			for(vector<Cluster *>::iterator c2 = clusters.begin(); c2 != clusters.end(); c2++){
				if(c1 == c2){
					continue;
				}	

				Point *centroid2 = (*c2)->centroid;
				double dist = centroid1->distance(*centroid2);
				if(dist < min_dist){
					min_dist = dist;
				}
			}

			shortest_distances.push_back(min_dist);
		}

		for(vector<double>::iterator dist = shortest_distances.begin(); dist != shortest_distances.end(); dist++){
			cerr << "Centroid " << dist-shortest_distances.begin() << " distance to closest centroid: " << *dist << endl;
		}
		double average = accumulate(shortest_distances.begin(), shortest_distances.end(), 0)/(double)shortest_distances.size();
		cerr << "Average centroid distance to closest centroid: " << average << endl;
		cerr << endl;
	}

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

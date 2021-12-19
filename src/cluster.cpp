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

using namespace std;

const int MIN_DIFF = 200;

int main(int argc, char *argv[]){
	int n;
	int d;
	string inputfile, outputfile, configurationfile;
	string assignment_method, update_method;
	bool complete = false, silhouettes_enabled = false;

	int opt;

	while ((opt = getopt(argc, argv, "i:c:o:Csa:u:")) != -1) {
        switch (opt) {
		// Input file
        case 'i':
            inputfile = optarg;
            break;
		// Output file
        case 'o':
            outputfile = optarg;
            break;
		// Config file
		case 'c':
			configurationfile = optarg;
			break;
		// Complete flag
		case 'C':
			complete = true;
			break;
		// Silhouette flag
		case 's':
			silhouettes_enabled = true;
			break;
		// Assignment Method
		case 'a':
			assignment_method = optarg;
			if(assignment_method != "Classic" && assignment_method != "LSH" && assignment_method != "Hypercube"){
				cerr << "Invalid assignment_method passed" << endl;
				exit(EXIT_FAILURE);
			}
			break;
		// Update assignment_method
		case 'u':
			update_method = optarg;
			if(update_method != "Mean Frechet" && update_method != "Mean Vector"){
				cerr << "Invalid assignment_method passed" << endl;
				exit(EXIT_FAILURE);
			}
			break;
        default:
        	cerr << "Wrong parameters passed" << endl;
            exit(EXIT_FAILURE);
        }
    }

	vector<Point *> points;

	// Parse input file
    points = parseinputfile(inputfile,d,n);
    if(points.empty()){
    	cerr << "Input file had an error, closing program" << endl;
		return 1;
    }

    struct Config config = parseConfig(configurationfile);

	srand(time(NULL));

	auto start = chrono::high_resolution_clock::now();

    vector<Cluster *> clusters = initializeClusters(points, config.number_of_clusters); 

	// Initialization metric

	if(complete){
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
	}

    double dist;

	// Assignment loop

   	while(1){
		// Assign points to current clusters
	   	assignment(points,clusters,assignment_method,config);

		// Update all clusters
	   	dist = 0;
	   	for(int i = 0; i < clusters.size(); i++){
			// cerr << "Cluster " << i << " size: " << clusters[i]->points.size() << endl;
	   		dist += clusters[i]->update();
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
	if(silhouettes_enabled){
		for(int i = 0; i < clusters.size(); i++){
			double cs = clusterSilhouette(clusters[i], clusters);
			silhouette.push_back(cs);
		}
		
		silhouette.push_back(totalSilhouette(clusters));
	}

	ostringstream ss; // Intermediary output buffer

	// Output

	ss << "Algorithm: " << assignment_method << endl;	
	
	for(int i = 0; i < clusters.size(); i++){
		ss << "CLUSTER-" << i+1 << " {size: " << clusters[i]->points.size();
		ss << ", centroid: " << clusters[i]->centroid->to_str() << "}" << endl;
	}
	
	ss << "clustering_time: " << duration.count() << " ms" << endl;	
	
	if(silhouettes_enabled){
		ss << "Silhouette: [";
		for(int i = 0; i < silhouette.size(); i++){
			ss << silhouette[i];	
			if(i+1 != silhouette.size()){
				ss << ",";
			}
		}
		ss << "]" << endl;
	}
	
	if(complete){
		for(int i = 0; i < clusters.size(); i++){
			ss << "CLUSTER-" << i+1 << " {" << clusters[i]->centroid->ID << ",";
			vector<Point *> points = clusters[i]->points;
			for(int j = 0; j < points.size(); j++){
				ss << points[j]->ID;	
				if(j+1 != points.size()){
					ss << ",";
				}
			}
			ss << "}" << endl;
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
	
	vector<Cluster *>::iterator it2;
	for(it2 = clusters.begin(); it2 != clusters.end(); it2++){
		delete (*it2)->centroid;
		delete *it2;
	}
}

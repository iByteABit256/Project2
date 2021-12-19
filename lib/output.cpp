#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <chrono>

#include "point.h"
#include "cluster.h"

void searchOutput(vector<vector<Point *>> res, vector<vector<Point *>> true_res, vector<Point *> querypoints, \
string algorithm, float average_duration, float brute_average_duration, string outputfile, distance_type dist){

	ostringstream ss; // Intermediary output buffer
    float maf = -1; // Maximum approximation factor

    for(int i = 0; i < res.size(); i++){
        float approx_dist = querypoints[i]->distance(*res[i][0], dist);
        float true_dist = querypoints[i]->distance(*true_res[i][0], dist);
        float approx_factor = approx_dist/(true_dist + numeric_limits<float>::min());
        
        if(approx_factor > maf){
            maf = approx_factor;
        }

		ss << "Query: " << querypoints[i]->ID << endl; 
        ss << "Algorithm: " << algorithm << endl;
        ss << "Approximate Nearest neighbor: " << res[i][0]->ID << endl;
        ss << "True Nearest neighbor: " << true_res[i][0]->ID << endl;
        ss << "distanceApproximate: " << approx_dist << endl;
        ss << "distanceTrue: " << true_dist << endl;
    }
    ss << endl;
    ss << "tApproximateAverage: " << average_duration << " ms" << endl;
    ss << "tTrueAverage: " << brute_average_duration << " ms" << endl;
    ss << "MAF: " << maf << endl;

	ofstream output;
	output.open(outputfile);

	if(output.is_open()){
		output << ss.str();
		output.close();
	}else{
		cout << ss.str();
	}
}

void clusterOutput(string assignment_method, vector<Cluster *> clusters, chrono::milliseconds duration, \
vector<double> silhouette, bool silhouettes_enabled, bool complete, string outputfile){

	ostringstream ss; // Intermediary output buffer

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
}

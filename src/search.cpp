#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <limits>

#include "../lib/search/LSH/LSH.h"
#include "../lib/search/Hypercube/Hypercube.h"
#include "../lib/search/Brute/Brute.h"
#include "../lib/common/parser.h"
#include "../lib/common/output.h"
#include "../lib/common/input.h"
#include "../lib/common/global.h"

using namespace std;


int main(int argc, char *argv[]){

    struct searchInfo sInfo = getSearchInfo(argc, argv);

	vector<Point *> querypoints;
	vector<Point *>::iterator queries;
	vector<Point *> points;
	vector<Point *> brute;
    
    if(sInfo.algorithm.empty()){
        cerr << "No algorithm specified." << endl;
        return 1;
    }

	// Parse input file
    points = parseinputfile(sInfo.inputfile,sInfo.d,sInfo.n);
    if(points.empty()){
    	cerr << "Input file had an error, closing program" << endl;
		return 1;
    }
	
	// Set window to max(n/5,w) so that it scales with input
	w = max(sInfo.n/5,w);

	// Parse query file
	int d_temp, n_temp;
	querypoints = parseinputfile(sInfo.queryfile, d_temp, n_temp);
    if(querypoints.empty()){
    	cerr << "Query file had an error, closing program" << endl;
		return 1;
    }
    
    vector<vector<Point *>> res;
    float average_duration;
    distance_type dist = EUCLIDEAN;

    srand(time(NULL));

    if(sInfo.algorithm == "LSH"){
        struct LSH_Info info = LSH_Initialize(points, sInfo.L, sInfo.k, sInfo.d);
        res = LSH_KNN(points, querypoints, info, 1, average_duration, EUCLIDEAN);
    }else if(sInfo.algorithm == "Hypercube"){
        struct Hypercube_Info info = Hypercube_Initialize(points, sInfo.k, sInfo.d, sInfo.probes, sInfo.M);
        res = Hypercube_KNN(points, querypoints, info, 1, average_duration, EUCLIDEAN);
    }else if(sInfo.algorithm == "Frechet"){
        struct LSH_Info info = LSH_Initialize(points, sInfo.L, sInfo.k, 2*sInfo.d, FRECHET, sInfo.delta);
        res = LSH_KNN(points, querypoints, info, 1, average_duration, FRECHET);
        dist = FRECHET;
    }

    float brute_average_duration;
    vector<vector<Point *>> true_res = Brute_KNN(points, querypoints, 1, brute_average_duration, dist);
    
    // Output

    searchOutput(res, true_res, querypoints, sInfo.algorithm, average_duration, brute_average_duration, sInfo.outputfile, dist);
	
	// Cleanup

	vector<Point *>::iterator it;
	for(it = points.begin(); it != points.end(); it++){
		delete *it;
	}

	for(it = querypoints.begin(); it != querypoints.end(); it++){
		delete *it;
	}

	return 0;
}

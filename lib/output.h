#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "point.h"
#include "cluster.h"

void searchOutput(vector<vector<Point *>> res, vector<vector<Point *>> true_res, vector<Point *> querypoints, \
string algorithm, float average_duration, float brute_average_duration, string outputfile, distance_type dist);

void clusterOutput(string assignment_method, vector<Cluster *> clusters, chrono::milliseconds duration, \
vector<double> silhouette, bool silhouettes_enabled, bool complete, string outputfile);

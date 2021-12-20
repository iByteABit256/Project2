#ifndef ASSIGNMENT
#define ASSIGNMENT

#include <string>
#include <vector>
#include "cluster.h"
#include "../common/point.h"

void assignment(vector<Point *> points, vector<Cluster *> &clusters, string assignment_method, struct Config config, string update_method);
void classicassignment(vector<Point*> points,vector<Cluster*> &clusters, distance_type type);
void lshassignment(vector<Point*> points,vector<Cluster*> &clusters,int k,int L, distance_type type);
void hypercubeassignment(vector<Point*> points,vector<Cluster*> &clusters,int M,int k,int probes, distance_type type);

#endif
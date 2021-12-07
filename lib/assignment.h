#ifndef ASSIGNMENT
#define ASSIGNMENT

#include <string>
#include <vector>
#include "cluster.h"
#include "point.h"

void assignment(vector<Point *> points, vector<Cluster *> &clusters, string method, struct Config config);
void classicassignment(vector<Point*> points,vector<Cluster*> &clusters);
void lshassignment(vector<Point*> points,vector<Cluster*> &clusters,int k,int L);
void hypercubeassignment(vector<Point*> points,vector<Cluster*> &clusters,int M,int k,int probes);

#endif
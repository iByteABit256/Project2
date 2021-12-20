#ifndef MATHS
#define MATHS

#include <vector>
#include <tuple>

#include "point.h"
#include "../cluster/cluster.h"

float dot(std::vector<float> x, std::vector<float> y);
int modulo(int , int );
float meanDist(Point *q, vector<Point *> predicted);
int hammingDist(uint32_t a, uint32_t b);
bool compareDist(const tuple<float,Point *> a, const tuple<float,Point *> b);
double totalSilhouette(vector<Cluster *> clusters);
double clusterSilhouette(Cluster *cluster, vector<Cluster *> clusters);

#endif
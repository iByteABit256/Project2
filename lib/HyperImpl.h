#ifndef HYPERIMPL
#define HYPERIMPL

#include <vector>
#include <unordered_map>

#include "point.h"

vector<Point*> hypercubekNN(Point* q, uint32_t qbitstring, unordered_map<uint32_t, vector<uint32_t>> &neighbours, vector<vector<Point*>> hashtable, int k, int probedist, int M, distance_type type = EUCLIDEAN);
vector<Point*> hyperrangeSearch(Point* q, uint32_t qbitstring, unordered_map<uint32_t, vector<uint32_t>> &neighbours, vector<vector<Point*>> hashtable, float R, int probedist, int M, distance_type type = EUCLIDEAN);

#endif
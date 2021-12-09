#ifndef LSHIMPL
#define LSHIMPL

#include <vector>

#include "point.h"

vector<Point *> bruteKNN(Point *q, vector<Point *> points, int k);
vector<Point *> kNN(Point *, vector<vector<vector<Point *>>>, vector<int>, int, distance);
vector<Point *> rangeSearch(Point *, vector<vector<vector<Point *>>> , vector<int> , float);

#endif
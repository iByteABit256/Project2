#include <vector>

#include "point.h"
#include "hash.h"


vector<vector<Point *>> Brute_KNN(vector<Point *> points, vector<Point *> querypoints, int N, float &average_duration, distance_type type);

#ifndef LSH_HEADER
#define LSH_HEADER

#include <vector>

#include "../../common/point.h"
#include "../hash.h"


struct LSH_Info{
    vector<vector<double>> grid_t; // t for each grid
	vector<vector<int>> r; // r's for each g 
	vector<vector<int>> h; // selection of h_i for each g
    HashHandler handler;
    vector<vector<vector<Point *>>> hashtables;
    int tableSize;
    double delta;
};

struct LSH_Info LSH_Initialize(vector<Point *> points, int L, int k, int d, distance_type type = EUCLIDEAN, double delta = 0);
vector<vector<Point *>> LSH_KNN(vector<Point *> points, vector<Point *> querypoints, struct LSH_Info info, \
int N , float &average_duration, distance_type type = EUCLIDEAN);

#endif
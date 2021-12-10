#include <vector>

#include "point.h"
#include "hash.h"


struct LSH_Info{
	vector<vector<int>> r; // r's for each g 
	vector<vector<int>> h; // selection of h_i for each g
    HashHandler handler;
    vector<vector<vector<Point *>>> hashtables;
    int tableSize;
};

struct LSH_Info LSH_Initialize(vector<Point *> points, int L, int k, int d);
vector<vector<Point *>> LSH_KNN(vector<Point *> points, vector<Point *> querypoints, struct LSH_Info info, \
int N , float &average_duration, distance_type type);


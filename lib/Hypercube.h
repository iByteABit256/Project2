#include <vector>

#include "point.h"
#include "hash.h"


struct Hypercube_Info{
	vector<unordered_map<int, char>> f; // f values for each output of each h_i
    HashHandler handler;
    vector<vector<Point *>> hashtable;
    int probes;
    int M;
};

struct Hypercube_Info Hypercube_Initialize(vector<Point *> points, int k, int d, int probes, int M);
vector<vector<Point *>> Hypercube_KNN(vector<Point *> points, vector<Point *> querypoints, \
struct Hypercube_Info info, int N, float &average_duration, distance_type type);


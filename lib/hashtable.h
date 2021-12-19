#ifndef HASHTABLE
#define HASHTABLE

#include <vector>
#include <unordered_map>
#include <stdint.h>

#include "LSH.h"
#include "hash.h"
#include "point.h"

vector<vector<vector<Point *>>> createHashtables(vector<Point *> points, struct LSH_Info *info, distance_type type = EUCLIDEAN);
vector<vector<Point *>> createHashtable(vector<Point *> points, vector<unordered_map<int, char>> &f, HashHandler handler);
vector<int> hashQuery(Point *q, struct LSH_Info info, distance_type type = EUCLIDEAN);
int hashQuery(Point *q, vector<unordered_map<int, char>> &f, HashHandler handler, distance_type type = EUCLIDEAN);
vector<uint32_t> findNeighbors(uint32_t id, int p, int n);

#endif
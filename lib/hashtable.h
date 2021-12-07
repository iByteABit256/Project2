#ifndef HASHTABLE
#define HASHTABLE

#include <vector>
#include <unordered_map>
#include <stdint.h>

#include "hash.h"
#include "point.h"

vector<vector<vector<Point *>>> createHashtables(vector<Point *> points, vector<vector<int>> r, vector<vector<int>> h, HashHandler handler, int &tableSize);
vector<vector<Point *>> createHashtable(vector<Point *> points, vector<unordered_map<int, char>> &f, HashHandler handler);
vector<int> hashQuery(Point *q, vector<vector<int>> r, vector<vector<int>> h, HashHandler handler, int tableSize);
int hashQuery(Point *q, vector<unordered_map<int, char>> &f, HashHandler handler);
vector<uint32_t> findNeighbors(uint32_t id, int p, int n);

#endif
#ifndef HASH
#define HASH

#include <vector>
#include <unordered_map>
#include <stdint.h>

#include "../common/point.h"

using namespace std;

extern int w;

class Hash{
public:
    vector<float> v;
    float t;
    
    Hash(vector<float> v, float t);
    int h(Point &p);
};

class HashHandler{
public:
    vector<Hash> hashes;
    
    HashHandler();
    HashHandler(vector<Hash> hashes);
    int g(Point &p, vector<int> r, vector<int> h, int tableSize);
    uint32_t f_h(Point &p, vector<unordered_map<int, char>> &f);
};

#endif
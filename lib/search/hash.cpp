#include <iostream>
#include <random>
#include <chrono>
#include <math.h>
#include <climits>

#include "hash.h"
#include "../common/maths.h"
#include "../common/global.h"

using namespace std;

const int M = INT_MAX-5;

Hash::Hash(vector<float> v, float t){
    this->v = v;
    this->t = t;
}

int Hash::h(Point &p){
    return floor((dot(p.pos,v)+t)/w);
}


HashHandler::HashHandler(){
}

HashHandler::HashHandler(vector<Hash> hashes){
    this->hashes = hashes;
}

int HashHandler::g(Point &p, vector<int> r, vector<int> h, int tableSize){
    int k = hashes.size(); // maybe add as parameter later 

    int sum = 0;
    for(int i = 0; i < k; i++){
        int h_i = h[i];
        Hash hash = hashes[h_i];
        sum += r[i]*hash.h(p);
    }
    
    // Save LSH ID for minimizing distance calculations
    p.LSH_ID.push_back(modulo(sum,M));
    
    return modulo(p.LSH_ID.back(),tableSize);
}

uint32_t HashHandler::f_h(Point &p, vector<unordered_map<int, char>> &f){
    int k = hashes.size();

    uint32_t res = 0;
    for(int i = 0; i < k; i++){
        int h_i = hashes[i].h(p);

        unordered_map<int, char>& map = f[i];
        if(map.count(h_i) == 0){
            map[h_i] = rand()%2;
        }
        
        res |= (uint32_t)map[h_i];
        res = res << 1;
    }
    
    return res;
}

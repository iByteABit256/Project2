#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "hashtable.h"
#include "hash.h"
#include "maths.h"

const int bucketSizeFactor = 2; // Average bucket size

vector<vector<vector<Point *>>> createHashtables(vector<Point *> points, vector<vector<int>> r, vector<vector<int>> h, HashHandler handler, int &tableSize){
    int L = r.size(); // # amplified functions
    int n = points.size(); // # input points 
    int d = points[0]->d; // dimension of points
    int k = handler.hashes.size(); // # of h functions
    tableSize = n/bucketSizeFactor;
    
    vector<vector<vector<Point *>>> hashtables(L, vector<vector<Point *>>(tableSize));
    
    // For each point
    for(int i = 0; i < n; i++){
        // For each amplified function
        Point *p = points[i];
        for(int j = 0; j < L; j++){

            // Calculate g_j(p) and insert to j'th hashtable
            int g_j = handler.g(*p,r[j],h[j],tableSize);
            
            hashtables[j][g_j].push_back(p);
        }
    }
    
    return hashtables;
}

vector<vector<Point *>> createHashtable(vector<Point *> points, vector<unordered_map<int, char>> &f, HashHandler handler){
    int n = points.size(); // # input points 
    int d = points[0]->d; // dimension of points
    int k = handler.hashes.size(); // # of h functions
    
    int size = 1 << k+1; // 2^(k+1)
    vector<vector<Point *>> hashtable(size);
    
    // For each point
    for(int i = 0; i < n; i++){
        Point *p = points[i];

        // Calculate hash
        uint32_t key = handler.f_h(*p,f);
        
        hashtable[key].push_back(p);
    }
    
    return hashtable;
}

// Get neighbors of bucket, up to a certain probe distance
vector<uint32_t> findNeighbors(uint32_t id, int p, int n){
    vector<pair<int,uint32_t>> temp; 

    for(uint32_t i = 0; i < n; i++){
        int dist = hammingDist(id,i);

        if(dist <= p){
            temp.push_back(make_pair<int,uint32_t>((int)dist, (uint32_t)i));
        }
    }
    
    sort(temp.begin(), temp.end());
    
    vector<uint32_t> res;
    for(vector<pair<int,uint32_t>>::iterator it = temp.begin(); it != temp.end(); it++){
        res.push_back(get<1>(*it));
    }
    
    return res;
}

// Hash query with every amplified function g
vector<int> hashQuery(Point *q, vector<vector<int>> r, vector<vector<int>> h, HashHandler handler, int tableSize){
    int L = r.size(); // # amplified functions
    int d = q->d; // dimension of points
    int k = handler.hashes.size(); // # of h functions
    
    vector<int> gindices;
    
    for(int i = 0; i < L; i++){

        // Calculate g_i(p)
        int g_i = handler.g(*q,r[i],h[i],tableSize);

        gindices.push_back(g_i);
    }
    
    return gindices;
}

// Hash query for hypercube
int hashQuery(Point *q, vector<unordered_map<int, char>> &f, HashHandler handler){
    int d = q->d; // dimension of points
    int k = handler.hashes.size(); // # of h functions
    
    return handler.f_h(*q,f);
}

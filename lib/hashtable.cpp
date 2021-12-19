#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <random>

#include "LSH.h"
#include "hashtable.h"
#include "hash.h"
#include "maths.h"
#include "point.h"

const int bucketSizeFactor = 2; // Average bucket size


Point *snapToGrid(Point *p, vector<vector<double>> grid_t, int L, double delta){
    vector<float> vec;
    for(int i = 0; i < p->d; i++){
        float x = (float)(i+1);
        float y = p->pos[i];

        float t_x = grid_t[0][L];
        float t_y = grid_t[1][L];

        // xi' = floor((xi-t)/δ + 1/2)*δ + t
        float x_snapped = floor((x-t_x)/delta + 0.5)*delta+t_x;
        float y_snapped = floor((y-t_y)/delta + 0.5)*delta+t_y;

        vec.push_back(x_snapped);
        vec.push_back(y_snapped);
    }

    Point *snapped_point = new Point(vec);
    return snapped_point;
}

vector<vector<vector<Point *>>> createHashtables(vector<Point *> points, struct LSH_Info *info, distance_type type){
    int L = info->r.size(); // # amplified functions
    int n = points.size(); // # input points 
    info->tableSize = n/bucketSizeFactor;
    
    vector<vector<vector<Point *>>> hashtables(L, vector<vector<Point *>>(info->tableSize));

    if(type == FRECHET){
        default_random_engine generator;
        uniform_real_distribution<double> distribution(0.0,info->delta);

        info->grid_t = vector<vector<double>>(2);
        for(int i = 0; i < 2; i ++){
            for(int j = 0; j < L; j++){
                info->grid_t[i].push_back(distribution(generator));
            }
        }
    }
    
    // For each point
    for(int i = 0; i < n; i++){
        // For each amplified function
        Point *p = points[i];
        for(int j = 0; j < L; j++){

            if(type == FRECHET){
                Point *snapped = snapToGrid(p, info->grid_t, j, info->delta);
                // cerr << "snapped point: " << endl << snapped->to_str() << endl;

                int g_j = info->handler.g(*snapped,info->r[j],info->h[j],info->tableSize);
                hashtables[j][g_j].push_back(p);
            }else{
                // Calculate g_j(p) and insert to j'th hashtable
                int g_j = info->handler.g(*p,info->r[j],info->h[j],info->tableSize);
                hashtables[j][g_j].push_back(p);
            }
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
vector<int> hashQuery(Point *q, struct LSH_Info info, distance_type type){
    int L = info.r.size(); // # amplified functions
    int d = q->d; // dimension of points
    int k = info.handler.hashes.size(); // # of h functions
    
    vector<int> gindices;
    
    for(int i = 0; i < L; i++){

        int g_i;

        if(type == FRECHET){
            Point *snapped = snapToGrid(q, info.grid_t, i, info.delta);
            g_i = info.handler.g(*snapped,info.r[i],info.h[i],info.tableSize);
        }else{
            g_i = info.handler.g(*q,info.r[i],info.h[i],info.tableSize);
        }
        gindices.push_back(g_i);
    }
    
    return gindices;
}

// Hash query for hypercube
int hashQuery(Point *q, vector<unordered_map<int, char>> &f, HashHandler handler, distance_type type){
    int d = q->d; // dimension of points
    int k = handler.hashes.size(); // # of h functions
    
    return handler.f_h(*q,f);
}

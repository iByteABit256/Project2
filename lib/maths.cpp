#include <iostream>
#include <algorithm>
#include <numeric>

#include "maths.h"

using namespace std;

// Dot product of two vectors
float dot(vector<float> x, vector<float> y){
    int n = x.size();
    float res = 0;

    if((n = x.size()) != y.size()){
        throw invalid_argument("Dot product with different sizes");
    }

    for(int i = 0; i < n; i++){
        res += x[i]*y[i]; 
    }

    return res;
}

// Modulo following Knuth's floored definition
int modulo(int a, int b){
    int r = a % b;
    return r < 0? r+b : r;
}

// Mean distance from point
float meanDist(Point *q, vector<Point *> predicted){
    float dist = 0;
    vector<Point *>::iterator it;

    for(it = predicted.begin(); it != predicted.end(); it++){
        Point *p = *it;
        dist += q->distance(*p);
    }
    
    return dist/predicted.size();
}

// Hamming distance between two unsigned ints
int hammingDist(uint32_t a, uint32_t b){
    int dist = 0;

    uint32_t x = a^b; // XOR
    while(x > 0){
        dist += x & 1; // Increment if last bit is 1
        x >>= 1;
    }
    
    return dist;
}

// Compare distances of points in vector of tuples
bool compareDist(const tuple<float,Point *> a, const tuple<float,Point *> b){
	return get<0>(a) < get<0>(b);
}

Cluster *getNextBestCluster(Point *p, vector<Cluster *> clusters){
    vector<pair<float, Cluster *>> dist;
    vector<Cluster *>::iterator it;
    
    for(it = clusters.begin(); it != clusters.end(); it++){
        dist.push_back(make_pair(p->distance(*((*it)->centroid)),*it));
    }
    
    sort(dist.begin(), dist.end());
    
    if(dist.size() < 2) return dist[0].second;

    return dist[1].second;
}

double totalSilhouette(vector<Cluster *> clusters){
    vector<float> sil;
    vector<Cluster *>::iterator it;
    vector<Point *>::iterator it2;
    
    for(it = clusters.begin(); it != clusters.end(); it++){
        for(it2 = (*it)->points.begin(); it2 != (*it)->points.end(); it2++){
            if((*it2)->sil_a < 0){
                (*it2)->sil_a = meanDist(*it2, (*it)->points);
            }
            if((*it2)->sil_b < 0){
                (*it2)->sil_b = meanDist(*it2, getNextBestCluster(*it2, clusters)->points);
            }

            float a = (*it2)->sil_a; 
            float b = (*it2)->sil_b;

            sil.push_back((b-a)/max(a,b));
        }
    }
    
    return (double)accumulate(sil.begin(), sil.end(), 0.0)/(double)sil.size();
}

double clusterSilhouette(Cluster *cluster, vector<Cluster *> clusters){
    vector<float> sil;
    vector<Point *>::iterator it;
    
    for(it = cluster->points.begin(); it != cluster->points.end(); it++){
        if((*it)->sil_a < 0){
            (*it)->sil_a = meanDist(*it, cluster->points);
        }
        if((*it)->sil_b < 0){
            (*it)->sil_b = meanDist(*it, getNextBestCluster(*it, clusters)->points);
        }
        
        float a = (*it)->sil_a; 
        float b = (*it)->sil_b;

        sil.push_back((b-a)/max(a,b));
    }
    
    return (double)accumulate(sil.begin(), sil.end(), 0.0)/(double)sil.size();
}

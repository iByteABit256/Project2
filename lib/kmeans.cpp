#include <iostream>
#include <limits>
#include <numeric>
#include <algorithm>

#include "kmeans.h"

// Squares of D(i)
vector<float> D_sqr(vector<Point *> points, vector<Cluster *> clusters, int n, int t){
    vector<float> dist(n);

    for(int i = 0; i < n; i++){
        dist[i] = numeric_limits<float>::max();
        for(int j = 0; j < t; j++){
            float d;
            if((d = points[i]->distance(*clusters[j]->centroid)) < dist[i]){
                dist[i] = d;
            }
        }
        
        dist[i] *= dist[i];
    }
    
    return dist;
}

// Calculates probabilty to pick each point as centroid
vector<float> calculateProbabilities(vector<float> dist, int n){
    vector<float> prob(n);
    
    float sum = accumulate(dist.begin(), dist.end(), 0);

    for(int i = 0; i < n; i++){
        prob[i] = dist[i]/sum;
    }
    
    return prob;
}

// "Flip coin" for every point to become centroid
int choose(vector<float> prob, int n){
    int r = -1;

    while(r == -1){
        for(int i = 0; i < n; i++){
            if(rand()%100 >= prob[i]*100){
                r = i;
                break;
            }
        }
    }
    
    return r;
}

// K-Means++ implementation
vector<Cluster *> initializeClusters(vector<Point *> points, int k){
    vector<Cluster *> clusters;
    int n = points.size();

    // First cluster
    int first = rand()%n;
    clusters.push_back(new Cluster(points[first]));
    points.erase(points.begin()+first);
    n -= 1;
    
    // Main loop
    int t;
    while((t = clusters.size()) < k){
        vector<float> dist = D_sqr(points,clusters,n,t);
        vector<float> prob = calculateProbabilities(dist,n);
        //sort(prob.begin(), prob.end(), greater<float>());
        int r = choose(prob,n);
        
        clusters.push_back(new Cluster(points[r])); 
        points.erase(points.begin()+r);
        n -= 1;
    }
    
    return clusters;
}

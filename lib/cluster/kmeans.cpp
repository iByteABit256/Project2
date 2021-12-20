#include <iostream>
#include <limits>
#include <numeric>
#include <algorithm>
#include <random>

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
    
    float max = *max_element(dist.begin(), dist.end());
    for(vector<float>::iterator d = dist.begin(); d != dist.end(); d++){
        *d /= max;
    }

    for(int i = 0; i < n; i++){
        prob[i] = accumulate(dist.begin(), dist.begin()+i,0);
    }
    
    return prob;
}

// Choose new centroid based on probabilities
int choose(vector<float> prob, int n){
    default_random_engine generator;
    uniform_real_distribution<float> distribution(0.0,prob[n-1]);

    float x = distribution(generator);
    for(int i = 0; i < n-1; i++){
        if(prob[i] < x && prob[i+1] >= x){
            return i;
        }
    }

    return n;
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

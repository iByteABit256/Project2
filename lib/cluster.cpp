#include <iostream>

#include "cluster.h"

Cluster::Cluster(Point *centroid){
    this->centroid = centroid;
}

void Cluster::addPoint(Point *p){
   this->points.push_back(p); 
}

// Calculates new centroid
float Cluster::update(){
    vector<Point *> points = this->points;
    int d = points[0]->d;
    int T = points.size();
    vector<float> centroid(d);

    for(int i = 0; i < d; i++){
        float sum = 0;
        for(int j = 0; j < T; j++){
            sum += points[j]->pos[i];
        }
        centroid[i] = sum/T;
    }

    Point *newcentre= new Point(centroid);

    float dist = newcentre->distance(*(this->centroid));
    
    //delete this->centroid;
    this->centroid = newcentre;

    return dist;
}

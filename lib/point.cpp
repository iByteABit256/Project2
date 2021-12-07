#include <iostream>
#include <cmath>
#include <sstream>

#include "point.h"

using namespace std;

Point::Point(){
}

Point::Point(vector<float> pos, string ID){
    this->pos = pos;
    this->d = pos.size();
    this->ID = ID;
    this->sil_a = -1;
    this->sil_b = -1;
}

// Coordinates as string
string Point::to_str(){
    vector<float>::iterator it;
    ostringstream ss;

    for(it = this->pos.begin(); it != this->pos.end(); it++){
        ss << *it << " ";
    } 
    ss << endl;
    
    return ss.str();
}

// Euclidean distance
float Point::distance(Point p){
    if(this->d != p.pos.size()){
        throw runtime_error("Distance between points of different dimensions");
    }
    
    float sum = 0;
    for(int i = 0; i < this->d; i++){
        float dif = this->pos[i]-p.pos[i];
        sum += dif*dif;
    }
    
    return sqrt(sum);
}

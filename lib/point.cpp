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
float euclidean(Point a, Point p){
    if(a.d != p.pos.size()){
        throw runtime_error("Distance between points of different dimensions");
    }
    
    float sum = 0;
    for(int i = 0; i < a.d; i++){
        float dif = a.pos[i]-p.pos[i];
        sum += dif*dif;
    }
    
    return sqrt(sum);

}

// Discrete frechete distance
float frechete_discrete(Point a, Point b){
    vector<vector<float>> c(a.d, vector<float>(b.d));

    for(int i = 0; i < a.d; i++){
        for(int j = 0; j < b.d; j++){
            float ecl = abs(a.pos[i] - a.pos[j]);
            if(i == 0 && j == 0){
                c[i][j] = ecl;
            }else if(i == 0){
                c[i][j] = max(c[i][j-1], ecl);
            }else if(j == 0){
                c[i][j] = max(c[i-1][j], ecl);
            }else{
                c[i][j] = max(min(c[i-1][j], min(c[i][j-1], c[i-1][j-1])), ecl);
            }
        }
    }

    return c[a.d-1][b.d-1];
}

// Distance from another point
float Point::distance(Point p, distance_type type){
    
    if(type==EUCLIDIAN){
        return euclidean(*this,p);
    }else if(type==FRECHETE){
        return frechete_discrete(*this,p);
    }
}

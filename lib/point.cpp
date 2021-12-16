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
    vector<vector<float>> c(a.d/2, vector<float>(b.d/2));

    // Point vectors are curves where p = (x1,y1,x2,y2,...)
    for(int i = 0; i < a.d-1; i+=2){
        for(int j = 0; j < b.d-1; j+=2){
            vector<float> coords_a = { a.pos[i], a.pos[i+1] };
            vector<float> coords_b = { b.pos[i], b.pos[i+1] };
            Point a_i(coords_a);
            Point b_j(coords_b);
            float ecl = a_i.distance(b_j);

            int c_i = i/2;
            int c_j = j/2;
            if(c_i == 0 && c_j == 0){
                c[c_i][c_j] = ecl;
            }else if(c_i == 0){
                c[c_i][c_j] = max(c[c_i][c_j-1], ecl);
            }else if(c_j == 0){
                c[c_i][c_j] = max(c[c_i-1][c_j], ecl);
            }else{
                c[c_i][c_j] = max(min(c[c_i-1][c_j], min(c[c_i][c_j-1], c[c_i-1][c_j-1])), ecl);
            }
        }
    }

    return c[a.d/2-1][b.d/2-1];
}

// Distance from another point
float Point::distance(Point p, distance_type type){
    
    if(type==EUCLIDEAN){
        return euclidean(*this,p);
    }else if(type==FRECHET){
        return frechete_discrete(*this,p);
    }
    
    return -1;
}

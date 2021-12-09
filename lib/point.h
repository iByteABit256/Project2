#ifndef POINT
#define POINT

#include <vector>

using namespace std;


enum distance{ EUCLIDIAN, FRECHETE };

class Point{
public:
    string ID;
    vector<float> pos;
    int d;
    vector<int> LSH_ID;
    float sil_a;
    float sil_b;

    Point();
    Point(vector<float> pos, string ID = "None");
    string to_str();
    float distance(Point p, distance type);
};




float euclidiandistance(Point a, Point p);
float frechete_discrete(Point a, Point b);

#endif



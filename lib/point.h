#ifndef POINT
#define POINT

#include <vector>

using namespace std;


enum distance_type{ EUCLIDIAN, FRECHETE };

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
    float distance(Point p, distance_type type = EUCLIDIAN);
};

#endif
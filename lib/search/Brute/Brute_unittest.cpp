#include <gtest/gtest.h>

#include "Brute.h"


TEST(Brute, KNN) {
    vector<float> v{1,2,3};
    Point a(v);
    Point b(v);
    Point c(v);
    vector<Point *> points;
    points.push_back(&a);
    points.push_back(&b);
    vector<Point *> queries;
    queries.push_back(&c);

    float average_duration;
    vector<vector<Point *>> res = Brute_KNN(points, queries, 1, average_duration, EUCLIDEAN);
    
    EXPECT_EQ(1, res.size());
    EXPECT_EQ(1, res[0].size());
    EXPECT_GE(average_duration, 0);
}

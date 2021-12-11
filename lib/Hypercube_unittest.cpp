#include <gtest/gtest.h>

#include "Hypercube.h"


TEST(Hypercube, Initialization) {
    vector<float> v{1,2,3};
    Point a(v);
    Point b(v);
    vector<Point *> points;
    points.push_back(&a);
    points.push_back(&b);
    int k = 3;
    int d = 3;
    int probes = 1;
    int M = 10;
    struct Hypercube_Info info = Hypercube_Initialize(points, k, d, probes, M);

    EXPECT_EQ(k, info.f.size());
    EXPECT_EQ(k, info.handler.hashes.size());
    EXPECT_GT(info.hashtable.size(), 0);
}

TEST(Hypercube, KNN) {
    vector<float> v{1,2,3};
    Point a(v);
    Point b(v);
    Point c(v);
    vector<Point *> points;
    points.push_back(&a);
    points.push_back(&b);
    vector<Point *> queries;
    queries.push_back(&c);
    int k = 3;
    int d = 3;
    int probes = 1;
    int M = 10;
    struct Hypercube_Info info = Hypercube_Initialize(points, k, d, probes, M);

    float average_duration;
    vector<vector<Point *>> res = Hypercube_KNN(points, queries, info, 1, average_duration, EUCLIDEAN);
    
    EXPECT_EQ(1, res.size());
    EXPECT_EQ(1, res[0].size());
    EXPECT_GE(average_duration, 0);
}

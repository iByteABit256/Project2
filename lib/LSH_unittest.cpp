#include <gtest/gtest.h>

#include "LSH.h"


TEST(LSH, Initialization) {
    vector<float> v{1,2,3};
    Point a(v);
    Point b(v);
    vector<Point *> points;
    points.push_back(&a);
    points.push_back(&b);
    int L = 6;
    int k = 3;
    int d = 3;
    struct LSH_Info info = LSH_Initialize(points, L, k, d);
    
    EXPECT_EQ(L, info.r.size());
    EXPECT_EQ(L, info.h.size());
    EXPECT_EQ(k, info.handler.hashes.size());
    EXPECT_EQ(L, info.hashtables.size());
    EXPECT_GT(info.tableSize, 0);
}

TEST(LSH, KNN) {
    vector<float> v{1,2,3};
    Point a(v);
    Point b(v);
    Point c(v);
    vector<Point *> points;
    points.push_back(&a);
    points.push_back(&b);
    vector<Point *> queries;
    queries.push_back(&c);
    int L = 6;
    int k = 3;
    int d = 3;
    struct LSH_Info info = LSH_Initialize(points, L, k, d);

    vector<vector<Point *>> res = LSH_KNN(points, queries, info, 1, EUCLIDEAN);
    
    EXPECT_EQ(1, res.size());
    EXPECT_EQ(1, res[0].size());
}

#include <gtest/gtest.h>

#include "point.h"


float max_err = 2e-6;

TEST(Distance, Euclidean) {
    vector<float> v1{1,2,3};
    vector<float> v2{4,5,6};
    Point a(v1);
    Point b(v2);

    ASSERT_NEAR(a.distance(b,EUCLIDEAN), 5.19615242271, max_err); 
    ASSERT_NEAR(b.distance(a,EUCLIDEAN), 5.19615242271, max_err); 
}

TEST(Distance, Frechet) {
    vector<float> v1{1,2};
    vector<float> v2{4,5};
    Point a(v1);
    Point b(v2);

    ASSERT_NEAR(a.distance(b,FRECHET), 3.0, max_err); 
    ASSERT_NEAR(b.distance(a,FRECHET), 3.0, max_err); 
}

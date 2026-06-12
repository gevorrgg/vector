#include <gtest/gtest.h>
#include <Vector.h>

TEST(ComparisonTests, EqualVectors)
{
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{1, 2, 3};

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
}

TEST(ComparisonTests, DifferentSize)
{
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{1, 2, 3, 4};

    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

TEST(ComparisonTests, DifferentElements)
{
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{1, 99, 3};

    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

TEST(ComparisonTests, SelfEquality)
{
    Vector<int> v{10, 20, 30};
    EXPECT_TRUE(v == v);
    EXPECT_FALSE(v != v);
}

TEST(ComparisonTests, EmptyVectorsEqual)
{
    Vector<int> v1;
    Vector<int> v2;

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
}

TEST(ComparisonTests, EmptyAndNonEmptyNotEqual)
{
    Vector<int> v1;
    Vector<int> v2{1};

    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}
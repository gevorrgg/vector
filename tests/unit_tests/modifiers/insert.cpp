#include <gtest/gtest.h>
#include "vector.hpp"

TEST(InsertUnitTests, InsertSingleElementMiddleWithoutReallocation)
{
    Vector<int> v{1, 2, 4};

    v.reserve(v.capacity() * 2);

    auto it = v.insert(v.begin() + 2, 3);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);

    EXPECT_EQ(*it, 3);
}

TEST(InsertUnitTests, InsertSingleElementMiddleWithReallocation)
{
    Vector<int> v{1, 2, 4};

    auto it = v.insert(v.begin() + 2, 3);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);

    EXPECT_EQ(*it, 3);
}

TEST(InsertUnitTests, InsertAtBeginningWithoutReallocation)
{
    Vector<int> v{2, 3};

    v.reserve(v.capacity() * 2);

    auto it = v.insert(v.begin(), 1);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(*it, 1);
}

TEST(InsertUnitTests, InsertAtBeginningWithReallocation)
{
    Vector<int> v{2, 3};
    auto it = v.insert(v.begin(), 1);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(*it, 1);
}

TEST(InsertUnitTests, InsertAtEndWithoutReallocation)
{
    Vector<int> v{1, 2};

    v.reserve(v.capacity() * 2);

    auto it = v.insert(v.end(), 3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(it, v.begin() + 2);
    EXPECT_EQ(*it, 3);
}

TEST(InsertUnitTests, InsertAtEndWithReallocation)
{
    Vector<int> v{1, 2};
    auto it = v.insert(v.end(), 3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(it, v.begin() + 2);
    EXPECT_EQ(*it, 3);
}

TEST(InsertUnitTests, InsertMultipleCopiesWithoutReallocation)
{
    Vector<int> v{1, 4};

    v.reserve(10);

    auto it = v.insert(v.begin() + 1, 2, 2);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 4);

    EXPECT_EQ(*it, 2);
}


TEST(InsertUnitTests, InsertMultipleCopiesWithReallocation)
{
    Vector<int> v{1, 4};
    auto it = v.insert(v.begin() + 1, 2, 2);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 4);

    EXPECT_EQ(*it, 2);
}

TEST(InsertUnitTests, InsertRangeWithoutReallocation)
{
    Vector v{1, 5};
    int arr[] = {2, 3, 4};

    v.reserve(10);

    auto it = v.insert(v.begin() + 1, arr, arr + 3);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);

    EXPECT_EQ(*it, 2);
}

TEST(InsertUnitTests, InsertRangeWithReallocation)
{
    Vector v{1, 5};
    int arr[] = {2, 3, 4};
    auto it = v.insert(v.begin() + 1, arr, arr + 3);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);

    EXPECT_EQ(*it, 2);
}


TEST(InsertUnitTests, InsertIntoEmpty)
{
    Vector<int> v;
    auto it = v.insert(v.begin(), 42);

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
    EXPECT_EQ(it, v.begin());
    EXPECT_EQ(*it, 42);
}

TEST(InsertUnitTests, InsertSingleElementIntoEmpty)
{
    Vector<int> v;
    auto it = v.insert(v.begin(), 42);

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
    EXPECT_EQ(it, v.begin());
    EXPECT_EQ(*it, 42);
}

TEST(InsertUnitTests, InsertMultipleCopiesIntoEmpty)
{
    Vector<int> v;
    auto it = v.insert(v.begin(), 3, 7); // insert 3 copies of 7

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 7);
    EXPECT_EQ(v[1], 7);
    EXPECT_EQ(v[2], 7);
    EXPECT_EQ(*it, 7);
}

TEST(InsertUnitTests, InsertRangeIntoEmpty)
{
    Vector<int> v;
    int arr[] = {10, 20, 30};
    auto it = v.insert(v.begin(), arr, arr + 3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
    EXPECT_EQ(*it, 10);
}

TEST(InsertUnitTests, InsertInitializerListIntoEmpty)
{
    Vector<int> v;
    auto it = v.insert(v.begin(), {5, 6, 7});

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v[1], 6);
    EXPECT_EQ(v[2], 7);
    EXPECT_EQ(*it, 5);
}

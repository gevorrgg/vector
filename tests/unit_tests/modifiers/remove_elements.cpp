#include <gtest/gtest.h>
#include "vector.hpp"

TEST(RemoveUnitTests, EraseSingleElement)
{
    Vector<int> v{1, 2, 3, 4};
    auto it = v.erase(v.begin() + 1);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 4);

    EXPECT_EQ(*it, 3);
}

TEST(RemoveUnitTests, EraseFirstElement)
{
    Vector<int> v{10, 20, 30};
    auto it = v.erase(v.begin());

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 20);
    EXPECT_EQ(v[1], 30);
    EXPECT_EQ(*it, 20);
}

TEST(RemoveUnitTests, EraseLastElement)
{
    Vector<int> v{5, 6, 7};
    auto it = v.erase(v.end() - 1);

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v[1], 6);

    EXPECT_EQ(it, v.end());
}

TEST(RemoveUnitTests, EraseRangeMiddle)
{
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 1, v.begin() + 4);

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 5);

    EXPECT_EQ(*it, 5);
}


TEST(RemoveUnitTests, EraseEntireRange) {
    Vector<int> v{9, 8, 7};
    auto it = v.erase(v.begin(), v.end());

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(it, v.begin());
    EXPECT_EQ(it, v.end());
}

TEST(RemoveUnitTests, EraseEmptyVector) {
    Vector<int> v;
    auto it = v.erase(v.begin(), v.end());

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(it, v.begin());
    EXPECT_EQ(it, v.end());
}

TEST(RemoveUnitTests, ShrinkToFit)
{
    Vector<int> v{1, 2, 3, 4, 5};

    v.reserve(20);

    v.shrink_to_fit();

    EXPECT_EQ(v.size(), v.capacity());
}

TEST(RemoveUnitTests, Clear)
{
    Vector<int> v{1, 2, 3, 4, 5};

    v.clear();

    EXPECT_EQ(v.size(), 0);

    EXPECT_TRUE(v.empty());
}

TEST(RemoveUnitTests, PopBackRemovesLastElement)
{
    Vector<int> v{1, 2, 3};
    EXPECT_EQ(v.size(), 3);

    v.pop_back();
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), 2);

    v.pop_back();
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.back(), 1);

    v.pop_back();
    EXPECT_TRUE(v.empty());
}

TEST(RemoveUnitTests, PopBackThrowsOnEmpty)
{
    Vector<int> v;
    EXPECT_TRUE(v.empty());

    EXPECT_THROW(v.pop_back(), std::length_error);
}
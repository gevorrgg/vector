//
// Created by user on 5/29/2026.
////
// Created by user on 5/28/2026.
//
#include <gtest/gtest.h>
#include <vector.hpp>

TEST(ConstructorUnitTests, DefaultConstructor)
{
    Vector<int> v;

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_TRUE(v.empty());
    EXPECT_GE(v.capacity(), v.size());
}

TEST(ConstructorUnitTests, ConstructorWithSizeArgument)
{
    constexpr size_t size = 10;
    Vector<int> v(size);

    EXPECT_EQ(v.size(), size);
    EXPECT_FALSE(v.empty());
    EXPECT_GE(v.capacity(), v.size());

    for (size_t i = 0; i < size; i++)
    {
        EXPECT_EQ(v[i], int{});
    }
}

TEST(ConstructorUnitTests, ConstructorWithSizeAndValue)
{
    constexpr int value = 2;
    constexpr size_t size = 15;
    Vector v(size, value);

    EXPECT_EQ(v.size(), size);
    EXPECT_GE(v.capacity(), v.size());

    for (size_t i = 0; i < size; i++)
    {
        EXPECT_EQ(v[i], value);
    }
}


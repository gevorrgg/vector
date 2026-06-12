#include <gtest/gtest.h>
#include "Vector.h"

TEST(ElementAccessUnitTests, MethodSize)
{
    Vector<int> v;

    EXPECT_EQ(v.size(), 0);

    Vector<int> v2{1, 2, 3, 4, 5};

    EXPECT_EQ(v2.size(), 5);

    Vector<int> v3(6, 4);

    EXPECT_EQ(v3.size(), 6);

    Vector<int> v4(7);

    EXPECT_EQ(v4.size(), 7);
}

TEST(ElementAccessUnitTests, MehthodEmty)
{
    Vector<int> v;

    EXPECT_TRUE(v.empty());

    Vector<int> v2{1, 2, 3, 4, 5};

    EXPECT_FALSE(v2.empty());

    Vector<int> v3(6, 4);

    EXPECT_FALSE(v3.empty());
}
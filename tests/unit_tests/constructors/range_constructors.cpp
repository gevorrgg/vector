#include <gtest/gtest.h>
#include <vector.hpp>
#include <helpers/vector_eq.h>
#include <vector>
#include <iostream>
#include <unordered_map>

TEST(ConstructorUnitTests, SimpleIteratorRangeConstructor)
{
    std::vector vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> ref{vec.begin(), vec.end()};
    Vector<int> v{vec.begin(), vec.end()};

    EXPECT_EQ(v.size(), 10);
    EXPECT_VECTOR_EQ(v, ref);
}

TEST(ConstructorUnitTests, ComplicatedIteratorRangeConstructor)
{
    std::unordered_map<int, int> map{{1, 2}, {3, 4}, {6, 7}, {4, 5}};
    std::vector<std::pair<int, int>> ref{map.begin(), map.end()};
    Vector<std::pair<int, int>> v{map.begin(), map.end()};

    EXPECT_VECTOR_EQ(v, ref);
}
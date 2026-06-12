#include <gtest/gtest.h>
#include <Vector.h>
#include "iterator_types.h"


TYPED_TEST_SUITE(IteratorUnitTests, IteratorTypes);

TYPED_TEST(IteratorUnitTests, IteratorBeginCallConsistency)
{
    auto begin = Iterator<TypeParam>::begin(this->v);

    for (int i = 0; i < 10000; i++)
    {
        EXPECT_EQ(begin, Iterator<TypeParam>::begin(this->v));
    }
}
TYPED_TEST(IteratorUnitTests, IteratorEndCallConsistency)
{
    auto end = Iterator<TypeParam>::end(this->v);

    for (int i = 0; i < 10000; i++)
    {
        EXPECT_EQ(end, Iterator<TypeParam>::end(this->v));
    }
}

TYPED_TEST(IteratorUnitTests, IteratorConsistency)
{
    auto it = Iterator<TypeParam>::begin(this->v);

    auto begin = Iterator<TypeParam>::begin(this->v);
    auto end = Iterator<TypeParam>::end(this->v);

    for (size_t i = 0; i < this->v.size(); ++i, ++it)
    {
        const   size_t it_distance = end - it;
        EXPECT_EQ(it + it_distance, end);
        EXPECT_EQ(end - it_distance, it);

        EXPECT_TRUE(it <= end);
        EXPECT_TRUE(it >= begin);

        EXPECT_EQ(begin + i, it);
        EXPECT_EQ(it - begin, i);
    }

    EXPECT_EQ(Iterator<TypeParam>::begin(this->empty_vec) + 0, Iterator<TypeParam>::begin(this->empty_vec));
    EXPECT_EQ(Iterator<TypeParam>::end(this->empty_vec) - 0, Iterator<TypeParam>::end(this->empty_vec));
}


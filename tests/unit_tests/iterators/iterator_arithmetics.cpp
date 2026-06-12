#include <gtest/gtest.h>
#include <vector.hpp>
#include "iterator_types.h"

TYPED_TEST_SUITE(IteratorUnitTests, IteratorTypes);

TYPED_TEST(IteratorUnitTests, IteratorArithmetic)
{
    size_t it_distance =
        Iterator<TypeParam>::end(this->v) - Iterator<TypeParam>::begin(this->v);

    EXPECT_EQ(this->v.size(), it_distance);

    it_distance =
        Iterator<TypeParam>::end(this->v2) - Iterator<TypeParam>::begin(this->v2);

    EXPECT_EQ(this->v2.size(), it_distance);

    auto it = Iterator<TypeParam>::begin(this->v);
    ++it;

    it_distance =
        Iterator<TypeParam>::end(this->v) - it;


    EXPECT_EQ(this->v.size() - 1, it_distance);
}

TYPED_TEST(IteratorUnitTests, IteratorAddition)
{
    auto it = Iterator<TypeParam>::begin(this->v);
    auto ref_it = Iterator<TypeParam>::begin(this->ref);

    for (size_t i = 0; i < this->v.size(); ++i, ++it, ++ref_it)
    {
        EXPECT_EQ(it, Iterator<TypeParam>::begin(this->v) + i);
        EXPECT_EQ(*ref_it, *it);
        EXPECT_EQ(*ref_it, *(Iterator<TypeParam>::begin(this->v) + i));
    }
}

TYPED_TEST(IteratorUnitTests, IteratorSubtraction)
{
    auto it = Iterator<TypeParam>::end(this->v);
    auto ref_it = Iterator<TypeParam>::end(this->ref);

    for (size_t i = 0; i < this->v.size(); ++i)
    {
        --it;
        --ref_it;

        size_t idx_from_end = i + 1;

        EXPECT_EQ(it, Iterator<TypeParam>::end(this->v) - idx_from_end);
        EXPECT_EQ(*ref_it, *(Iterator<TypeParam>::end(this->v) - idx_from_end));

        EXPECT_EQ(Iterator<TypeParam>::end(this->v) - it, idx_from_end);
        EXPECT_EQ(it - Iterator<TypeParam>::begin(this->v), this->v.size() - idx_from_end);
    }
}
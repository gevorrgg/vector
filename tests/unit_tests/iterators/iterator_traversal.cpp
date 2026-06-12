#include <gtest/gtest.h>
#include <Vector.h>
#include "iterator_types.h"

TYPED_TEST_SUITE(IteratorUnitTests, IteratorTypes);

TYPED_TEST(IteratorUnitTests, BasicForwardIteration)
{
    const size_t it_distance = Iterator<TypeParam>::end(this->v) - Iterator<TypeParam>::begin(this->v);

    EXPECT_EQ(this->v.size(), it_distance);

    auto ref_it = Iterator<TypeParam>::begin(this->ref);

    for (auto it = Iterator<TypeParam>::begin(this->v); it != Iterator<TypeParam>::end(this->v); ++it, ++ref_it)
    {
        EXPECT_EQ(*it, *ref_it);
    }
}

TYPED_TEST(IteratorUnitTests, BasicBackwardIteration)
{
    auto it = Iterator<TypeParam>::end(this->v);
    --it;

    auto ref_it = Iterator<TypeParam>::end(this->ref);
    --ref_it;

    while (it != Iterator<TypeParam>::begin(this->v))
    {
        --it;
        --ref_it;

        EXPECT_EQ(*it, *ref_it);
    }

    EXPECT_EQ(*it, *ref_it);
}
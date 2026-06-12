#include <gtest/gtest.h>
#include <Vector.h>
#include "iterator_types.h"
#include <vector>

TYPED_TEST_SUITE(IteratorUnitTests, IteratorTypes);

TYPED_TEST(IteratorUnitTests, DefaultConstructorIteratorEquality)
{
    EXPECT_TRUE(Iterator<TypeParam>::begin(this->empty_vec) == Iterator<TypeParam>::begin(this->empty_vec));
}

TYPED_TEST(IteratorUnitTests, IteratorComparisonNonEmptyVector)
{
    auto middle = Iterator<TypeParam>::begin(this->non_empty) + 1;

    EXPECT_FALSE(Iterator<TypeParam>::begin(this->non_empty) == Iterator<TypeParam>::end(this->non_empty));
    EXPECT_FALSE(Iterator<TypeParam>::begin(this->non_empty) == middle);
    EXPECT_FALSE(middle == Iterator<TypeParam>::end(this->non_empty));

    EXPECT_TRUE(Iterator<TypeParam>::begin(this->non_empty) != Iterator<TypeParam>::end(this->non_empty));
    EXPECT_TRUE(Iterator<TypeParam>::begin(this->non_empty) != middle);
    EXPECT_TRUE(middle != Iterator<TypeParam>::end(this->non_empty));

    EXPECT_TRUE(Iterator<TypeParam>::begin(this->non_empty) < Iterator<TypeParam>::end(this->non_empty));
    EXPECT_TRUE(Iterator<TypeParam>::begin(this->non_empty) < middle);
    EXPECT_TRUE(middle < Iterator<TypeParam>::end(this->non_empty));

    EXPECT_TRUE(Iterator<TypeParam>::begin(this->non_empty) <= Iterator<TypeParam>::end(this->non_empty));
    EXPECT_TRUE(Iterator<TypeParam>::begin(this->non_empty) <= middle);
    EXPECT_TRUE(middle <= Iterator<TypeParam>::end(this->non_empty));

    EXPECT_FALSE(Iterator<TypeParam>::begin(this->non_empty) > Iterator<TypeParam>::end(this->non_empty));
    EXPECT_FALSE(Iterator<TypeParam>::begin(this->non_empty) > middle);
    EXPECT_FALSE(middle > Iterator<TypeParam>::end(this->non_empty));

    EXPECT_FALSE(Iterator<TypeParam>::begin(this->non_empty) >= Iterator<TypeParam>::end(this->non_empty));
    EXPECT_FALSE(Iterator<TypeParam>::begin(this->non_empty) >= middle);
    EXPECT_FALSE(middle >= Iterator<TypeParam>::end(this->non_empty));
}

TYPED_TEST(IteratorUnitTests, IteratorComparisonEmptyVector)
{
    EXPECT_FALSE(Iterator<TypeParam>::begin(this->empty_vec) != Iterator<TypeParam>::end(this->empty_vec));
    EXPECT_TRUE(Iterator<TypeParam>::begin(this->empty_vec) == Iterator<TypeParam>::end(this->empty_vec));
    EXPECT_FALSE(Iterator<TypeParam>::begin(this->empty_vec) < Iterator<TypeParam>::end(this->empty_vec));
    EXPECT_TRUE(Iterator<TypeParam>::begin(this->empty_vec) <= Iterator<TypeParam>::end(this->empty_vec));
    EXPECT_FALSE(Iterator<TypeParam>::begin(this->empty_vec) > Iterator<TypeParam>::end(this->empty_vec));
    EXPECT_TRUE(Iterator<TypeParam>::begin(this->empty_vec) >= Iterator<TypeParam>::end(this->empty_vec));
}
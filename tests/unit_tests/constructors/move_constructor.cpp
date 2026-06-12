#include <gtest/gtest.h>
#include <vector.hpp>
#include <vector>
#include <helpers/vector_eq.h>

TEST(ConstructorUnitTests, MoveConstructor)
{
    Vector v1{1, 2, 3};
    std::vector ref{1, 2, 3};
    Vector v2{std::move(v1)};

    EXPECT_VECTOR_EQ(v2, ref);

    EXPECT_EQ(v1.size(), 0);
}
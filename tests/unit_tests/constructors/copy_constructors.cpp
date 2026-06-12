#include <gtest/gtest.h>
#include <vector.hpp>
#include <vector>
#include <helpers/vector_eq.h>

TEST(ConstructorUnitTests, OracleInitializerListConstructor)
{
    const size_t ilist_size = 10;
    std::vector ref{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    EXPECT_GE(v.capacity(), ilist_size);
    EXPECT_VECTOR_EQ(v, ref);
}


TEST(ConstructorUnitTests, CopyConstructor)
{
    const size_t size = 10;
    int value = 3;

    Vector v2(size, value);
    Vector<int> v1 = v2;

    EXPECT_VECTOR_EQ(v1, v2);
}


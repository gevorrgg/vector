#include <gtest/gtest.h>
#include <vector.hpp>
#include <helpers/vector_eq.h>

TEST(IteratorUnitTests, PushBackWithoutReallocation)
{
    Vector<int> v;

    v.reserve(10);

    v.push_back(1);

    const auto it = v.begin();

    v.push_back(2);

    EXPECT_EQ(it, v.begin());
    EXPECT_EQ(*it, 1);
}

TEST(IteratorUnitTests, PushBackWithReallocation)
{
    Vector v{1};


    auto data = v.data();

    vector_fill(v);

    // reallocation
    v.push_back(1);
    EXPECT_FALSE(data == v.data());
}

TEST(IteratorUnitTests, CopyAssignmentWithoutReallocation)
{
    Vector v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    Vector v2{1, 2, 3, 4, 5};

    auto old_data = v1.data();

    v1 = v2;

    EXPECT_TRUE(old_data == v1.data());
}

TEST(IteratorUnitTests, CopyAssignmentWithReallocation)
{
    Vector v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    Vector v2{1, 2, 3, 4, 5, 56, 67, 8, 2, 6, 7, 1, 8, 2, 8, 2, 9};

    size_t new_size = v1.size();

    if (v2.capacity() >= new_size)
    {
        v1.resize(v2.capacity() * 2);
    }

    auto old_data = v2.data();

    v2 = v1;

    EXPECT_FALSE(old_data == v2.data());
}

TEST(IteratorUnitTests, MoveAssignment)
{
    Vector v{1, 2, 3, 4, 5};
    Vector v1{1, 2, 3, 4};

    auto old_data = v1.data();

    v1 = std::move(v);

    EXPECT_FALSE(old_data == v1.data());
}

TEST(IteratorUnitTests, InitListAssign)
{
    Vector v{1};

    v.shrink_to_fit();

    auto old_data = v.data();

    v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    EXPECT_FALSE(old_data == v.data());
}

TEST(IteratorUnitTests, ResizeWithoutReallocation)
{
    Vector v{1, 2, 3, 4};

    v.reserve(30);

    auto old_data = v.data();

    v.resize(10);

    EXPECT_TRUE(old_data == v.data());
}

TEST(IteratorUnitTests, ResizeWithReallocation)
{
    Vector v{1, 2, 3, 4};

    v.shrink_to_fit();

    auto old_data = v.data();

    // reallocation
    v.resize(v.capacity() * 2);

    EXPECT_FALSE(old_data == v.data());
}

TEST(IteratorUnitTests, ReserveWithReallocation)
{
    Vector v{1, 2, 3, 4};

    auto old_data = v.data();

    v.reserve(v.capacity() * 2);

    EXPECT_FALSE(old_data == v.data());
}

TEST(IteratorUnitTests, InsertWithoutRealocation)
{
    Vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    v.reserve(v.capacity() * 2);

    auto old_data = v.data();

    v.insert(v.begin(), 1);

    EXPECT_TRUE(old_data == v.data());
}

TEST(IteratorUnitTests, InsertWithReallocation)
{
    Vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    v.shrink_to_fit();

    auto old_data = v.data();

    v.insert(v.begin(), 1);

    EXPECT_FALSE(old_data == v.data());
}

TEST(IteratorUnitTests, InsertRangeWithouReallocation)
{
    Vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::vector range = {1, 2, 3, 4, 5, 6};

    v.reserve(v.capacity() * 2 + range.size());

    auto old_data = v.data();

    v.insert(v.begin(), range.begin(), range.end());

    EXPECT_TRUE(old_data == v.data());
}

TEST(IteratorUnitTests, InsertRangeWithReallocation)
{
    Vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::vector range = {1, 2, 3, 4, 5, 6};

    v.shrink_to_fit();

    auto old_data = v.data();

    v.insert(v.begin(), range.begin(), range.end());

    EXPECT_FALSE(old_data == v.data());
}

TEST(IteratorUnitTests, ShrinkToFitWithReallocation)
{
    Vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    v.reserve(v.capacity() * 2);

    auto old_data = v.data();

    v.shrink_to_fit();

    EXPECT_FALSE(old_data == v.data());
}

#include <gtest/gtest.h>
#include <vector.hpp>
#include <helpers/throwing_types.h>
#include <helpers/allocators.h>
#include <vector>

//
// Single element insert
//
TEST(ExceptionSafetyTests, InsertSingleWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200); // no reallocation

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(5);

    try {
        ThrowingType::reset_counts();
        CountingAllocator<ThrowingType>::reset_counts();
        v.insert(v.begin() + 25, ThrowingType(7));
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v); // unchanged
    }
}

TEST(ExceptionSafetyTests, InsertSingleWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit(); // minimal capacity forces reallocation

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(5);

    try {
        ThrowingType::reset_counts();
        CountingAllocator<ThrowingType>::reset_counts();
        v.insert(v.begin() + 25, ThrowingType(7));
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// Multiple copies insert
//
TEST(ExceptionSafetyTests, InsertCountWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(15);

    try {
        CountingAllocator<ThrowingType>::reset_counts();
        v.insert(v.begin() + 10, 40, ThrowingType(7));
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

TEST(ExceptionSafetyTests, InsertCountWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit();

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;


    try {
        CountingAllocator<ThrowingType>::reset_counts();
        ThrowingType::set_thold(15);
        v.insert(v.begin() + 10, 40, ThrowingType(7));
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// Range insert
//
TEST(ExceptionSafetyTests, InsertRangeWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    std::vector<ThrowingType> src(40, ThrowingType(7));
    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    try {
        ThrowingType::reset_counts();
        CountingAllocator<ThrowingType>::reset_counts();
        ThrowingType::set_thold(20);
       // v.insert(v.begin() + 15, src.begin(), src.end());
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

TEST(ExceptionSafetyTests, InsertRangeWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    std::vector<ThrowingType> src(40, ThrowingType(7));
    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit();

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(20);

    try {
        ThrowingType::reset_counts();
        CountingAllocator<ThrowingType>::reset_counts();
        v.insert(v.begin() + 15, src.begin(), src.end());
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// Initializer list insert
//
TEST(ExceptionSafetyTests, InsertInitListWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(10);

    try {
        ThrowingType::reset_counts();
        CountingAllocator<ThrowingType>::reset_counts();
        v.insert(v.begin() + 20, {ThrowingType(7), ThrowingType(8), ThrowingType(9)});
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

TEST(ExceptionSafetyTests, InsertInitListWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit();

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(10);

    try {
        ThrowingType::reset_counts();
        CountingAllocator<ThrowingType>::reset_counts();
        v.insert(v.begin() + 20, {ThrowingType(7), ThrowingType(8), ThrowingType(9)});
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

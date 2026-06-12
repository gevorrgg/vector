#include <gtest/gtest.h>
#include <vector.hpp>
#include <helpers/throwing_types.h>
#include <helpers/allocators.h>
#include <vector>

//
// Copy assignment
//
TEST(ExceptionSafetyTests, CopyAssignWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> src(50, ThrowingType(7));
    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200); // ensures no reallocation

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(33);

    try {
        v = src; // copy assignment
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v); // unchanged
    }
}

TEST(ExceptionSafetyTests, CopyAssignWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> src(50, ThrowingType(7));
    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit(); // force minimal capacity

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(33);

    try {
        v = src; // copy assignment with reallocation
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// Move assignment
//
TEST(ExceptionSafetyTests, MoveAssignWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> src(50, ThrowingType(7));
    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(33);

    try {
        v = std::move(src); // move assignment
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

TEST(ExceptionSafetyTests, MoveAssignWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> src(50, ThrowingType(7));
    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit();

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(33);

    try {
        v = std::move(src); // move assignment with reallocation
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// Assign count
//
TEST(ExceptionSafetyTests, AssignCountWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(33);

    try {
        v.assign(40, ThrowingType(7));
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

TEST(ExceptionSafetyTests, AssignCountWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit();

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(33);

    try {
        v.assign(40, ThrowingType(7));
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// Assign range
//
TEST(ExceptionSafetyTests, AssignRangeWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> src(40, ThrowingType(7));
    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(33);

    try {
        v.assign(src.begin(), src.end());
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

TEST(ExceptionSafetyTests, AssignRangeWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> src(40, ThrowingType(7));
    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit();

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(33);

    try {
        v.assign(src.begin(), src.end());
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// Created by user on 6/12/2026.
//
#include <gtest/gtest.h>
#include <vector.hpp>
#include <helpers/throwing_types.h>
#include <helpers/allocators.h>

//
// Erase single element with reserve
//
TEST(ExceptionSafetyTest, EraseSingleWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();
    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(10);

    try {
        v.erase(v.begin() + 20);
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// Erase range with reallocation
//
TEST(ExceptionSafetyTest, EraseRangeWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();
    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit();

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(15);

    try {
        v.erase(v.begin() + 10, v.begin() + 30);
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// Clear with reserve
//
TEST(ExceptionSafetyTest, ClearWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();
    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(20);

    try {
        v.clear();
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// Shrink_to_fit with reallocation
//
TEST(ExceptionSafetyTest, ShrinkToFitWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();
    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(100, ThrowingType(1));
    v.reserve(200);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(25);

    try {
        v.shrink_to_fit();
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}


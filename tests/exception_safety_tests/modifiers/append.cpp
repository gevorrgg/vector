//
// Created by user on 6/12/2026.
//
#include <gtest/gtest.h>
#include <Vector.h>
#include <helpers/throwing_types.h>
#include <helpers/allocators.h>

//
// push_back with reserve
//
TEST(ExceptionSafetyTests, PushBackWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200); // no reallocation

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(10);

    try {
        v.push_back(ThrowingType(7));
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v); // unchanged
    }
}

//
// push_back with reallocation
//
TEST(ExceptionSafetyTests, PushBackWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit(); // minimal capacity forces reallocation

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(10);

    try {
        v.push_back(ThrowingType(7));
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// emplace_back with reserve
//
TEST(ExceptionSafetyTests, EmplaceBackWithReserveNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.reserve(200);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(10);

    try {
        v.emplace_back(7); // construct in place
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}

//
// emplace_back with reallocation
//
TEST(ExceptionSafetyTests, EmplaceBackWithReallocationNoLeaksOnException)
{
    ThrowingType::reset_counts();
    CountingAllocator<ThrowingType>::reset_counts();

    ThrowingType::set_thold(1000);

    Vector<ThrowingType, CountingAllocator<ThrowingType>> v(50, ThrowingType(1));
    v.shrink_to_fit();

    Vector<ThrowingType, CountingAllocator<ThrowingType>> old_vector = v;

    ThrowingType::set_thold(10);

    try {
        v.emplace_back(7); // construct in place
    } catch (std::runtime_error&) {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_TRUE(old_vector == v);
    }
}


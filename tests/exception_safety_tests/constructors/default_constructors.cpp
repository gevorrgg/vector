#include <gtest/gtest.h>
#include <vector.hpp>
#include <helpers/throwing_types.h>
#include <helpers/allocators.h>
#include <vector>

TEST(ExceptionSafetyTests, SizeConstructorNoLeaksOnException)
{
    ThrowingType::copy_count = 0;
    CountingAllocator<ThrowingType>::alloc_count = 0;
    CountingAllocator<ThrowingType>::dealloc_count = 0;
    ThrowingType::live_count = 0;

    try
    {
        Vector<ThrowingType, CountingAllocator<ThrowingType>>(40);
    }
    catch (std::runtime_error&)
    {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
              CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_EQ(ThrowingType::live_count, 0);
    }
}

TEST(ExceptionSafetyTests, SizeValueConstructorNoLeaksOnException)
{
    ThrowingType::copy_count = 0;
    ThrowingType::live_count = 0;
    CountingAllocator<ThrowingType>::alloc_count = 0;
    CountingAllocator<ThrowingType>::dealloc_count = 0;


    try
    {
        Vector<ThrowingType, CountingAllocator<ThrowingType>>(40, ThrowingType(7));
    }
    catch (std::runtime_error&)
    {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
              CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_EQ(ThrowingType::live_count, 0);
    }
}

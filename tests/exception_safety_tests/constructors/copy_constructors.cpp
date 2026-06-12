#include <gtest/gtest.h>
#include <vector.hpp>
#include <helpers/throwing_types.h>
#include <helpers/allocators.h>
#include <vector>

TEST(ExceptionSafetyTests, CopyConstructorNoLeaksOnException)
{
    ThrowingType::copy_count = 0;
    ThrowingType::live_count = 0;
    CountingAllocator<ThrowingType>::alloc_count = 0;
    CountingAllocator<ThrowingType>::dealloc_count = 0;

    try
    {

        Vector<ThrowingType, CountingAllocator<ThrowingType>> src(20, ThrowingType(7));

        Vector<ThrowingType, CountingAllocator<ThrowingType>> copy(src);
    }
    catch (std::runtime_error&)
    {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_EQ(ThrowingType::live_count, 0);
    }
}

TEST(ExceptionSafetyTests, InitListConstructorNoLeaksOnException)
{
    ThrowingType::copy_count = 0;
    ThrowingType::live_count = 0;
    CountingAllocator<ThrowingType>::alloc_count = 0;
    CountingAllocator<ThrowingType>::dealloc_count = 0;

    try
    {
        Vector<ThrowingType, CountingAllocator<ThrowingType>> v{
            ThrowingType(1), ThrowingType(2), ThrowingType(3), ThrowingType(4),
            ThrowingType(5), ThrowingType(6), ThrowingType(7), ThrowingType(8),
            ThrowingType(9), ThrowingType(10), ThrowingType(11), ThrowingType(12),
            ThrowingType(13), ThrowingType(14), ThrowingType(15), ThrowingType(16),
            ThrowingType(17), ThrowingType(18), ThrowingType(19), ThrowingType(20)
        };
    }
    catch (std::runtime_error&)
    {
        EXPECT_EQ(CountingAllocator<ThrowingType>::alloc_count,
                  CountingAllocator<ThrowingType>::dealloc_count);
        EXPECT_EQ(ThrowingType::live_count, 0);
    }
}

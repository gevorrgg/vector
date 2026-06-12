//
// Created by user on 6/2/2026.
//

#ifndef VECTOR_TESTS_ALLOCATORS_H
#define VECTOR_TESTS_ALLOCATORS_H

template <typename T>
struct CountingAllocator {
    using value_type = T;
    static int alloc_count;
    static int dealloc_count;

    CountingAllocator() noexcept = default;
    template <class U>
    explicit CountingAllocator(const CountingAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        ++alloc_count;
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        ++dealloc_count;
        std::allocator<T>{}.deallocate(p, n);
    }

    static void reset_counts() noexcept
    {
        alloc_count = 0;
        dealloc_count = 0;
    }
};

template <typename T> int CountingAllocator<T>::alloc_count = 0;
template <typename T> int CountingAllocator<T>::dealloc_count = 0;

#endif //VECTOR_TESTS_ALLOCATORS_H

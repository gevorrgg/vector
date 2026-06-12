//
// Created by user on 5/22/2026.
//

#ifndef VECTOR_VECTOR_MEMORY_H
#define VECTOR_VECTOR_MEMORY_H

#include <memory>


template <typename T, typename Allocator>
class Pointer
{
public:
    Pointer() = delete;
    Pointer(const Pointer& other) = delete;
    Pointer& operator=(const Pointer& other) = delete;
    Pointer(Allocator& allocator, size_t n)
        : m_allocator(&allocator),
          m_ptr(nullptr),
          n(n)
    {
        if (n)
        {
            m_ptr = std::allocator_traits<Allocator>::allocate(*m_allocator, n);
        }
    }

    Pointer(Pointer&& other) noexcept
        : m_allocator(other.m_allocator),
          m_ptr(other.m_ptr),
          n(other.n)
    {
        other.m_ptr = nullptr;
        other.n = 0;
    }

    T* release()
    {
        T* ptr = m_ptr;
        m_ptr = nullptr;

        n = 0;

        return ptr;
    }

    [[nodiscard]]
    T* data()
    {
        return m_ptr;
    }

    ~Pointer()
    {
        if (!m_ptr)
            return;

        std::allocator_traits<Allocator>::deallocate(*m_allocator, m_ptr, n);
    }

private:
    Allocator *m_allocator;
    T* m_ptr;
    size_t n;
};

template <typename Allocator, typename It>
void destroy_range(Allocator& m_allocator, It first, It last)
{
    for (auto it = first; it != last; ++it)
    {
        std::allocator_traits<Allocator>::destroy(m_allocator, it);
    }
}


template <typename T, typename Allocator>
Pointer<T, Allocator> make_pointer(Allocator& allocator, size_t n)
{
    Pointer<T, Allocator> ptr(allocator, n);

    return ptr;
}

template <typename T, typename Allocator>
[[nodiscard]]
T* reallocate(Allocator& allocator, T* buf, size_t n, size_t old_capacity, size_t new_capacity)
{
    Pointer tmp = make_pointer<T, Allocator>(allocator, new_capacity);
    constexpr bool can_move = std::is_nothrow_move_constructible_v<T>;

    if constexpr (can_move)
    {
        std::uninitialized_move(buf, buf + n, tmp.data());
    }
    else
    {
        std::uninitialized_copy(buf, buf + n, tmp.data());
    }

    if constexpr (!std::is_trivially_destructible_v<T>)
    {
        destroy_range(allocator, buf, buf + n);
    }

    std::allocator_traits<Allocator>::deallocate(allocator, buf, old_capacity);

    return tmp.release();
}

template <typename T, typename Allocator>
void destroy_and_deallocate(Allocator& allocator, T* buf, size_t constructed, size_t capacity)
{
    if (!buf) return;

    if constexpr(!std::is_trivially_destructible_v<T>)
    {
        destroy_range(allocator, buf, buf + constructed);
    }

    std::allocator_traits<Allocator>::deallocate(allocator, buf, capacity);
}

template <typename T, typename It>
void uninitialized_move_if_noexcept(It begin, It end, T* dest)
{
    if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
    {
        std::uninitialized_move(begin, end, dest);
    }
    else
    {
        std::uninitialized_copy(begin, end, dest);
    }
}

template <typename T, typename It>
void move_backward_if_noexcept(It first, It last, T* dest)
{
    if constexpr(std::is_nothrow_move_assignable_v<T> || !std::is_copy_assignable_v<T>)
    {
        std::move_backward(first, last, dest);
    }
    else
    {
        std::copy_backward(first, last, dest);
    }
}

#endif //VECTOR_VECTOR_MEMORY_H
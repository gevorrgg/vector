//
// Created by user on 11/21/2025.
//

#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <algorithm>
#include <stdexcept>
#include <memory>
#include <iterator>
#include "vector_memory.h"
#include <cassert>

template<
    typename T,
    typename Allocator = std::allocator<T>
>
class Vector
{
public:

    // available iterators
    using iterator                 =  T*;
    using const_iterator           =  const T*;
    using reverse_iterator         =  std::reverse_iterator<iterator>;
    using const_reverse_iterator   =  std::reverse_iterator<const_iterator>;

    /**
    * @brief Constructors
    *
    * @par Exception Safety
    * - **Strong guarantee**
    */
    explicit Vector(const Allocator& allocator = Allocator()) noexcept(
        std::is_nothrow_copy_constructible_v<Allocator>) :
        m_size(0),
        m_capacity(0),
        m_data(nullptr),
        m_allocator(allocator)
    {}

    template<std::input_iterator It>
    Vector(It begin, It end, const Allocator& allocator = Allocator())
	requires std::constructible_from<T, std::iter_reference_t<It>>
        : Vector(allocator)
    {
        if constexpr(std::forward_iterator<It>)
        {
            forward_iter_ctor_impl(begin, end);
        }
        else
        {
            input_iter_ctor_impl(begin, end);
        }
    }

	Vector(size_t size, const Allocator& allocator = Allocator())
		: Vector(std::allocator_traits<Allocator>::select_on_container_copy_construction(allocator))
	{
		Pointer tmp = make_pointer<T, Allocator>(m_allocator, size);

        std::uninitialized_value_construct_n(tmp.data(), size);

        m_size       = size;
        m_capacity   = m_size;
        m_data       = tmp.release();
	}


    Vector(size_t size, const T& value, const Allocator& allocator = Allocator())
        : Vector(std::allocator_traits<Allocator>::select_on_container_copy_construction(allocator))
    {
        Pointer tmp = make_pointer<T, Allocator>(m_allocator, size);

        std::uninitialized_fill_n(tmp.data(), size, value);

        m_size       = size;
        m_capacity   = m_size;
        m_data       = tmp.release();
    }

    Vector(const Vector& other)
    : Vector(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.m_allocator))
    {
        Pointer tmp  = make_pointer<T, Allocator>(m_allocator, other.m_size);

        std::uninitialized_copy(other.begin(), other.end(), tmp.data());

        m_data        = tmp.release();
        m_size        = other.m_size;
        m_capacity    = other.m_size;
    }

    Vector(Vector&& other) noexcept(
         std::allocator_traits<Allocator>::is_always_equal::value ||
         std::is_nothrow_move_constructible_v<Allocator>)
         : m_size(other.m_size),
           m_capacity(other.m_capacity),
           m_data(other.m_data),
           m_allocator(std::move(other.m_allocator))
    {
        other.m_size = 0;
        other.m_capacity = 0;
        other.m_data = nullptr;
    }

    Vector(std::initializer_list<T> other, const Allocator& allocator = Allocator())
    requires std::constructible_from<T, const T&>
        : Vector(allocator)
    {
        const size_t n = other.size();

        Pointer tmp    = make_pointer<T, Allocator>(m_allocator, n);

        std::uninitialized_copy(other.begin(), other.end(), tmp.data());

        m_data         = tmp.release();
        m_size         = n;
        m_capacity     = n;
    }

    ~Vector()
    {
        destroy_and_deallocate(m_allocator, m_data, m_size, m_capacity);
    }

    /**
        * @brief assignment operators.
        *
        * @par Exception Safety
        * - **Strong guarantee**
        */

    Vector& operator=(const Vector& other)
    requires std::constructible_from<T, const T&>
    {
        if (this ==  &other) return *this;

        auto *new_allocator = &m_allocator;

        bool can_throw = !std::is_nothrow_copy_assignable_v<T> ||
            (other.m_size > m_size && !std::is_nothrow_copy_constructible_v<T>);

        if constexpr (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value)
        {
            new_allocator = &other.m_allocator;
        }

        if (other.m_size > m_capacity || can_throw)
        {
            Pointer tmp = make_pointer<T, Allocator>(*new_allocator, other.m_size);

            std::uninitialized_copy(other.begin(), other.end(), tmp.data());

            Allocator old_allocator = m_allocator;
            T* old_data         = m_data;
            size_t old_size     = m_size;
            size_t old_capacity = m_capacity;

            if constexpr (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value)
            {
                m_allocator = *new_allocator;
            }

            m_size        = other.m_size;
            m_capacity    = other.m_size;
            m_data        = tmp.release();

            destroy_and_deallocate(old_allocator, old_data, old_size, old_capacity);
        }
        else
        {
            size_t overlap_gap = std::min(m_size, other.m_size);

            if constexpr(std::is_copy_assignable_v<T>)
            {
                std::copy(other.m_data, other.m_data + overlap_gap, m_data);
            }
            else
            {
                if constexpr(!std::is_copy_assignable_v<T>)
                {
                    destroy_range(*new_allocator, m_data, m_data + overlap_gap);
                }

                std::uninitialized_copy(other.m_data, other.m_data + overlap_gap, m_data);
            }

            if (m_size < other.m_size)
            {
                std::uninitialized_copy(other.m_data + overlap_gap,
                                        other.m_data + other.m_size,
                                        m_data + overlap_gap);
            }

            if (m_size > other.m_size)
            {
                // destroy odd elements
                if constexpr(!std::is_trivially_destructible_v<T>)
                {
                    destroy_range(m_allocator, m_data + other.m_size, m_data + m_size);
                }
            }

            if constexpr (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value)
            {
                m_allocator = *new_allocator;
            }

            m_size        = other.m_size;
        }

        return *this;
    }

    Vector& operator=(Vector&& other) noexcept(
           (POCMA::value || std::allocator_traits<Allocator>::is_always_equal::value)
       )
    {
        if (this == &other) return *this;

        constexpr bool always_equal = std::allocator_traits<Allocator>::is_always_equal::value;
        constexpr bool propagate    = POCMA::value;

        if constexpr (always_equal || propagate)
        {
            steal_from(std::move(other));
        }
        else
        {
			if (m_allocator == other.m_allocator)
			{
				steal_from(std::move(*other));
			}

			move_from(std::move(other));
        }

        return *this;
    }

    Vector& operator=(std::initializer_list<T> other)
    {
        auto tmp = Vector(other, m_allocator);

        *this = std::move(tmp);

        return *this;
    }

    [[nodiscard]]
    size_t max_size() const noexcept
    {
        return std::allocator_traits<Allocator>::max_size(m_allocator);
    }

    /**
       * @brief increases capacity of vector to the new_capacity.
       * Values between [m_size, m_capacity) remain uninitialized
       *
       * @par Exception Safety
       * - **Strong guarantee**
       */


    void reserve(size_t new_capacity)
    {
        const size_t max_size = std::allocator_traits<Allocator>::max_size(m_allocator);

        if (new_capacity > max_size)
        {
            throw std::length_error("Vector::reserve(size_t new_capacity): vector is full");
        }

        if (new_capacity <= m_capacity) return;

        T* tmp = reallocate(m_allocator, m_data, m_size, m_capacity, new_capacity);

        m_capacity = new_capacity;
        m_data = tmp;
    }

    /**
     * @brief increases size and capacity of vector to the new_size.
     * Fills unconstructed elements either with value or with default constructed object
     *
     * @par Exception Safety
     * - **Strong guarantee**
     */


    void resize(size_t new_size, const T& value)
    requires std::constructible_from<T, const T&>
    {
        if (m_size == new_size) return;

        if (new_size > m_capacity) // growth
        {
            T tmp_value{value};
            Pointer tmp = make_pointer<T, Allocator>(m_allocator, new_size);

            uninitialized_move_if_noexcept(m_data, m_data + m_size, tmp.data());

            try
            {
                std::uninitialized_fill_n(tmp.data() + m_size, new_size - m_size, std::move_if_noexcept(tmp_value));
            }
            catch (...)
            {
                destroy_range(m_allocator, tmp.data(), tmp.data() + m_size);

                throw;
            }

            T* old_data         = m_data;
            size_t old_size     = m_size;
            size_t old_capacity = m_capacity;

            m_capacity  = new_size;
            m_data      = tmp.release();

            destroy_and_deallocate(m_allocator, old_data, old_size, old_capacity);
        }
        else if (new_size < m_size) // shrink
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
                destroy_range(m_allocator, m_data + new_size, m_data + m_size);
        }
        else // inplace fill
        {
            std::uninitialized_fill_n(m_data + m_size, new_size - m_size, value);
        }

        m_size = new_size;
    }

    void resize(size_t new_size)
    {
        if (m_size == new_size) return;

        if (new_size > m_capacity) // growth
        {
            Pointer tmp = make_pointer<T, Allocator>(m_allocator, new_size);

            uninitialized_move_if_noexcept(m_data, m_data + m_size, tmp.data());

            try
            {
                std::uninitialized_value_construct_n(tmp.data() + m_size, new_size - m_size);
            }
            catch (...)
            {
                destroy_range(m_allocator, tmp.data(), tmp.data() + m_size);

                throw;
            }

            T* old_data         = m_data;
            size_t old_size     = m_size;
            size_t old_capacity = m_capacity;

            m_capacity  = new_size;
            m_data      = tmp.release();

            destroy_and_deallocate(m_allocator, old_data, old_size, old_capacity);
        }
        else if (new_size < m_size) // shrink
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
                destroy_range(m_allocator, m_data + new_size, m_data + m_size);
        }
        else // inplace fill
        {
            std::uninitialized_value_construct_n(m_data + m_size, new_size - m_size);
        }

        m_size = new_size;
    }

    /**
     * @brief Append element to the end.
     *
     * @par Exception Safety
     * - **Strong guarantee**
     */

    void push_back(const T& value)
    {
        emplace_back(value);
    }

    void push_back(T&& value)
    {
        emplace_back(std::move(value));
    }

    template <typename ... Args>
    void emplace_back(Args&& ... args)
    {
        if (m_size == m_capacity)
        {
            size_t new_capacity = growth_policy();
            T tmp_value{std::forward<Args>(args)...};

            T* tmp = reallocate(m_allocator, m_data, m_size, m_capacity, new_capacity);

            try
            {
                std::allocator_traits<Allocator>::construct(m_allocator, tmp + m_size, std::move_if_noexcept(tmp_value));
            }
            catch (...)
            {
                destroy_and_deallocate(m_allocator, tmp, m_size, new_capacity);

                throw;
            }

            m_data     = tmp;
            m_capacity = new_capacity;
        }
        else
        {
            std::allocator_traits<Allocator>::construct(m_allocator, m_data + m_size, std::forward<Args>(args)...);
        }

        m_size++;
    }

    /**
    * @brief Assign values or range to the container overwriting existing objects.
    *
    * @par Exception Safety
    * - **Strong guarantee**
    */

    void assign( size_t count, const T& value)
    {
        const bool can_throw = (count > m_size)
            ? (!std::is_nothrow_copy_assignable_v<T> || !std::is_nothrow_copy_constructible_v<T>)
            : (!std::is_nothrow_copy_assignable_v<T>);

        if (count > m_capacity || can_throw)
        {
            Pointer tmp = make_pointer<T, Allocator>(m_allocator, count);

            std::uninitialized_fill_n(tmp.data(), count, value);

            auto old_data    = m_data;
            size_t old_size     = m_size;
            size_t old_capacity = m_capacity;

            m_data      = tmp.release();
            m_capacity  = count;
            m_size      = count;

            destroy_and_deallocate(m_allocator, old_data, old_size, old_capacity);
        }
        else
        {
            T tmp{value};

            if (count > m_size)
            {
                std::fill_n(m_data, m_size, tmp);
                std::uninitialized_fill_n(m_data + m_size, count - m_size, tmp);
            }
            else
            {
                std::fill_n(m_data, count, tmp);

                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    destroy_range(m_allocator, m_data + count, m_data + m_size);
                }
            }

            m_size = count;
        }
    }

    template < typename InputIt >
    void assign( InputIt first, InputIt last )
    requires (!std::integral<InputIt>)
    {
        if constexpr (std::forward_iterator<InputIt>)
        {
            assign_range_forward_iter(first, last);
        }
        else // input iterator
        {
            assign_range_input_iter(first, last);
        }
    }

    void assign(std::initializer_list<T> ilist)
    {
        assign(ilist.begin(), ilist.end());
    }

    /**
     * @brief Inserts an element or range at the specified position.
     *
     * @par Exception Safety
     * - **Strong guarantee** if:
     *   - the allocator does not throw on construction
     *   - move does not throw on construction and assignment
     *   - copy does not throw on construction and assignment
     * - **Basic guarantee** otherwise
     *   - vector remains valid but insert may not complete
     *
     * **Note**
     *  - does not guarantee self overlapping correctness
     * @pre `pos` must be a valid iterator in `[begin(), end()]`
     */

    iterator insert(const_iterator pos, T&& value)
    requires std::is_move_assignable_v<T> && std::is_move_constructible_v<T>
    {
        assert(begin() <= pos && pos <= end());

        return insert_impl(pos, 1, std::move(value));
    }

	iterator insert(const_iterator pos, size_t count, const T& value)
	{
		assert(begin() <= pos && pos <= end());

		return insert_impl(pos, count, value);
	}

    iterator insert(const_iterator pos, const T& value)
    requires std::is_copy_assignable_v<T> && std::constructible_from<T, const T&>
    {
        assert(begin() <= pos && pos <= end());

        return insert_impl(pos, 1, value);
    }

    template <typename It>
    iterator insert(const_iterator pos, It first, It last)
    requires std::constructible_from<T, std::iter_reference_t<It>> &&
             (std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>)
    {
        assert(begin() <= pos && pos <= end());

        if constexpr(std::forward_iterator<It>)
        {
            return insert_forward_it(pos, first, last);
        }

        return insert_input_it(pos, first, last);
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist)
    requires std::constructible_from<T, const T&> && std::is_copy_assignable_v<T>
    {
        assert(begin() <= pos && pos <= end());

        return insert_forward_it(pos, ilist.begin(), ilist.end());
    }

    /**
     * @brief Erases an element or range at the specified position.
     *
     * @par Exception Safety
     * - **Strong guarantee** if:
     *   - move does not throw on assignment or
     *   - copy does not throw on assignment
     * - **Basic guarantee** otherwise
     *   - vector remains valid but insert may not complete
     *
     * @pre `pos` must be a valid iterator in `[begin(), end()]`
     */

    iterator erase(const_iterator pos) noexcept(
        std::is_nothrow_move_assignable_v<T> ||
        std::is_nothrow_copy_assignable_v<T>)
    requires std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>
    {
        assert(begin() <= pos && pos < end());

        size_t offset = pos - cbegin();

        if constexpr(std::is_nothrow_move_assignable_v<T> ||
                     !std::is_copy_assignable_v<T>)
        {
            std::move(m_data + offset + 1, m_data + m_size, m_data + offset);
        }
        else
        {
            std::copy(m_data + offset + 1, m_data + m_size, m_data + offset);
        }


        std::allocator_traits<Allocator>::destroy(m_allocator, m_data + m_size - 1);

        m_size--;

        return iterator(m_data + offset);
    }

    iterator erase(const_iterator first, const_iterator last) noexcept(
        std::is_nothrow_move_assignable_v<T> ||
        std::is_nothrow_copy_assignable_v<T>)
    requires std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>
    {
        assert(first <= last);
        assert(begin() <= first && first <= end());
        assert(begin() <= last && last <= end());

        const size_t first_index = first - cbegin();
        const size_t last_index  = last - cbegin();
        const size_t n           = last_index - first_index;

        if constexpr(std::is_nothrow_move_assignable_v<T> || !std::is_copy_assignable_v<T>)
        {
            std::move(m_data + last_index, m_data + m_size, m_data + first_index);
        }
        else
        {
            std::copy(m_data + last_index, m_data + m_size, m_data + first_index);
        }

        destroy_range(m_allocator,
                     m_data + m_size - n,
                     m_data + m_size);

        m_size -= n;

        return iterator(m_data + first_index);
    }

    void shrink_to_fit()
    {
        if (m_capacity == m_size) return;

        if (m_size == 0 && m_data != nullptr)
        {
            std::allocator_traits<Allocator>::deallocate(m_allocator, m_data, m_capacity);
            m_data = nullptr;
            m_capacity = 0;

            return;
        }

        T *tmp_data = reallocate(m_allocator, m_data, m_size, m_capacity, m_size);

        m_capacity = m_size;
        m_data     = tmp_data;
    }

    void pop_back()
    {
        if (m_size == 0)
        {
            throw std::length_error("Vector::pop_back() : vector is empty");
        }

        if constexpr(!std::is_trivially_destructible_v<T>)
            std::allocator_traits<Allocator>::destroy(m_allocator, m_data + m_size - 1);

        m_size--;
    }

    bool operator==(const Vector& other) const
    {
        if (this == &other) return true;

        if (m_size != other.m_size) return false;

        for (size_t i = 0; i < m_size; i++)
        {
            if (m_data[i] != other.m_data[i]) return false;
        }

        return true;
    }

    bool operator!=(const Vector& other) const
    {
        return !(*this == other);
    }

    T&       operator[](size_t index)        noexcept { assert(index < m_size); return m_data[index]; }
    const T& operator[] (size_t index) const noexcept { assert(index < m_size); return m_data[index]; }

    T& at(size_t index)
    {
        if (index >= m_size)
        {
            throw std::out_of_range("Vector::at(size_t index): parameter 'index' is out of bounds");
        }

        return m_data[index];
    }

    const T& at(size_t index) const
    {
        if (index >= m_size)
        {
            throw std::out_of_range("Vector::at(size_t index) const: parameter 'index' is out of bounds");
        }

        return m_data[index];
    }

    void clear() noexcept
    {
        if constexpr(!std::is_trivially_destructible_v<T>)
            destroy_range(m_allocator, m_data, m_data + m_size);

        m_size = 0;
    }

    [[nodiscard]]
    T&       front ()          { assert(m_size); return *m_data; }
    [[nodiscard]]
    const T& front () const    { assert(m_size); return *m_data; }

    [[nodiscard]]
    T&       back ()           { assert(m_size); return *(m_data + m_size - 1);}
    [[nodiscard]]
    const T& back () const     { assert(m_size); return *(m_data + m_size - 1);}

    [[nodiscard]]
    bool empty() const noexcept { return m_size == 0;}

    // getter functions
    [[nodiscard]] Allocator get_allocator() const noexcept { return m_allocator; }

    [[nodiscard]] const T* data () const noexcept { return m_data; }
    [[nodiscard]]       T* data ()       noexcept { return m_data; }

    [[nodiscard]] size_t size     () const noexcept { return m_size;     }
    [[nodiscard]] size_t capacity () const noexcept { return m_capacity; }

    iterator               begin   ()       noexcept  { return iterator(m_data);                    }
    iterator               end     ()       noexcept  { return iterator(m_data + m_size);           }

    const_iterator         begin   () const noexcept  { return const_iterator(m_data);              }
    const_iterator         end     () const noexcept  { return const_iterator(m_data + m_size);     }

    const_iterator         cbegin  () const noexcept  { return const_iterator(m_data);              }
    const_iterator         cend    () const noexcept  { return const_iterator(m_data + m_size);     }

    reverse_iterator       rbegin  () noexcept        { return reverse_iterator(end());           }
    reverse_iterator       rend    () noexcept        { return reverse_iterator(begin());         }

    const_reverse_iterator rbegin  () const noexcept  { return const_reverse_iterator(end());     }
    const_reverse_iterator rend    () const noexcept  { return const_reverse_iterator(begin());   }

    const_reverse_iterator crbegin () const noexcept  { return const_reverse_iterator(cend());    }
    const_reverse_iterator crend   () const noexcept  { return const_reverse_iterator(cbegin());  }

private:
    size_t    m_size;
    size_t    m_capacity;
    T*        m_data;
    Allocator m_allocator;

    using POCMA = std::allocator_traits<Allocator>::propagate_on_container_move_assignment;


    void move_from(Vector&& other)
    requires std::constructible_from<T, T&&>
    {
        constexpr bool can_throw = (std::is_move_assignable_v<T> && !std::is_nothrow_move_assignable_v<T>) ||
                          !std::is_nothrow_move_constructible_v<T> ;

        if (other.m_size > m_capacity || can_throw)
        {
            Pointer tmp = make_pointer<T, Allocator>(m_allocator, other.m_size);

            std::uninitialized_move(other.m_data, other.m_data + other.m_size, tmp.data());

            T* old_data         = m_data;
            size_t old_size     = m_size;
            size_t old_capacity = m_capacity;

            m_size        = other.m_size;
            m_capacity    = other.m_size;
            m_data        = tmp.release();

            destroy_and_deallocate(m_allocator, old_data, old_size, old_capacity);
        }
        else
        {
            size_t overlap_gap = std::min(m_size, other.m_size);

            if constexpr (std::is_move_assignable_v<T>)
            {
                std::move(other.m_data, other.m_data + overlap_gap, m_data);
            }
            else
            {
                if constexpr (!std::is_trivially_destructible_v<T>)
                    destroy_range(m_allocator, m_data, m_data + overlap_gap);

                std::uninitialized_move(other.m_data, other.m_data + overlap_gap, m_data);
            }

            if (m_size < other.m_size)
            {
                std::uninitialized_move(other.m_data + m_size, other.m_data + other.m_size, m_data + m_size);
            }

            if (m_size > other.m_size)
            {
                // destroy odd elements
                if constexpr(!std::is_trivially_destructible_v<T>)
                {
                    destroy_range(m_allocator, m_data + other.m_size, m_data + m_size);
                }
            }

            m_size        = other.m_size;
        }
    }

    void steal_from(Vector&& other)
    requires std::allocator_traits<Allocator>::is_always_equal::value ||
            std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value
    {
        auto old_allocator  = m_allocator;
        T* old_data         = m_data;
        size_t old_size     = m_size;
        size_t old_capacity = m_capacity;

        if constexpr(!std::allocator_traits<Allocator>::is_always_equal::value)
            m_allocator = std::move(other.m_allocator);

        m_size      = other.m_size;
        m_capacity  = other.m_capacity;
        m_data      = other.m_data;

        destroy_and_deallocate(old_allocator, old_data, old_size, old_capacity);

        other.m_size     = 0;
        other.m_capacity = 0;
        other.m_data     = nullptr;
    }

    size_t growth_policy()
    {
        const size_t max_size = std::allocator_traits<Allocator>::max_size(m_allocator);

        if (m_size == max_size)
        {
            throw std::length_error("Vector::template <typename ... Args> void emplace_back(Args&& ... args): vector is full");
        }

        return capacity_growth_policy(m_capacity);
    }

    size_t capacity_growth_policy(const size_t old_capacity)
    {
        const size_t max_size = std::allocator_traits<Allocator>::max_size(m_allocator);

        size_t new_capacity;

        if (old_capacity > max_size / 2)
        {
            new_capacity = max_size;
        }
        else if (old_capacity)
        {
            new_capacity = old_capacity * 2; // no overflow
        }
        else
        {
            constexpr size_t default_capacity = 1;
            new_capacity = default_capacity;
        }

        return new_capacity;
    }

    template <typename U>
    iterator realloc_insert(const_iterator pos, size_t count, U&& value, size_t new_capacity)
    requires std::constructible_from<T, U&&>
    {
        Pointer tmp = make_pointer<T, Allocator>(m_allocator, new_capacity);
        size_t offset = pos - cbegin();

        T tmp_value{std::forward<U>(value)};

        uninitialized_move_if_noexcept(m_data, m_data + offset, tmp.data());

        try
        {
            std::uninitialized_fill_n(tmp.data() + offset, count, std::move_if_noexcept(tmp_value));
        }
        catch (...)
        {
            destroy_range(m_allocator, tmp.data(), tmp.data() + offset);

            throw;
        }

        try
        {
            uninitialized_move_if_noexcept(m_data + offset, m_data + m_size, tmp.data() + offset + count);
        }
        catch (...)
        {
            destroy_range(m_allocator, tmp.data(), tmp.data() + offset + count);

            throw;
        }


        T* old_data = m_data;
        size_t old_size = m_size;
        size_t old_capacity = m_capacity;


        m_capacity  = new_capacity;
        m_size      = m_size + count;
        m_data      = tmp.release();


        destroy_and_deallocate(m_allocator, old_data, old_size, old_capacity);


        return iterator(m_data + offset);
    }

    template <typename U>
    iterator inplace_insert(const_iterator pos, size_t count, U&& value)
    requires std::constructible_from<T, U&&> &&
         (std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>)
    {
        size_t offset = pos - cbegin();

        T tmp_value{std::forward<U>(value)};

        if (offset < m_size)
        {
            right_shift(pos, count);
        }

		const size_t tail    = m_size - offset;
        const size_t spill   = std::min(count, tail); // elements that should be constructed in uninitialized memory

        if (count > tail)
        {
			try
			{
			    std::fill_n(m_data + offset, tail, std::forward<U>(value));
				std::uninitialized_fill_n(m_data + offset + tail, count - tail, std::forward<U>(value));
			}
			catch (...)
			{
				destroy_range(m_allocator, m_data + m_size + count - spill, m_data + count); // destroying the right shifted data

				throw;
			}
        }
        else
        {
			try
			{
				std::fill_n(m_data + offset, count, std::forward<U>(value));
			}
			catch (...)
			{
				destroy_range(m_allocator, m_data + m_size + count - spill, m_data + count);

				throw;
			}
        }

        m_size += count;

        return iterator(m_data + offset);
    }

    template <typename U>
    iterator insert_impl(const_iterator pos, size_t count, U&& value)
    requires std::constructible_from<T, U&&>
    {
        constexpr bool can_throw = !std::is_nothrow_move_constructible_v<T> && !std::is_nothrow_copy_assignable_v<T>;

		size_t new_size = m_size + count;

        if (new_size > m_capacity) // reallocation path. Basic exception guarantee if allocator::construct
        {
            const size_t new_capacity = new_size;

            return realloc_insert(pos, count, std::forward<U>(value), new_capacity);
        }

        if (!can_throw) // inplace insert. Basic exception guarantee if allocator::construct may throw
        {
            return inplace_insert(pos, count, std::forward<U>(value));
        }

        const size_t new_capacity = m_capacity;

        return realloc_insert(pos, count, std::forward<U>(value), new_capacity);
    }

    template <std::forward_iterator It>
    iterator realloc_insert_range(const_iterator pos, It first, It last, size_t new_capacity, size_t n)
    {
        Pointer tmp = make_pointer<T, Allocator>(m_allocator, new_capacity);
        const size_t offset = pos - cbegin();

        uninitialized_move_if_noexcept(m_data, m_data + offset, tmp.data());

        try
        {
            using Ref = std::iter_reference_t<It>;

            if constexpr(std::is_rvalue_reference_v<Ref>)
            {
                std::uninitialized_move(first, last, tmp.data() + offset);
            }
            else
            {
                std::uninitialized_copy(first, last, tmp.data() + offset);
            }
        }
        catch (...)
        {
            destroy_range(m_allocator, tmp.data(), tmp.data() + offset);

            throw;
        }

        try
        {
            uninitialized_move_if_noexcept(m_data + offset, m_data + m_size, tmp.data() + offset + n);
        }
        catch (...)
        {
            destroy_range(m_allocator, tmp.data(), tmp.data() + offset + n);

            throw;
        }

        T* old_data = m_data;
        size_t old_size = m_size;
        size_t old_capacity = m_capacity;

        m_capacity  = new_capacity;
        m_size      = m_size + n;
        m_data      = tmp.release();

        destroy_and_deallocate(m_allocator, old_data, old_size, old_capacity);

        return iterator(m_data + offset);
    }

	void right_shift(const_iterator pos, size_t n)
	{
		const size_t offset = pos - cbegin();
        const size_t tail    = m_size - offset;
        const size_t spill   = std::min(n, tail); // elements that should be constructed in uninitialized memory
        const size_t overlap = tail - spill;          // number of elements that should be shifted right

        // Move last spil elements into uninitialized place
        uninitialized_move_if_noexcept(m_data + offset + overlap,
                                        m_data + m_size,
                                       m_data + m_size + n - spill);

        // Shift to the right by n
        try
        {
            move_backward_if_noexcept(
                        m_data + offset,
                        m_data + offset + overlap,
                       m_data + offset + overlap + n);
        }
        catch (...)
        {
            destroy_range(m_allocator, m_data + m_size + n - spill, m_data + m_size + n);

            throw;
        }
	}


    template <std::forward_iterator It>
    iterator inplace_insert_range(const_iterator pos, It first, It last)
    requires (std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>) &&
             (std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>) &&
              std::is_assignable_v<T&, std::iter_reference_t<It>> &&
              std::is_constructible_v<T, std::iter_reference_t<It>>
    {
       	const size_t n = std::distance(first, last);

		right_shift(pos, n);

        // constructing new elements
        using Ref = std::iter_reference_t<It>;

        const size_t offset = pos - cbegin();
 		const size_t tail    = m_size - offset;
        const size_t spill   = std::min(n, tail); // elements that should be constructed in uninitialized memory

        if (n > tail)
        {
            auto mid = std::next(first, tail); // the position of range where elements in which can be copied/moved

            try
            {
                // tail elements are shifted to the end so there are tail elements that can be copied/moved to the initialized memory
                if constexpr(std::is_rvalue_reference_v<Ref>)
                {
                    std::move(first, mid, m_data + offset);
                    std::uninitialized_move(mid, last, m_data + offset + tail);
                }
                else
                {
                    std::copy(first, mid, m_data + offset);
                    std::uninitialized_copy(mid, last, m_data + offset + tail);
                }
            }
            catch (...)
            {
                destroy_range(m_allocator, m_data + m_size + n - spill, m_data + n);

                throw;
            }
        }
        else
        {
            try
            {
                if constexpr(std::is_rvalue_reference_v<Ref>)
                {
                    std::move(first, last, m_data + offset);
                }
                else
                {
                    std::copy(first, last, m_data + offset);
                }
            }
            catch (...)
            {
                destroy_range(m_allocator, m_data + m_size + n - spill, m_data + n);

                throw;
            }
        }

        m_size += n;

        return iterator(m_data + offset);
    }

    template <std::forward_iterator It>
    iterator insert_forward_it(const_iterator pos, It first, It last)
    requires std::constructible_from<T, std::iter_reference_t<It>> &&
             (std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>)
    {
        const size_t n      = std::distance(first, last);
        const size_t new_size = m_size + n;
        const size_t new_capacity = new_size;

        if (new_size > m_capacity) // reallocation path. Basic exception guarantee if allocator::construct throws
        {
            return realloc_insert_range(pos, first, last, new_capacity, n);
        }

        // inplace insert. Basic exception guarantee if allocator::construct may throw
        return inplace_insert_range(pos, first, last);
    }

    template <std::input_iterator It>
    iterator insert_input_it(const_iterator pos, It first, It last)
    requires std::constructible_from<T, std::iter_reference_t<It>> &&
        (std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>)
    {
        size_t offset = pos - cbegin();
        size_t old_size = m_size;

        for (auto it = first; it != last; ++it)
        {
            try
            {
                emplace_back(*it);
            }
            catch (...)
            {
                destroy_range(m_allocator, m_data + old_size, m_data + m_size);
                m_size = old_size;

                throw;
            }
        }

        std::rotate(m_data + offset, m_data + old_size, m_data + m_size);

        return iterator(m_data + offset);
    }

    template<std::input_iterator It>
    void input_iter_ctor_impl(It begin, It end)
    {
        while (begin != end)
        {
            emplace_back(*begin);
            ++begin;
        }
    }

    template<std::forward_iterator It>
    void forward_iter_ctor_impl(It begin, It end)
    {
        size_t n = std::distance(begin, end);

        Pointer tmp = make_pointer<T>(m_allocator, n);

        if constexpr (std::is_rvalue_reference_v<std::iter_reference_t<It>>)
        {
            std::uninitialized_move(begin, end, tmp.data());
        }
        else
        {
            std::uninitialized_copy(begin, end, tmp.data());
        }

        m_data     = tmp.release();
        m_size     = n;
        m_capacity = n;
    }

    template <std::forward_iterator InputIt>
    void assign_range_forward_iter_realloc(InputIt first, InputIt last)
    {
        auto old_data    = m_data;
        size_t old_size     = m_size;
        size_t old_capacity = m_capacity;

        forward_iter_ctor_impl(first, last);

        destroy_and_deallocate(m_allocator, old_data, old_size, old_capacity);
    }

    template <std::forward_iterator InputIt>
    void assign_range_forward_iter_inplace(InputIt first, InputIt last)
    {
        size_t count = std::distance(first, last);
        using Ref = std::iter_reference_t<InputIt>;

        if (count > m_size)
        {
            auto mid = std::next(first, m_size);

            if constexpr(std::is_rvalue_reference_v<Ref>)
            {
                std::move(first, mid, m_data);
                std::uninitialized_move(mid, last, m_data + m_size);
            }
            else
            {
                std::copy(first, mid, m_data);
                std::uninitialized_copy(mid, last, m_data + m_size);
            }
        }
        else
        {
            if constexpr(std::is_rvalue_reference_v<Ref>)
            {
                std::move(first, last, m_data);
            }
            else
            {
                std::copy(first, last, m_data);
            }

            if constexpr(!std::is_trivially_destructible_v<T>)
            {
                destroy_range(m_allocator, m_data + count, m_data + m_size);
            }
        }
    }


    template <std::input_iterator InputIt>
    void assign_range_input_iter(InputIt first, InputIt last)
    {
        size_t capacity = m_capacity;
        size_t size = 0;
        T* tmp = std::allocator_traits<Allocator>::allocate(m_allocator, capacity);

        while (first != last)
        {
            if (size == capacity)
            {

                auto old_capacity = capacity;
                capacity = capacity_growth_policy(capacity);

                try
                {
                    T* new_ptr = reallocate(m_allocator, tmp, size, old_capacity, capacity);
                    tmp = new_ptr;
                }
                catch (...)
                {
                    destroy_and_deallocate(m_allocator, tmp, size, old_capacity);

                    throw;
                }
            }

            try
            {
                std::allocator_traits<Allocator>::construct(m_allocator, tmp + size, *first);
            }
            catch (...)
            {
                if constexpr(!std::is_trivially_destructible_v<T>)
                {
                    destroy_and_deallocate(m_allocator, tmp, size, capacity);
                }

                throw;
            }

            ++size;
            ++first;
        }

        auto old_data    = m_data;
        size_t old_size     = m_size;
        size_t old_capacity = m_capacity;

        m_data      = tmp;
        m_capacity  = capacity;
        m_size      = size;

        destroy_and_deallocate(m_allocator, old_data, old_size, old_capacity);
    }

    template <std::forward_iterator InputIt>
    void assign_range_forward_iter(InputIt first, InputIt last)
    {
        size_t count = std::distance(first, last);

        using Ref = std::iter_reference_t<InputIt>;

        constexpr bool can_throw = !std::is_nothrow_assignable_v<T, Ref> ||
            !std::is_nothrow_constructible_v<T, Ref>;

        if (count > m_capacity || can_throw) // reallocation path
        {
           assign_range_forward_iter_realloc(first, last);
        }
        else // inplace path
        {
            assign_range_forward_iter_inplace(first, last);
        }
    }
};

#endif //VECTOR_VECTOR_H

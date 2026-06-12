//
// Created by user on 6/2/2026.
//

#ifndef VECTOR_TESTS_THROWING_TYPES_H
#define VECTOR_TESTS_THROWING_TYPES_H

#include <stdexcept>

struct ThrowingType {
    int value;
    inline static int copy_count       = 0;
    inline static int live_count       = 0;
    inline static int moved_count      = 0;
    inline static int copy_ctor_thold  = 33;
    inline static int move_ctor_thold  = 33;
    inline static int copy_thold       = 33;
    inline static int move_thold       = 33;

    explicit ThrowingType(int v = 0) : value(v) { ++live_count; }

    ThrowingType(const ThrowingType& other) : value(other.value)
    {
        if (copy_count == copy_ctor_thold) {
            throw std::runtime_error("33rd copy triggered");
        }

        ++copy_count;
        ++live_count;
    }

    ThrowingType(ThrowingType&& other) : value(other.value)
    {
        if (moved_count == move_ctor_thold)
        {
            throw std::runtime_error("33rd copy triggered");
        }

        ++moved_count;
        ++live_count;
    }

    ThrowingType& operator=(const ThrowingType& other)
    {
        ++copy_count;
        if (copy_count == copy_thold) {
            throw std::runtime_error("33rd assign triggered");
        }
        value = other.value;
        return *this;
    }

    static void set_thold(size_t thold)
    {
        copy_ctor_thold = thold;
        move_ctor_thold = thold;
        copy_thold = thold;
        move_thold = thold;
    }

    bool operator==(const ThrowingType& other) const
    {
        return value == other.value;
    }

    static void reset_counts()
    {
        copy_count  = 0;
        live_count  = 0;
        moved_count = 0;
    }

    ~ThrowingType() { --live_count; }
};

#endif //VECTOR_TESTS_THROWING_TYPES_H

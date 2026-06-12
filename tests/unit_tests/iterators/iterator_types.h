//
// Created by user on 6/1/2026.
//

#ifndef VECTOR_TESTS_ITERATOR_TYPES_H
#define VECTOR_TESTS_ITERATOR_TYPES_H

#include <Vector.h>
#include <vector>
#include <iostream>

#include "gtest/internal/gtest-type-util.h"

using Vec = Vector<int>;

using IteratorTypes = ::testing::Types<
        Vec::iterator,
        Vec::const_iterator,
        Vec::reverse_iterator,
        Vec::const_reverse_iterator
    >;

template <typename It>
struct Iterator;

template <>
struct Iterator<Vec::iterator>
{
    template <typename Vector>
    static Vector::iterator begin(Vector &v) { return v.begin(); }

    template <typename Vector>
    static Vector::iterator end(Vector &v) { return v.end();}
};

template <>
struct Iterator<Vec::const_iterator>
{
    template <typename Vector>
    static Vector::const_iterator begin(const Vector &v) { return v.cbegin(); }

    template <typename Vector>
    static Vector::const_iterator end(const Vector &v) { return v.cend(); }
};

template <>
struct Iterator<Vec::reverse_iterator>
{
    template <typename Vector>
    static Vector::reverse_iterator begin(Vector &v) { return v.rbegin(); }

    template <typename Vector>
    static Vector::reverse_iterator end(Vector &v) { return v.rend(); }
};

template <>
struct Iterator<Vec::const_reverse_iterator>
{
    template <typename Vector>
    static Vector::const_reverse_iterator begin(const Vector &v) { return v.crbegin(); }

    template <typename Vector>
    static Vector::const_reverse_iterator end(const Vector &v) { return v.crend(); }
};

template <typename It>
class IteratorUnitTests : public ::testing::Test
{
protected:
    Vec non_empty{1, 2, 3};
    Vec empty_vec;
    Vec v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> ref{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vec v2;
};

#endif //VECTOR_TESTS_ITERATOR_TYPES_H
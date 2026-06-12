//
// Created by user on 5/29/2026.
//

#ifndef VECTOR_TESTS_VECTOR_EQ_H
#define VECTOR_TESTS_VECTOR_EQ_H

template <typename Vec1, typename Vec2>
bool vector_eq(const Vec1& lhs, const Vec2& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    auto it1 = lhs.begin();
    auto it2 = rhs.begin();

    for (; it1 != lhs.end(); ++it1, ++it2)
    {
        if (*it1 != *it2)
            return false;
    }

    return true;
}

#define EXPECT_VECTOR_EQ(v1, v2)  EXPECT_TRUE(vector_eq(v1, v2))

template <typename Vec>
void vector_fill(Vec& v)
{
    while (v.size() < v.capacity())
    {
        v.emplace_back();
    }
}

#endif //VECTOR_TESTS_VECTOR_EQ_H
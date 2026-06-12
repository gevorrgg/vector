#include <gtest/gtest.h>
#include "vector.hpp"

TEST(ElementAccessUnitTests, OperatorIndexValid) {
    Vector<int> v{10, 20, 30};
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);

    v[1] = 99;
    EXPECT_EQ(v[1], 99);
}

TEST(ElementAccessUnitTests, OperatorIndexConst) {
    const Vector<int> v{1, 2, 3};
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(ElementAccessUnitTests, AtValid) {
    Vector<int> v{5, 6, 7};
    EXPECT_EQ(v.at(0), 5);
    EXPECT_EQ(v.at(1), 6);
    EXPECT_EQ(v.at(2), 7);
}

TEST(ElementAccessUnitTests, AtThrowsOutOfRange) {
    Vector<int> v{1, 2, 3};
    EXPECT_THROW(v.at(3), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(ElementAccessUnitTests, FrontBackValid) {
    Vector<int> v{11, 22, 33};
    EXPECT_EQ(v.front(), 11);
    EXPECT_EQ(v.back(), 33);

    v.front() = 100;
    v.back() = 200;
    EXPECT_EQ(v[0], 100);
    EXPECT_EQ(v[2], 200);
}

TEST(ElementAccessUnitTests, FrontBackConst) {
    const Vector<int> v{7, 8, 9};
    EXPECT_EQ(v.front(), 7);
    EXPECT_EQ(v.back(), 9);
}

TEST(ElementAccessUnitTests, DataPointer) {
    Vector<int> v{1, 2, 3};
    int* ptr = v.data();
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);

    const Vector<int> cv{4, 5, 6};
    const int* cptr = cv.data();
    EXPECT_EQ(cptr[0], 4);
    EXPECT_EQ(cptr[1], 5);
    EXPECT_EQ(cptr[2], 6);
}
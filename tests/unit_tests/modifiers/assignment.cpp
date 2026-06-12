#include <gtest/gtest.h>
#include "Vector.h"
#include <sstream>
#include <iterator>
#include <sstream>

TEST(AssignmentUnitTests, CopyAssignmentWithoutReallocation) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2;
    v2.reserve(10);
    v2 = v1;

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
}

TEST(AssignmentUnitTests, CopyAssignmentWithReallocation) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2;
    v2 = v1;

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
}

TEST(AssignmentUnitTests, MoveAssignmentWithoutReallocation) {
    Vector<int> v1{10, 20, 30};
    Vector<int> v2;
    v2.reserve(10);
    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 10);
    EXPECT_EQ(v2[1], 20);
    EXPECT_EQ(v2[2], 30);
    EXPECT_TRUE(v1.empty());
}

TEST(AssignmentUnitTests, MoveAssignmentWithReallocation) {
    Vector<int> v1{10, 20, 30};
    Vector<int> v2;
    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 10);
    EXPECT_EQ(v2[1], 20);
    EXPECT_EQ(v2[2], 30);
    EXPECT_TRUE(v1.empty());
}

TEST(AssignmentUnitTests, AssignCountValueWithoutReallocation) {
    Vector<int> v;
    v.reserve(10);
    v.assign(4, 7);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 7);
    EXPECT_EQ(v[1], 7);
    EXPECT_EQ(v[2], 7);
    EXPECT_EQ(v[3], 7);
}

TEST(AssignmentUnitTests, AssignCountValueWithReallocation) {
    Vector<int> v;
    v.assign(4, 7);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 7);
    EXPECT_EQ(v[1], 7);
    EXPECT_EQ(v[2], 7);
    EXPECT_EQ(v[3], 7);
}

TEST(AssignmentUnitTests, AssignRangeForwardIteratorWithoutReallocation) {
    int arr[] = {1, 2, 3, 4};
    Vector<int> v;
    v.reserve(10);
    v.assign(arr, arr + 4);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
}

TEST(AssignmentUnitTests, AssignRangeForwardIteratorWithReallocation) {
    int arr[] = {1, 2, 3, 4};
    Vector<int> v;
    v.assign(arr, arr + 4);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
}

TEST(AssignmentUnitTests, AssignRangeInputIterator) {
    std::istringstream iss("5 6 7 8");
    std::istream_iterator<int> first(iss);
    std::istream_iterator<int> last;
    Vector<int> v;
    v.assign(first, last);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v[1], 6);
    EXPECT_EQ(v[2], 7);
    EXPECT_EQ(v[3], 8);
}

TEST(AssignmentUnitTests, AssignInitializerListWithoutReallocation) {
    Vector<int> v;
    v.reserve(10);
    v.assign({9, 8, 7});

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 9);
    EXPECT_EQ(v[1], 8);
    EXPECT_EQ(v[2], 7);
}

TEST(AssignmentUnitTests, AssignInitializerListWithReallocation) {
    Vector<int> v;
    v.assign({9, 8, 7});

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 9);
    EXPECT_EQ(v[1], 8);
    EXPECT_EQ(v[2], 7);
}

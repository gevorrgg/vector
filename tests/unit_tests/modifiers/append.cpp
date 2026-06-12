#include <gtest/gtest.h>
#include <Vector.h>
#include <vector>

struct MyType
{
    MyType(int a, double b, char c) noexcept
       : a(a), b(b), c(c)
    {
        ++constructions;
    }

    int a;
    double b;
    char c;

    static int constructions;

    bool operator==(const MyType& other) const
    {
        return a == other.a && b == other.b && c == other.c;
    }
};

struct BasicMoveContainer
{
    mutable bool is_moved_from = false;
    mutable bool is_copied = false;

    BasicMoveContainer(const BasicMoveContainer& other)
        : m_v(other.m_v),
        is_moved_from(other.is_moved_from),
        is_copied(other.is_copied)
    {
        other.is_copied = true;
    }

    explicit BasicMoveContainer(std::vector<int>&& v) noexcept : m_v(std::move(v))
    {}

    BasicMoveContainer(BasicMoveContainer&& other) noexcept : m_v(std::move(other.m_v))
    {
        is_moved_from = other.is_moved_from;
        is_copied = other.is_copied;
        other.is_moved_from = true;
        other.is_copied = false;
    }

    BasicMoveContainer& operator=(const BasicMoveContainer& other)
    {
        if (this == &other)
            return *this;

        m_v = other.m_v;
        is_moved_from = other.is_moved_from;
        is_copied = other.is_copied;

        other.is_moved_from = false;
        other.is_copied = true;

        return *this;
    }
    BasicMoveContainer& operator=(BasicMoveContainer&& other)
    {
        if (this == &other)
            return *this;

        m_v = std::move(other.m_v);
        is_moved_from = other.is_moved_from;
        is_copied = other.is_copied;
        other.is_moved_from = true;
        other.is_copied = false;

        return *this;
    }

    bool operator==(const BasicMoveContainer& other) const
    {
        return m_v == other.m_v;
    }


private:
    std::vector<int> m_v;
};

int MyType::constructions = 0;

TEST(ModifiersUnitTests, EmplaceBackWithValue)
{
    Vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    auto old_size = v.size();

    v.emplace_back(2);

    EXPECT_EQ(old_size + 1, v.size());

    EXPECT_EQ(v.back(), 2);

    Vector<int> v2;

    old_size = v2.size();
    v2.emplace_back(2);
    EXPECT_EQ(old_size + 1, v2.size());
    EXPECT_EQ(v2.back(), 2);
}

TEST(ModifiersUnitTests, EmplaceBackWithArguments)
{
    Vector<MyType> v{MyType{1, 1.2, 'a'}, MyType{2, 1.44, 'b'}};

    auto old_size = v.size();

    MyType::constructions = 0;

    v.emplace_back(10, 3.14, 'c');

    EXPECT_EQ(v.size(), old_size + 1);
    EXPECT_EQ(MyType::constructions, 1);
    EXPECT_TRUE(v.back() == MyType(10, 3.14, 'c'));

    Vector<MyType> v2;

    old_size = v2.size();

    MyType::constructions = 0;

    v2.emplace_back(10, 3.14, 'c');

    EXPECT_EQ(v2.size(), old_size + 1);
    EXPECT_EQ(MyType::constructions, 1);
    EXPECT_TRUE(v2.back() == MyType(10, 3.14, 'c'));
}

TEST(ModifiersUnitTests, EmplaceBackWithMove)
{
    Vector<BasicMoveContainer> v;

    std::vector vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector vec_cpy = vec;
    BasicMoveContainer container{std::move(vec)};
    BasicMoveContainer container_cpy{std::move(vec_cpy)};

    auto old_size = v.size();

    v.emplace_back(std::move(container));

    EXPECT_TRUE(container.is_moved_from);
    EXPECT_FALSE(container.is_copied);
    EXPECT_EQ(v.size(), old_size + 1);
    EXPECT_TRUE(v.back() == container_cpy);

    vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    vec_cpy = vec;
    BasicMoveContainer container2{std::move(vec)};
    BasicMoveContainer container2_cpy{std::move(vec_cpy)};

    old_size = v.size();

    v.emplace_back(std::move(container2));

    EXPECT_TRUE(container2.is_moved_from);
    EXPECT_FALSE(container2.is_copied);
    EXPECT_EQ(v.size(), old_size + 1);
    EXPECT_TRUE(v.back() == container2_cpy);
}

TEST(ModifiersUnitTests, PushBackWithMoveSemantics)
{
    Vector<BasicMoveContainer> v;

    std::vector vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector vec_cpy = vec;
    BasicMoveContainer container{std::move(vec)};
    BasicMoveContainer container_cpy{std::move(vec_cpy)};

    auto old_size = v.size();

    v.push_back(std::move(container));

    EXPECT_TRUE(container.is_moved_from);
    EXPECT_FALSE(container.is_copied);
    EXPECT_EQ(v.size(), old_size + 1);
    EXPECT_TRUE(v.back() == container_cpy);

    vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    vec_cpy = vec;
    BasicMoveContainer container2{std::move(vec)};
    BasicMoveContainer container2_cpy{std::move(vec_cpy)};

    old_size = v.size();

    v.push_back(std::move(container2));

    EXPECT_TRUE(container2.is_moved_from);
    EXPECT_FALSE(container2.is_copied);
    EXPECT_EQ(v.size(), old_size + 1);
    EXPECT_TRUE(v.back() == container2_cpy);
}

TEST(ModifiersUnitTests, PushBackWithCopySemantics)
{
    Vector<BasicMoveContainer> v;

    std::vector vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector vec_cpy = vec;
    BasicMoveContainer container{std::move(vec)};
    BasicMoveContainer container_cpy{std::move(vec_cpy)};

    auto old_size = v.size();

    v.push_back(container);

    EXPECT_FALSE(container.is_moved_from);
    EXPECT_TRUE(container.is_copied);
    EXPECT_EQ(v.size(), old_size + 1);
    EXPECT_TRUE(v.back() == container_cpy);

    vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    vec_cpy = vec;
    BasicMoveContainer container2{std::move(vec)};
    BasicMoveContainer container2_cpy{std::move(vec_cpy)};

    old_size = v.size();

    v.push_back(container2);

    EXPECT_FALSE(container2.is_moved_from);
    EXPECT_TRUE(container2.is_copied);
    EXPECT_EQ(v.size(), old_size + 1);
    EXPECT_TRUE(v.back() == container2_cpy);
}
cmake_minimum_required(VERSION 3.20)
project(iterator_unit_tests)


add_executable(iterator_unit_tests
        iterator_consistency.cpp
        invalidation.cpp
        iterator_arithmetics.cpp
        iterator_comparison.cpp
        iterator_traversal.cpp
)

target_link_libraries(iterator_unit_tests
        PRIVATE
        test_support
        Vector::Vector
)

include(GoogleTest)
gtest_discover_tests(iterator_unit_tests)
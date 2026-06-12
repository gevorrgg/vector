cmake_minimum_required(VERSION 3.20)
project(comparison_unit_tests)


add_executable(comparison_unit_tests
        comparison.cpp
)

target_link_libraries(comparison_unit_tests
        PRIVATE
        test_support
        Vector::Vector
)

include(GoogleTest)
gtest_discover_tests(comparison_unit_tests)
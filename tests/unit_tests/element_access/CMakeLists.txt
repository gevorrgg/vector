cmake_minimum_required(VERSION 3.20)
project(element_access_unit_tests)


add_executable(element_access_unit_tests
        element_access.cpp
        size.cpp
)

target_link_libraries(element_access_unit_tests
        PRIVATE
        test_support
        Vector::Vector
)

include(GoogleTest)
gtest_discover_tests(element_access_unit_tests)
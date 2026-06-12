cmake_minimum_required(VERSION 3.20)
project(modifiers_unit_tests)


add_executable(modifiers_unit_tests
        append.cpp
        remove_elements.cpp
        insert.cpp
        assignment.cpp
)

target_link_libraries(modifiers_unit_tests
        PRIVATE
        test_support
        Vector::Vector
)

include(GoogleTest)
gtest_discover_tests(modifiers_unit_tests)
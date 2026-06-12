cmake_minimum_required(VERSION 3.20)
project(modifiers_exception_safety_tests)


add_executable(modifiers_exception_safety_tests
        assignment.cpp
        insert.cpp
        append.cpp
        remove_elements.cpp
)

target_link_libraries(modifiers_exception_safety_tests
        PRIVATE
        test_support
        Vector::Vector
)

include(GoogleTest)
gtest_discover_tests(modifiers_exception_safety_tests)

cmake_minimum_required(VERSION 3.20)
project(constructor_unit_tests)


add_executable(constructor_unit_tests
               default_constructors.cpp
               copy_constructors.cpp
               move_constructor.cpp
               range_constructors.cpp
)

target_link_libraries(constructor_unit_tests
        PRIVATE
        test_support
        Vector::Vector
)

include(GoogleTest)
gtest_discover_tests(constructor_unit_tests)

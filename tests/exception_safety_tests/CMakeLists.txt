
cmake_minimum_required(VERSION 3.20)

project(vector_tests)

enable_testing()

add_subdirectory(constructors)
add_subdirectory(modifiers)

add_custom_target(exception_safety_tests ALL)

add_dependencies(exception_safety_tests
    constructor_exception_safety_tests
    modifiers_exception_safety_tests
)

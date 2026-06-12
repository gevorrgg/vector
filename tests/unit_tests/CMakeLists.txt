cmake_minimum_required(VERSION 3.20)

project(vector_tests)

enable_testing()

add_subdirectory(constructors)
add_subdirectory(modifiers)
add_subdirectory(iterators)
add_subdirectory(element_access)
add_subdirectory(comparison)

add_custom_target(unit_tests ALL)

add_dependencies(unit_tests
        constructor_unit_tests
        modifiers_unit_tests
        iterator_unit_tests
        element_access_unit_tests
        comparison_unit_tests
)

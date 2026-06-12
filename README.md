# Vector

A custom C++20 `std::vector`-compatible dynamic array with full allocator support, iterator support, and comprehensive exception safety guarantees.

## Features

- **Full allocator support** — respects `propagate_on_container_copy_assignment`, `propagate_on_container_move_assignment`, `select_on_container_copy_construction`, and `is_always_equal`
- **Exception safety** — strong guarantee on constructors, `reserve`, `resize`, `push_back`/`emplace_back`, and `assign`; basic guarantee on `insert` when move/copy may throw
- **Iterator support** — `iterator`, `const_iterator`, `reverse_iterator`, `const_reverse_iterator`
- **C++20 concepts** — constructors and modifiers are constrained with `std::constructible_from`, `std::input_iterator`, `std::forward_iterator`, etc.
- **Efficient reallocation** — uses `move_if_noexcept` throughout; forward iterators avoid double-pass; input iterators handled separately

## Requirements

- C++20 compiler (GCC 11+, Clang 14+, MSVC 19.29+)
- CMake 3.20+
- Google Test (fetched automatically via CMake FetchContent)

## Project Structure

```
.
├── CMakeLists.txt
├── include
│   ├── Vector.h           # Main Vector template class
│   └── vector_memory.h    # Allocator helpers (make_pointer, reallocate, destroy_range, ...)
├── readme.md
└── tests
    ├── helpers
    │   ├── allocators.h       # Custom allocators for testing (tracking, throwing, stateful)
    │   ├── throwing_types.h   # Types that throw on copy/move/construct at configurable points
    │   └── vector_eq.h        # Deep equality helpers for test assertions
    ├── unit_tests
    │   ├── comparison/        # operator== / operator!=
    │   ├── constructors/      # Default, copy, move, range, initializer_list constructors
    │   ├── element_access/    # operator[], at(), front(), back(), data(), size(), capacity()
    │   ├── iterators/         # Arithmetic, comparison, traversal, consistency, invalidation
    │   └── modifiers/         # push_back, emplace_back, insert, erase, assign, resize, reserve
    └── exception_safety_tests
        ├── constructors/      # Exception safety for all constructor forms
        └── modifiers/         # Exception safety for append, assignment, insert, remove
```

## Building

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Running Tests

```bash
# All tests
ctest --test-dir build

# Unit tests only
./build/tests/unit_tests/unit_tests

# Exception safety tests only
./build/tests/exception_safety_tests/exception_safety_tests
```

## API Reference

### Constructors

```cpp
Vector()                                          // default
Vector(size_t size)                               // value-initialized elements
Vector(size_t size, const T& value)               // fill constructor
Vector(const Vector& other)                       // copy
Vector(Vector&& other)                            // move
Vector(It begin, It end)                          // range (input or forward iterator)
Vector(std::initializer_list<T>)                  // initializer list
```

All constructors accept an optional `Allocator` argument as a trailing parameter.

### Capacity

| Method | Description |
|---|---|
| `size()` | Number of elements |
| `capacity()` | Allocated storage |
| `empty()` | True if size is 0 |
| `max_size()` | Maximum possible size |
| `reserve(n)` | Ensure capacity ≥ n |
| `shrink_to_fit()` | Reduce capacity to size |

### Element Access

| Method | Description |
|---|---|
| `operator[](i)` | Unchecked access (asserts in debug) |
| `at(i)` | Checked access, throws `std::out_of_range` |
| `front()` | First element |
| `back()` | Last element |
| `data()` | Raw pointer to storage |

### Modifiers

| Method | Description |
|---|---|
| `push_back(value)` | Append copy |
| `push_back(T&&)` | Append move |
| `emplace_back(args...)` | Construct in place at end |
| `pop_back()` | Remove last element |
| `insert(pos, value)` | Insert copy or move at iterator |
| `insert(pos, count, value)` | Insert `count` copies |
| `insert(pos, first, last)` | Insert range |
| `insert(pos, ilist)` | Insert initializer list |
| `erase(pos)` | Remove element at iterator |
| `erase(first, last)` | Remove range |
| `assign(count, value)` | Replace contents with `count` copies |
| `assign(first, last)` | Replace contents with range |
| `assign(ilist)` | Replace contents with initializer list |
| `resize(n)` | Resize, value-initializing new elements |
| `resize(n, value)` | Resize, fill-initializing new elements |
| `clear()` | Destroy all elements, keep capacity |

### Iterators

```cpp
begin() / end()           // iterator
cbegin() / cend()         // const_iterator
rbegin() / rend()         // reverse_iterator
crbegin() / crend()       // const_reverse_iterator
```

### Comparison

```cpp
operator==(const Vector&)
operator!=(const Vector&)
```

## Exception Safety

| Operation | Guarantee | Condition |
|---|---|---|
| All constructors | Strong | — |
| `reserve` | Strong | — |
| `resize` | Strong | — |
| `push_back` / `emplace_back` | Strong | — |
| `assign` | Strong | — |
| `insert` (single value) | Strong | if move/copy does not throw |
| `insert` (range, forward it) | Strong | if move/copy does not throw |
| `insert` (range, input it) | Basic | — |
| `erase` | Strong | if move/copy does not throw |
| `operator=` (copy) | Strong | — |
| `operator=` (move) | Strong (nothrow if POCMA or always_equal) | — |

## Growth Policy

Capacity doubles on each reallocation (`new_capacity = old_capacity * 2`), starting from 1. When within factor of 2 of `max_size`, capacity is capped at `max_size`.

## Allocator Propagation

The implementation fully respects all four allocator propagation traits:

- `select_on_container_copy_construction` — used in copy constructor and copy assignment
- `propagate_on_container_copy_assignment` — allocator is replaced on copy assignment when true
- `propagate_on_container_move_assignment` (POCMA) — enables steal-from-other optimization; falls back to element-wise move when false and allocators are unequal
- `is_always_equal` — enables steal-from-other optimization unconditionally when true

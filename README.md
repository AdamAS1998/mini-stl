# MiniSTL

MiniSTL is an educational C++20 project that implements common STL-style containers and container adapters from scratch.

The project was built to explore the internal design of standard data structures, with a focus on manual memory and object lifetime management, generic programming, iterators, copy/move semantics, hashing, heap structures, and algorithmic complexity.

MiniSTL is intended for learning and experimentation only.

## Implemented Containers

| Container | Implementation |
|---|---|
| `Vector` | Dynamic contiguous array |
| `List` | Doubly linked list |
| `Deque` | Dynamic circular buffer |
| `Stack` | Container adapter using `Deque` |
| `Queue` | Container adapter using `Deque` |
| `PriorityQueue` | Binary heap using `Vector` |
| `HashMap` | Hash table using separate chaining |

## Features

- Generic template-based containers
- Manual dynamic memory management
- Explicit object construction and destruction
- Rule of Five and move semantics
- Custom iterators
- STL algorithm compatibility where applicable
- Copy and move support
- Move-only type support
- Perfect forwarding with `emplace`
- Custom comparator support
- Heap construction and maintenance
- Hash collision handling
- Automatic hash table rehashing
- Load factor management
- Container adapters built on other MiniSTL containers

## Complexity Overview

| Container | Operation | Complexity |
|---|---|---|
| `Vector` | Random access | O(1) |
| `Vector` | `push_back` | Amortized O(1) |
| `Vector` | Insert/erase in middle | O(n) |
| `List` | Insert/erase at iterator | O(1) |
| `List` | Search | O(n) |
| `Deque` | Random access | O(1) |
| `Deque` | Push/pop front | Amortized O(1) |
| `Deque` | Push/pop back | Amortized O(1) |
| `Stack` | Push/pop/top | Amortized O(1) |
| `Queue` | Push/pop/front/back | Amortized O(1) |
| `PriorityQueue` | Top | O(1) |
| `PriorityQueue` | Push/pop | O(log n) |
| `PriorityQueue` | Heap construction | O(n) |
| `HashMap` | Find/insert/erase | Average O(1), worst O(n) |

## Container Design

### Vector

`Vector` implements a dynamically growing contiguous array.

It includes:

- Dynamic capacity growth
- Random-access iterators
- Copy and move semantics
- `push_back` and `emplace_back`
- Insert and erase operations
- `reserve`, `resize`, and `shrink_to_fit`
- Manual object lifetime management

### List

`List` is implemented as a doubly linked list.

It includes:

- Bidirectional iterators
- Constant-time insertion and removal at known positions
- Front and back operations
- Copy and move semantics
- `splice`
- `remove`
- `unique`
- `reverse`
- Merge-sort-based sorting

### Deque

`Deque` is implemented using a dynamically growing circular buffer.

It includes:

- Constant-time random access
- Efficient insertion and removal from both ends
- Circular indexing and wraparound handling
- Dynamic buffer growth
- Random-access-style iterators
- Copy and move semantics

### Stack

`Stack` is a container adapter using MiniSTL `Deque` as its default underlying container.

It provides standard LIFO operations:

- `push`
- `emplace`
- `pop`
- `top`
- `empty`
- `size`

### Queue

`Queue` is a container adapter using MiniSTL `Deque` as its default underlying container.

It provides standard FIFO operations:

- `push`
- `emplace`
- `pop`
- `front`
- `back`
- `empty`
- `size`

### PriorityQueue

`PriorityQueue` is implemented as a binary heap using MiniSTL `Vector`.

It includes:

- Max-heap behavior by default
- Custom comparator support
- Min-heap support
- `push`
- `emplace`
- `pop`
- `top`
- Bottom-up heap construction
- Sift-up and sift-down operations

### HashMap

`HashMap` is implemented as a hash table using separate chaining.

It includes:

- Configurable hash functions
- Configurable key equality
- Collision handling through linked chains
- Automatic rehashing
- Load factor management
- `insert`
- `emplace`
- `erase`
- `find`
- `contains`
- `operator[]`
- `at`
- `reserve`
- Iteration over stored elements
- Copy and move semantics

## Testing

Each implemented container has its own test suite.

The tests cover areas including:

- Basic container operations
- Boundary cases
- Copy construction and assignment
- Move construction and assignment
- Iterator behavior
- Const correctness
- Custom types
- Move-only types
- Container growth
- Deque wraparound behavior
- Hash collisions and rehashing
- Custom priority queue comparators
- Comparison against equivalent standard-library containers
- Randomized testing
- Large stress tests

The complete test suite has been compiled and executed successfully using GCC with C++20.

Example:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -g -O0 -Iinclude tests/vector_test.cpp -o vector_test
./vector_test
```

## Memory Testing

The complete test suite was also checked using Valgrind Memcheck.

Example:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./vector_test
```

All seven container test suites completed with:

- 0 detected memory errors
- 0 bytes remaining allocated at exit

The containers tested were:

- Vector
- List
- Deque
- Stack
- Queue
- PriorityQueue
- HashMap

## Building with CMake

### Requirements

- C++20-compatible compiler
- CMake

Clone the repository and enter the project directory:

```bash
git clone https://github.com/AdamAS1998/mini-stl
cd mini-stl
```

Configure the project:

```bash
cmake -S . -B build
```

Build:

```bash
cmake --build build
```

## Running Tests

The tests can be run individually after building, or through CTest:

```bash
ctest --test-dir build --output-on-failure
```

## Project Structure

```text
mini-stl/
├── include/
│   └── ministl/
│       └── containers/
│           ├── vector.hpp
│           ├── linked_list.hpp
│           ├── deque.hpp
│           ├── stack.hpp
│           ├── queue.hpp
│           ├── priority_queue.hpp
│           └── hashmap.hpp
│
├── tests/
│   ├── vector_test.cpp
│   ├── linked_list_test.cpp
│   ├── deque_test.cpp
│   ├── stack_tests.cpp
│   ├── queue_test.cpp
│   ├── priority_queue_test.cpp
│   └── hashmap_test.cpp
│
├── CMakeLists.txt
└── README.md
```

## What I Learned

Building these containers from scratch provided practical experience with:

- C++ templates and generic programming
- Dynamic memory allocation
- Raw storage and object lifetime
- RAII (Resource Acquisition Is Initialization)
- Rule of Five and Rule of Zero
- Copy and move semantics
- Perfect forwarding
- Iterator implementation
- Circular buffers
- Linked data structures
- Binary heaps
- Hash tables and collision handling
- Rehashing and load factors
- Algorithmic complexity
- Testing data structures against the C++ Standard Library
- Memory debugging with Valgrind

## Future Improvements

Possible future additions and improvements include:

- `Set` and tree-based `Map` containers
- Improved exception safety
- Additional algorithms and utilities
- Performance benchmarking against standard-library containers

## Educational Scope

MiniSTL intentionally implements a subset of the functionality provided by the C++ Standard Library.

The goal is to understand how common STL containers and container adapters can be implemented internally rather than to provide a production-ready replacement for the standard library.

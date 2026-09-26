cmake_minimum_required(VERSION 3.27)

project(MiniSTL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MiniSTL
add_library(ministl INTERFACE)

target_include_directories(ministl INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Tests
enable_testing()

function(add_ministl_test test_name source_file)

    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name} PRIVATE ministl)

    add_test(
            NAME ${test_name}
            COMMAND ${test_name}
    )

endfunction()

add_ministl_test(vector_test tests/vector_test.cpp)
add_ministl_test(linked_list_test tests/linked_list_test.cpp)
add_ministl_test(deque_test tests/deque_test.cpp)
add_ministl_test(stack_tests tests/stack_tests.cpp)
add_ministl_test(queue_test tests/queue_test.cpp)
add_ministl_test(priority_queue_test tests/priority_queue_test.cpp)
add_ministl_test(hashmap_test tests/hashmap_test.cpp)
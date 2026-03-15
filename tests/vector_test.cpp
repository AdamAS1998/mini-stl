#include <cassert>
#include <iostream>
#include "../include/ministl/containers/vector.hpp"

using ministl::Vector;

void test_push_back() {
    Vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    assert(v.size() == 3);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
}

void test_capacity_growth() {
    Vector<int> v;

    size_t initial = v.capacity();

    for (int i = 0; i < 100; i++)
        v.push_back(i);

    assert(v.size() == 100);
    assert(v.capacity() >= 100);
}

void test_pop_back() {
    Vector<int> v;

    v.push_back(1);
    v.push_back(2);

    v.pop_back();

    assert(v.size() == 1);
    assert(v.back() == 1);
}

void test_insert() {
    Vector<int> v;

    v.push_back(1);
    v.push_back(3);

    v.insert(1, 2);

    assert(v.size() == 3);
    assert(v[1] == 2);
}

void test_erase() {
    Vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.erase(1);

    assert(v.size() == 2);
    assert(v[1] == 3);
}

void test_resize() {
    Vector<int> v;

    v.resize(5);

    assert(v.size() == 5);

    v.resize(2);

    assert(v.size() == 2);
}

void test_reserve() {
    Vector<int> v;

    v.reserve(50);

    assert(v.capacity() >= 50);
}

void test_clear() {
    Vector<int> v;

    v.push_back(1);
    v.push_back(2);

    v.clear();

    assert(v.empty());
}

void test_front_back() {
    Vector<int> v;

    v.push_back(10);
    v.push_back(20);

    assert(v.front() == 10);
    assert(v.back() == 20);
}

void test_iterator() {
    Vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    int sum = 0;

    for (auto it = v.begin(); it != v.end(); ++it)
        sum += *it;

    assert(sum == 6);
}

void test_range_loop() {
    Vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    int sum = 0;

    for (int x : v)
        sum += x;

    assert(sum == 6);
}

void test_sort() {
    Vector<int> v;

    v.push_back(3);
    v.push_back(1);
    v.push_back(2);

    v.sort();

    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
}

void test_copy_constructor() {
    Vector<int> v;

    v.push_back(1);
    v.push_back(2);

    Vector<int> copy(v);

    assert(copy.size() == 2);
    assert(copy[0] == 1);
    assert(copy[1] == 2);
}

void test_copy_assignment() {
    Vector<int> v;

    v.push_back(1);
    v.push_back(2);

    Vector<int> other;
    other = v;

    assert(other.size() == 2);
    assert(other[0] == 1);
    assert(other[1] == 2);
}

void test_move_semantics() {
    Vector<int> v;

    v.push_back(1);
    v.push_back(2);

    Vector<int> moved = std::move(v);

    assert(moved.size() == 2);
    assert(v.size() == 0);
}

void test_emplace_back() {
    Vector<int> v;

    v.emplace_back(5);
    v.emplace_back(10);

    assert(v.size() == 2);
    assert(v[1] == 10);
}

void test_stress() {
    Vector<int> v;

    const int N = 100000;

    // push many elements
    for (int i = 0; i < N; ++i)
        v.push_back(i);

    assert(v.size() == N);

    // verify values
    for (int i = 0; i < N; ++i)
        assert(v[i] == i);

    // pop half
    for (int i = 0; i < N / 2; ++i)
        v.pop_back();

    assert(v.size() == N / 2);

    // insert in middle repeatedly
    for (int i = 0; i < 1000; ++i)
        v.insert(v.size() / 2, 42);

    // erase many
    for (int i = 0; i < 1000; ++i)
        v.erase(v.size() / 2);

    // final sanity check
    for (size_t i = 0; i < v.size(); ++i)
        assert(v[i] == static_cast<int>(i));

    Vector<int> s;

    for (int i = 100000000; i > 0; --i)
        s.push_back(i);

    s.sort();

    for (int i = 0; i < 100000000; ++i)
        assert(s[i] == i + 1);
}

int main() {

    test_push_back();
    test_capacity_growth();
    test_pop_back();
    test_insert();
    test_erase();
    test_resize();
    test_reserve();
    test_clear();
    test_front_back();
    test_iterator();
    test_range_loop();
    test_sort();
    test_copy_constructor();
    test_copy_assignment();
    test_move_semantics();
    test_emplace_back();
    test_stress();

    std::cout << "All MiniSTL Vector tests passed!\n";
}
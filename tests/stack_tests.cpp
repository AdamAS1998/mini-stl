#include <cassert>
#include <stack>
#include <vector>
#include <string>
#include <utility>
#include <random>
#include <iostream>

#include "../include/ministl/containers/stack.hpp"

using namespace ministl;

/** AI GENERATED TESTS **/

// ============================================================
// Move-only helper
// ============================================================

struct MoveOnly {
    int value;

    explicit MoveOnly(int v)
            : value(v) {}

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&& other) noexcept
            : value(other.value) {
        other.value = -1;
    }

    MoveOnly& operator=(MoveOnly&& other) noexcept {
        value = other.value;
        other.value = -1;
        return *this;
    }
};


// ============================================================
// Default construction
// ============================================================

void test_default_construction() {
    Stack<int> s;

    assert(s.empty());
    assert(s.size() == 0);
}


// ============================================================
// Basic push / pop / top
// ============================================================

void test_basic() {
    Stack<int> s;

    s.push(10);
    s.push(20);
    s.push(30);

    assert(!s.empty());
    assert(s.size() == 3);

    assert(s.top() == 30);

    s.pop();
    assert(s.top() == 20);
    assert(s.size() == 2);

    s.pop();
    assert(s.top() == 10);

    s.pop();

    assert(s.empty());
    assert(s.size() == 0);
}


// ============================================================
// LIFO behavior
// ============================================================

void test_lifo() {
    Stack<int> s;

    for (int i = 0; i < 1000; ++i)
        s.push(i);

    assert(s.size() == 1000);

    for (int i = 999; i >= 0; --i) {
        assert(s.top() == i);
        s.pop();
    }

    assert(s.empty());
}


// ============================================================
// top() mutation
// ============================================================

void test_top_mutation() {
    Stack<int> s;

    s.push(10);
    s.push(20);

    s.top() = 100;

    assert(s.top() == 100);

    s.pop();

    assert(s.top() == 10);
}


// ============================================================
// const top()
// ============================================================

void test_const_top() {
    Stack<int> s;

    s.push(10);
    s.push(20);

    const Stack<int>& cs = s;

    assert(cs.size() == 2);
    assert(!cs.empty());
    assert(cs.top() == 20);
}


// ============================================================
// Copy push
// ============================================================

void test_copy_push() {
    Stack<std::string> s;

    std::string value = "hello";

    s.push(value);

    assert(value == "hello");
    assert(s.top() == "hello");
}


// ============================================================
// Move push
// ============================================================

void test_move_push() {
    Stack<std::string> s;

    std::string value = "hello";

    s.push(std::move(value));

    assert(s.top() == "hello");
}


// ============================================================
// emplace
// ============================================================

void test_emplace() {
    Stack<std::pair<int, std::string>> s;

    s.emplace(10, "hello");
    s.emplace(20, "world");

    assert(s.size() == 2);

    assert(s.top().first == 20);
    assert(s.top().second == "world");

    s.pop();

    assert(s.top().first == 10);
    assert(s.top().second == "hello");
}


// ============================================================
// Construct from copied container
// ============================================================

void test_container_copy_constructor() {
    Deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    Stack<int> s(d);

    assert(s.size() == 3);
    assert(s.top() == 3);

    // Make sure Stack owns a copy.
    d.back() = 100;

    assert(s.top() == 3);
}


// ============================================================
// Construct from moved container
// ============================================================

void test_container_move_constructor() {
    Deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    Stack<int> s(std::move(d));

    assert(s.size() == 3);
    assert(s.top() == 3);

    assert(d.empty());
}


// ============================================================
// Stack copy constructor
// ============================================================

void test_copy_constructor() {
    Stack<int> original;

    for (int i = 0; i < 100; ++i)
        original.push(i);

    Stack<int> copy(original);

    assert(copy.size() == original.size());
    assert(copy.top() == original.top());

    copy.pop();

    assert(copy.size() == 99);
    assert(original.size() == 100);

    assert(copy.top() == 98);
    assert(original.top() == 99);
}


// ============================================================
// Stack copy assignment
// ============================================================

void test_copy_assignment() {
    Stack<int> a;
    Stack<int> b;

    for (int i = 0; i < 100; ++i)
        a.push(i);

    b.push(1000);
    b.push(2000);

    b = a;

    assert(b.size() == a.size());
    assert(b.top() == a.top());

    b.pop();

    assert(b.size() == 99);
    assert(a.size() == 100);
}


// ============================================================
// Self copy assignment
// ============================================================

void test_self_copy_assignment() {
    Stack<int> s;

    for (int i = 0; i < 100; ++i)
        s.push(i);

    s = s;

    assert(s.size() == 100);
    assert(s.top() == 99);
}


// ============================================================
// Stack move constructor
// ============================================================

void test_move_constructor() {
    Stack<int> original;

    for (int i = 0; i < 100; ++i)
        original.push(i);

    Stack<int> moved(std::move(original));

    assert(moved.size() == 100);
    assert(moved.top() == 99);

    assert(original.empty());
}


// ============================================================
// Stack move assignment
// ============================================================

void test_move_assignment() {
    Stack<int> a;
    Stack<int> b;

    for (int i = 0; i < 100; ++i)
        a.push(i);

    b.push(1000);

    b = std::move(a);

    assert(b.size() == 100);
    assert(b.top() == 99);

    assert(a.empty());
}


// ============================================================
// Swap
// ============================================================

void test_swap() {
    Stack<int> a;
    Stack<int> b;

    a.push(1);
    a.push(2);

    b.push(10);
    b.push(20);
    b.push(30);

    a.swap(b);

    assert(a.size() == 3);
    assert(a.top() == 30);

    assert(b.size() == 2);
    assert(b.top() == 2);
}


// ============================================================
// Equality
// ============================================================

void test_equality() {
    Stack<int> a;
    Stack<int> b;

    for (int i = 0; i < 10; ++i) {
        a.push(i);
        b.push(i);
    }

    assert(a == b);
    assert(!(a != b));

    b.pop();

    assert(a != b);
    assert(!(a == b));
}


// ============================================================
// Relational comparisons
// ============================================================

void test_comparisons() {
    Stack<int> a;
    Stack<int> b;
    Stack<int> c;

    a.push(1);
    a.push(2);

    b.push(1);
    b.push(3);

    c.push(1);
    c.push(2);
    c.push(0);

    assert(a < b);
    assert(b > a);

    assert(a < c);
    assert(c > a);

    assert(a <= b);
    assert(a <= a);

    assert(b >= a);
    assert(a >= a);
}


// ============================================================
// Strings
// ============================================================

void test_strings() {
    Stack<std::string> s;

    s.push("one");
    s.push("two");
    s.emplace("three");

    assert(s.top() == "three");

    s.pop();
    assert(s.top() == "two");

    s.pop();
    assert(s.top() == "one");

    s.pop();

    assert(s.empty());
}


// ============================================================
// Move-only type
// ============================================================

void test_move_only() {
    Stack<MoveOnly> s;

    s.push(MoveOnly(10));
    s.emplace(20);
    s.emplace(30);

    assert(s.size() == 3);
    assert(s.top().value == 30);

    s.pop();

    assert(s.top().value == 20);

    s.pop();

    assert(s.top().value == 10);
}


// ============================================================
// Compare with std::stack
// ============================================================

void test_vs_std_stack() {
    Stack<int> mine;
    std::stack<int> standard;

    for (int i = 0; i < 10000; ++i) {
        mine.push(i);
        standard.push(i);
    }

    assert(mine.size() == standard.size());

    while (!standard.empty()) {
        assert(!mine.empty());
        assert(mine.top() == standard.top());

        mine.pop();
        standard.pop();
    }

    assert(mine.empty());
}


// ============================================================
// Randomized stress against std::stack
// ============================================================

void test_random_stress() {
    Stack<int> mine;
    std::stack<int> standard;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> operation(0, 2);
    std::uniform_int_distribution<int> value(-100000, 100000);

    constexpr int OPERATIONS = 100000;

    for (int i = 0; i < OPERATIONS; ++i) {
        int op = operation(rng);

        if (op == 0 && !standard.empty()) {
            assert(mine.top() == standard.top());

            mine.pop();
            standard.pop();
        }
        else {
            int v = value(rng);

            mine.push(v);
            standard.push(v);
        }

        assert(mine.size() == standard.size());
        assert(mine.empty() == standard.empty());

        if (!standard.empty())
            assert(mine.top() == standard.top());
    }
}


// ============================================================
// Large stress
// ============================================================

void test_large_stress() {
    constexpr int N = 100000;

    Stack<int> s;

    for (int i = 0; i < N; ++i)
        s.push(i);

    assert(s.size() == N);

    for (int i = N - 1; i >= 0; --i) {
        assert(s.top() == i);
        s.pop();
    }

    assert(s.empty());
}


// ============================================================
// main
// ============================================================

int main() {
    test_default_construction();

    test_basic();
    test_lifo();

    test_top_mutation();
    test_const_top();

    test_copy_push();
    test_move_push();
    test_emplace();

    test_container_copy_constructor();
    test_container_move_constructor();

    test_copy_constructor();
    test_copy_assignment();
    test_self_copy_assignment();

    test_move_constructor();
    test_move_assignment();

    test_swap();

    test_equality();
    test_comparisons();

    test_strings();
    test_move_only();

    test_vs_std_stack();
    test_random_stress();
    test_large_stress();

    std::cout << "All Stack tests passed!\n";

    return 0;
}
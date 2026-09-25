#include <cassert>
#include <queue>
#include <string>
#include <utility>
#include <random>
#include <iostream>

#include "../include/ministl/containers/queue.hpp"

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
    Queue<int> q;

    assert(q.empty());
    assert(q.size() == 0);
}


// ============================================================
// Basic operations
// ============================================================

void test_basic() {
    Queue<int> q;

    q.push(10);
    q.push(20);
    q.push(30);

    assert(!q.empty());
    assert(q.size() == 3);

    assert(q.front() == 10);
    assert(q.back() == 30);

    q.pop();

    assert(q.size() == 2);
    assert(q.front() == 20);
    assert(q.back() == 30);

    q.pop();

    assert(q.front() == 30);
    assert(q.back() == 30);

    q.pop();

    assert(q.empty());
}


// ============================================================
// FIFO behavior
// ============================================================

void test_fifo() {
    Queue<int> q;

    for (int i = 0; i < 1000; ++i)
        q.push(i);

    assert(q.size() == 1000);

    for (int i = 0; i < 1000; ++i) {
        assert(q.front() == i);
        q.pop();
    }

    assert(q.empty());
}


// ============================================================
// front/back mutation
// ============================================================

void test_front_back_mutation() {
    Queue<int> q;

    q.push(10);
    q.push(20);
    q.push(30);

    q.front() = 100;
    q.back() = 300;

    assert(q.front() == 100);
    assert(q.back() == 300);

    q.pop();

    assert(q.front() == 20);
    assert(q.back() == 300);
}


// ============================================================
// Const access
// ============================================================

void test_const_access() {
    Queue<int> q;

    q.push(10);
    q.push(20);
    q.push(30);

    const Queue<int>& cq = q;

    assert(cq.size() == 3);
    assert(!cq.empty());

    assert(cq.front() == 10);
    assert(cq.back() == 30);
}


// ============================================================
// Copy push
// ============================================================

void test_copy_push() {
    Queue<std::string> q;

    std::string value = "hello";

    q.push(value);

    assert(value == "hello");
    assert(q.front() == "hello");
    assert(q.back() == "hello");
}


// ============================================================
// Move push
// ============================================================

void test_move_push() {
    Queue<std::string> q;

    std::string value = "hello";

    q.push(std::move(value));

    assert(q.front() == "hello");
}


// ============================================================
// emplace
// ============================================================

void test_emplace() {
    Queue<std::pair<int, std::string>> q;

    q.emplace(10, "hello");
    q.emplace(20, "world");

    assert(q.size() == 2);

    assert(q.front().first == 10);
    assert(q.front().second == "hello");

    assert(q.back().first == 20);
    assert(q.back().second == "world");

    q.pop();

    assert(q.front().first == 20);
    assert(q.front().second == "world");
}


// ============================================================
// Construct from copied container
// ============================================================

void test_container_copy_constructor() {
    Deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    Queue<int> q(d);

    assert(q.size() == 3);
    assert(q.front() == 1);
    assert(q.back() == 3);

    d.front() = 100;

    assert(q.front() == 1);
}


// ============================================================
// Construct from moved container
// ============================================================

void test_container_move_constructor() {
    Deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    Queue<int> q(std::move(d));

    assert(q.size() == 3);
    assert(q.front() == 1);
    assert(q.back() == 3);

    assert(d.empty());
}


// ============================================================
// Queue copy constructor
// ============================================================

void test_copy_constructor() {
    Queue<int> original;

    for (int i = 0; i < 100; ++i)
        original.push(i);

    Queue<int> copy(original);

    assert(copy.size() == original.size());

    assert(copy.front() == original.front());
    assert(copy.back() == original.back());

    copy.pop();

    assert(copy.front() == 1);
    assert(original.front() == 0);

    assert(copy.size() == 99);
    assert(original.size() == 100);
}


// ============================================================
// Queue copy assignment
// ============================================================

void test_copy_assignment() {
    Queue<int> a;
    Queue<int> b;

    for (int i = 0; i < 100; ++i)
        a.push(i);

    b.push(1000);
    b.push(2000);

    b = a;

    assert(b.size() == a.size());

    assert(b.front() == 0);
    assert(b.back() == 99);

    b.pop();

    assert(b.front() == 1);
    assert(a.front() == 0);
}


// ============================================================
// Self copy assignment
// ============================================================

void test_self_copy_assignment() {
    Queue<int> q;

    for (int i = 0; i < 100; ++i)
        q.push(i);

    q = q;

    assert(q.size() == 100);
    assert(q.front() == 0);
    assert(q.back() == 99);
}


// ============================================================
// Queue move constructor
// ============================================================

void test_move_constructor() {
    Queue<int> original;

    for (int i = 0; i < 100; ++i)
        original.push(i);

    Queue<int> moved(std::move(original));

    assert(moved.size() == 100);
    assert(moved.front() == 0);
    assert(moved.back() == 99);

    assert(original.empty());
}


// ============================================================
// Queue move assignment
// ============================================================

void test_move_assignment() {
    Queue<int> a;
    Queue<int> b;

    for (int i = 0; i < 100; ++i)
        a.push(i);

    b.push(1000);

    b = std::move(a);

    assert(b.size() == 100);
    assert(b.front() == 0);
    assert(b.back() == 99);

    assert(a.empty());
}


// ============================================================
// Swap
// ============================================================

void test_swap() {
    Queue<int> a;
    Queue<int> b;

    a.push(1);
    a.push(2);

    b.push(10);
    b.push(20);
    b.push(30);

    a.swap(b);

    assert(a.size() == 3);
    assert(a.front() == 10);
    assert(a.back() == 30);

    assert(b.size() == 2);
    assert(b.front() == 1);
    assert(b.back() == 2);
}


// ============================================================
// Equality
// ============================================================

void test_equality() {
    Queue<int> a;
    Queue<int> b;

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
    Queue<int> a;
    Queue<int> b;
    Queue<int> c;

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
    Queue<std::string> q;

    q.push("one");
    q.push("two");
    q.emplace("three");

    assert(q.front() == "one");
    assert(q.back() == "three");

    q.pop();

    assert(q.front() == "two");

    q.pop();

    assert(q.front() == "three");

    q.pop();

    assert(q.empty());
}


// ============================================================
// Move-only type
// ============================================================

void test_move_only() {
    Queue<MoveOnly> q;

    q.push(MoveOnly(10));
    q.emplace(20);
    q.emplace(30);

    assert(q.size() == 3);

    assert(q.front().value == 10);
    assert(q.back().value == 30);

    q.pop();

    assert(q.front().value == 20);

    q.pop();

    assert(q.front().value == 30);
}


// ============================================================
// Repeated push/pop
// Exercises underlying Deque wraparound through Queue
// ============================================================

void test_repeated_push_pop() {
    Queue<int> q;

    for (int i = 0; i < 100; ++i)
        q.push(i);

    for (int round = 0; round < 10000; ++round) {
        int current = q.front();

        q.pop();
        q.push(current + 100);

        assert(q.size() == 100);
    }
}


// ============================================================
// Compare with std::queue
// ============================================================

void test_vs_std_queue() {
    Queue<int> mine;
    std::queue<int> standard;

    for (int i = 0; i < 10000; ++i) {
        mine.push(i);
        standard.push(i);
    }

    assert(mine.size() == standard.size());

    while (!standard.empty()) {
        assert(!mine.empty());

        assert(mine.front() == standard.front());
        assert(mine.back() == standard.back());

        mine.pop();
        standard.pop();
    }

    assert(mine.empty());
}


// ============================================================
// Randomized stress against std::queue
// ============================================================

void test_random_stress() {
    Queue<int> mine;
    std::queue<int> standard;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> operation(0, 2);
    std::uniform_int_distribution<int> value(-100000, 100000);

    constexpr int OPERATIONS = 100000;

    for (int i = 0; i < OPERATIONS; ++i) {
        int op = operation(rng);

        if (op == 0 && !standard.empty()) {
            assert(mine.front() == standard.front());

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

        if (!standard.empty()) {
            assert(mine.front() == standard.front());
            assert(mine.back() == standard.back());
        }
    }
}


// ============================================================
// Large stress
// ============================================================

void test_large_stress() {
    constexpr int N = 100000;

    Queue<int> q;

    for (int i = 0; i < N; ++i)
        q.push(i);

    assert(q.size() == N);

    for (int i = 0; i < N; ++i) {
        assert(q.front() == i);
        q.pop();
    }

    assert(q.empty());
}


// ============================================================
// main
// ============================================================

int main() {
    test_default_construction();

    test_basic();
    test_fifo();

    test_front_back_mutation();
    test_const_access();

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

    test_repeated_push_pop();

    test_vs_std_queue();
    test_random_stress();
    test_large_stress();

    std::cout << "All Queue tests passed!\n";

    return 0;
}
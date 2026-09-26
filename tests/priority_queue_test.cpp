#include <cassert>
#include <queue>
#include <vector>
#include <functional>
#include <string>
#include <random>
#include <iostream>
#include <utility>

#include "../include/ministl/containers/priority_queue.hpp"

using namespace ministl;

/** AI GENERATED TESTS **/

// ============================================================
// Custom type
// ============================================================

struct Item {
    int priority;
    std::string name;

    Item(int p, std::string n)
            : priority(p), name(std::move(n)) {}
};


// ============================================================
// Custom comparator
// ============================================================

struct ItemCompare {

    bool operator()(const Item& a, const Item& b) const {
        return a.priority < b.priority;
    }
};


// ============================================================
// Move-only type
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


struct MoveOnlyCompare {

    bool operator()(const MoveOnly& a, const MoveOnly& b) const {
        return a.value < b.value;
    }
};


// ============================================================
// Default construction
// ============================================================

void test_default_construction() {

    PriorityQueue<int> pq;

    assert(pq.empty());
    assert(pq.size() == 0);
}


// ============================================================
// Basic push / top / pop
// ============================================================

void test_basic() {

    PriorityQueue<int> pq;

    pq.push(10);
    assert(pq.top() == 10);

    pq.push(30);
    assert(pq.top() == 30);

    pq.push(20);
    assert(pq.top() == 30);

    assert(pq.size() == 3);

    pq.pop();

    assert(pq.top() == 20);
    assert(pq.size() == 2);

    pq.pop();

    assert(pq.top() == 10);

    pq.pop();

    assert(pq.empty());
}


// ============================================================
// Max-heap ordering
// ============================================================

void test_max_heap_order() {

    PriorityQueue<int> pq;

    int values[] = {
            40, 10, 90, 20, 70,
            100, 30, 60, 50, 80
    };

    for (int value : values)
        pq.push(value);

    for (int expected = 100; expected >= 10; expected -= 10) {

        assert(pq.top() == expected);

        pq.pop();
    }

    assert(pq.empty());
}


// ============================================================
// Duplicates
// ============================================================

void test_duplicates() {

    PriorityQueue<int> pq;

    pq.push(10);
    pq.push(30);
    pq.push(30);
    pq.push(20);
    pq.push(30);
    pq.push(10);

    assert(pq.size() == 6);

    assert(pq.top() == 30);
    pq.pop();

    assert(pq.top() == 30);
    pq.pop();

    assert(pq.top() == 30);
    pq.pop();

    assert(pq.top() == 20);
    pq.pop();

    assert(pq.top() == 10);
    pq.pop();

    assert(pq.top() == 10);
    pq.pop();

    assert(pq.empty());
}


// ============================================================
// Negative numbers
// ============================================================

void test_negative_numbers() {

    PriorityQueue<int> pq;

    pq.push(-100);
    pq.push(-20);
    pq.push(-300);
    pq.push(-1);
    pq.push(-50);

    assert(pq.top() == -1);

    pq.pop();
    assert(pq.top() == -20);

    pq.pop();
    assert(pq.top() == -50);

    pq.pop();
    assert(pq.top() == -100);

    pq.pop();
    assert(pq.top() == -300);
}


// ============================================================
// Mixed values
// ============================================================

void test_mixed_values() {

    PriorityQueue<int> pq;

    pq.push(0);
    pq.push(-10);
    pq.push(100);
    pq.push(-100);
    pq.push(50);

    assert(pq.top() == 100);

    pq.pop();
    assert(pq.top() == 50);

    pq.pop();
    assert(pq.top() == 0);

    pq.pop();
    assert(pq.top() == -10);

    pq.pop();
    assert(pq.top() == -100);
}


// ============================================================
// Min heap
// ============================================================

void test_min_heap() {

    PriorityQueue<
            int,
            Vector<int>,
            std::greater<int>
    > pq;

    pq.push(50);
    pq.push(20);
    pq.push(70);
    pq.push(10);
    pq.push(40);

    assert(pq.top() == 10);

    pq.pop();
    assert(pq.top() == 20);

    pq.pop();
    assert(pq.top() == 40);

    pq.pop();
    assert(pq.top() == 50);

    pq.pop();
    assert(pq.top() == 70);

    pq.pop();

    assert(pq.empty());
}


// ============================================================
// Emplace
// ============================================================

void test_emplace() {

    PriorityQueue<
            Item,
            Vector<Item>,
            ItemCompare
    > pq;

    pq.emplace(10, "low");
    pq.emplace(100, "high");
    pq.emplace(50, "medium");

    assert(pq.size() == 3);

    assert(pq.top().priority == 100);
    assert(pq.top().name == "high");

    pq.pop();

    assert(pq.top().priority == 50);
    assert(pq.top().name == "medium");

    pq.pop();

    assert(pq.top().priority == 10);
    assert(pq.top().name == "low");
}


// ============================================================
// Copy push
// ============================================================

void test_copy_push() {

    PriorityQueue<std::string> pq;

    std::string value = "hello";

    pq.push(value);

    assert(value == "hello");
    assert(pq.top() == "hello");
}


// ============================================================
// Move push
// ============================================================

void test_move_push() {

    PriorityQueue<std::string> pq;

    std::string value = "hello";

    pq.push(std::move(value));

    assert(pq.top() == "hello");
}


// ============================================================
// Strings
// ============================================================

void test_strings() {

    PriorityQueue<std::string> pq;

    pq.push("banana");
    pq.push("apple");
    pq.push("orange");
    pq.push("grape");

    assert(pq.top() == "orange");

    pq.pop();

    assert(pq.top() == "grape");

    pq.pop();

    assert(pq.top() == "banana");

    pq.pop();

    assert(pq.top() == "apple");
}


// ============================================================
// Container copy constructor
// ============================================================

void test_container_copy_constructor() {

    Vector<int> values;

    values.push_back(20);
    values.push_back(100);
    values.push_back(10);
    values.push_back(70);
    values.push_back(50);

    PriorityQueue<int> pq(
            std::less<int>{},
            values
    );

    assert(pq.size() == 5);
    assert(pq.top() == 100);

    // Original container must still exist independently.
    assert(values.size() == 5);

    values[0] = 1000;

    assert(pq.top() == 100);
}


// ============================================================
// Container move constructor
// ============================================================

void test_container_move_constructor() {

    Vector<int> values;

    values.push_back(20);
    values.push_back(100);
    values.push_back(10);
    values.push_back(70);
    values.push_back(50);

    PriorityQueue<int> pq(
            std::less<int>{},
            std::move(values)
    );

    assert(pq.size() == 5);
    assert(pq.top() == 100);

    assert(values.empty());
}


// ============================================================
// build_heap correctness
// ============================================================

void test_build_heap() {

    Vector<int> values;

    for (int i = 0; i < 1000; ++i)
        values.push_back(i);

    PriorityQueue<int> pq(
            std::less<int>{},
            std::move(values)
    );

    assert(pq.size() == 1000);

    for (int expected = 999; expected >= 0; --expected) {

        assert(pq.top() == expected);

        pq.pop();
    }

    assert(pq.empty());
}


// ============================================================
// Copy constructor
// ============================================================

void test_copy_constructor() {

    PriorityQueue<int> original;

    for (int i = 0; i < 100; ++i)
        original.push(i);

    PriorityQueue<int> copy(original);

    assert(copy.size() == original.size());
    assert(copy.top() == original.top());

    copy.pop();

    assert(copy.size() == 99);
    assert(original.size() == 100);

    assert(copy.top() == 98);
    assert(original.top() == 99);
}


// ============================================================
// Copy assignment
// ============================================================

void test_copy_assignment() {

    PriorityQueue<int> a;
    PriorityQueue<int> b;

    for (int i = 0; i < 100; ++i)
        a.push(i);

    b.push(1000);
    b.push(2000);

    b = a;

    assert(b.size() == a.size());
    assert(b.top() == a.top());

    b.pop();

    assert(b.top() == 98);
    assert(a.top() == 99);
}


// ============================================================
// Self copy assignment
// ============================================================

void test_self_copy_assignment() {

    PriorityQueue<int> pq;

    for (int i = 0; i < 100; ++i)
        pq.push(i);

    pq = pq;

    assert(pq.size() == 100);
    assert(pq.top() == 99);
}


// ============================================================
// Move constructor
// ============================================================

void test_move_constructor() {

    PriorityQueue<int> original;

    for (int i = 0; i < 100; ++i)
        original.push(i);

    PriorityQueue<int> moved(std::move(original));

    assert(moved.size() == 100);
    assert(moved.top() == 99);

    assert(original.empty());
}


// ============================================================
// Move assignment
// ============================================================

void test_move_assignment() {

    PriorityQueue<int> a;
    PriorityQueue<int> b;

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

    PriorityQueue<int> a;
    PriorityQueue<int> b;

    a.push(10);
    a.push(20);
    a.push(30);

    b.push(100);
    b.push(200);

    a.swap(b);

    assert(a.size() == 2);
    assert(a.top() == 200);

    assert(b.size() == 3);
    assert(b.top() == 30);
}


// ============================================================
// Move-only type
// ============================================================

void test_move_only() {

    PriorityQueue<
            MoveOnly,
            Vector<MoveOnly>,
            MoveOnlyCompare
    > pq;

    pq.push(MoveOnly(10));
    pq.emplace(50);
    pq.emplace(30);
    pq.push(MoveOnly(100));

    assert(pq.size() == 4);

    assert(pq.top().value == 100);

    pq.pop();
    assert(pq.top().value == 50);

    pq.pop();
    assert(pq.top().value == 30);

    pq.pop();
    assert(pq.top().value == 10);

    pq.pop();

    assert(pq.empty());
}


// ============================================================
// Compare against std::priority_queue
// ============================================================

void test_vs_std_priority_queue() {

    PriorityQueue<int> mine;
    std::priority_queue<int> standard;

    for (int i = 0; i < 10000; ++i) {

        int value = (i * 37) % 10007;

        mine.push(value);
        standard.push(value);
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
// Min heap against std::priority_queue
// ============================================================

void test_min_heap_vs_std() {

    PriorityQueue<
            int,
            Vector<int>,
            std::greater<int>
    > mine;

    std::priority_queue<
            int,
            std::vector<int>,
            std::greater<int>
    > standard;

    for (int i = 0; i < 10000; ++i) {

        int value = (i * 73) % 20011;

        mine.push(value);
        standard.push(value);
    }

    while (!standard.empty()) {

        assert(mine.top() == standard.top());

        mine.pop();
        standard.pop();
    }

    assert(mine.empty());
}


// ============================================================
// Randomized stress against std::priority_queue
// ============================================================

void test_random_stress() {

    PriorityQueue<int> mine;
    std::priority_queue<int> standard;

    std::mt19937 rng(42);

    std::uniform_int_distribution<int> operation(0, 2);
    std::uniform_int_distribution<int> value(
            -1000000,
            1000000
    );

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


    // Compare everything that remains.
    while (!standard.empty()) {

        assert(mine.top() == standard.top());

        mine.pop();
        standard.pop();
    }

    assert(mine.empty());
}


// ============================================================
// Large sequential stress
// ============================================================

void test_large_stress() {

    constexpr int N = 100000;

    PriorityQueue<int> pq;

    for (int i = 0; i < N; ++i)
        pq.push(i);

    assert(pq.size() == N);
    assert(pq.top() == N - 1);

    for (int expected = N - 1; expected >= 0; --expected) {

        assert(pq.top() == expected);

        pq.pop();
    }

    assert(pq.empty());
}


// ============================================================
// Repeated push/pop
// ============================================================

void test_repeated_push_pop() {

    PriorityQueue<int> pq;

    for (int round = 0; round < 10000; ++round) {

        pq.push(round);
        pq.push(round + 100);
        pq.push(round + 50);

        assert(pq.top() == round + 100);

        pq.pop();
        pq.pop();
        pq.pop();

        assert(pq.empty());
    }
}


// ============================================================
// main
// ============================================================

int main() {

    test_default_construction();

    test_basic();
    test_max_heap_order();

    test_duplicates();
    test_negative_numbers();
    test_mixed_values();

    test_min_heap();

    test_emplace();

    test_copy_push();
    test_move_push();

    test_strings();

    test_container_copy_constructor();
    test_container_move_constructor();

    test_build_heap();

    test_copy_constructor();
    test_copy_assignment();
    test_self_copy_assignment();

    test_move_constructor();
    test_move_assignment();

    test_swap();

    test_move_only();

    test_vs_std_priority_queue();
    test_min_heap_vs_std();

    test_random_stress();
    test_large_stress();
    test_repeated_push_pop();

    std::cout << "All PriorityQueue tests passed!\n";

    return 0;
}
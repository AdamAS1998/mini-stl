#include <cassert>
#include <deque>
#include <string>
#include <utility>
#include <random>
#include <iostream>
#include <type_traits>
#include <algorithm>

#include "../include/ministl/containers/deque.hpp"

using namespace ministl;

/** AI GENERATED TESTS **/

// ============================================================
// Helper types
// ============================================================

struct Tracker {
    static int alive;
    static int constructed;
    static int destroyed;
    static int copied;
    static int moved;

    int value;

    explicit Tracker(int v = 0)
            : value(v) {
        ++alive;
        ++constructed;
    }

    Tracker(const Tracker& other)
            : value(other.value) {
        ++alive;
        ++constructed;
        ++copied;
    }

    Tracker(Tracker&& other) noexcept
            : value(other.value) {
        other.value = -1;

        ++alive;
        ++constructed;
        ++moved;
    }

    Tracker& operator=(const Tracker& other) {
        value = other.value;
        ++copied;
        return *this;
    }

    Tracker& operator=(Tracker&& other) noexcept {
        value = other.value;
        other.value = -1;
        ++moved;
        return *this;
    }

    ~Tracker() {
        --alive;
        ++destroyed;
    }

    bool operator==(const Tracker& other) const {
        return value == other.value;
    }

    bool operator<(const Tracker& other) const {
        return value < other.value;
    }

    static void reset() {
        alive = 0;
        constructed = 0;
        destroyed = 0;
        copied = 0;
        moved = 0;
    }
};

int Tracker::alive = 0;
int Tracker::constructed = 0;
int Tracker::destroyed = 0;
int Tracker::copied = 0;
int Tracker::moved = 0;


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
// Basic construction
// ============================================================

void test_default_construction() {
    Deque<int> d;

    assert(d.empty());
    assert(d.size() == 0);
    assert(d.capacity() == 0);

    assert(d.begin() == d.end());
    assert(d.cbegin() == d.cend());
}


// ============================================================
// push_back
// ============================================================

void test_push_back() {
    Deque<int> d;

    d.push_back(10);
    d.push_back(20);
    d.push_back(30);

    assert(!d.empty());
    assert(d.size() == 3);

    assert(d[0] == 10);
    assert(d[1] == 20);
    assert(d[2] == 30);

    assert(d.front() == 10);
    assert(d.back() == 30);
}


// ============================================================
// push_front
// ============================================================

void test_push_front() {
    Deque<int> d;

    d.push_front(10);
    d.push_front(20);
    d.push_front(30);

    assert(d.size() == 3);

    assert(d[0] == 30);
    assert(d[1] == 20);
    assert(d[2] == 10);

    assert(d.front() == 30);
    assert(d.back() == 10);
}


// ============================================================
// Mixed push front/back
// ============================================================

void test_mixed_push() {
    Deque<int> d;

    d.push_back(2);
    d.push_back(3);

    d.push_front(1);
    d.push_front(0);

    d.push_back(4);

    assert(d.size() == 5);

    for (int i = 0; i < 5; ++i) {
        assert(d[i] == i);
    }
}


// ============================================================
// pop_back
// ============================================================

void test_pop_back() {
    Deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    d.pop_back();

    assert(d.size() == 2);
    assert(d.back() == 2);

    d.pop_back();

    assert(d.size() == 1);
    assert(d.back() == 1);

    d.pop_back();

    assert(d.empty());
}


// ============================================================
// pop_front
// ============================================================

void test_pop_front() {
    Deque<int> d;

    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    d.pop_front();

    assert(d.size() == 2);
    assert(d.front() == 2);

    d.pop_front();

    assert(d.size() == 1);
    assert(d.front() == 3);

    d.pop_front();

    assert(d.empty());
}


// ============================================================
// Mixed pop/push
// ============================================================

void test_mixed_operations() {
    Deque<int> d;

    d.push_back(2);
    d.push_front(1);
    d.push_back(3);
    d.push_front(0);

    assert(d.size() == 4);

    d.pop_front();

    assert(d.front() == 1);

    d.pop_back();

    assert(d.back() == 2);

    d.push_back(5);
    d.push_front(4);

    assert(d.size() == 4);

    assert(d[0] == 4);
    assert(d[1] == 1);
    assert(d[2] == 2);
    assert(d[3] == 5);
}


// ============================================================
// operator[] and mutation
// ============================================================

void test_index_access() {
    Deque<int> d;

    for (int i = 0; i < 20; ++i)
        d.push_back(i);

    for (int i = 0; i < 20; ++i)
        assert(d[i] == i);

    d[5] = 100;
    d[10] = 200;

    assert(d[5] == 100);
    assert(d[10] == 200);
}


// ============================================================
// at()
// ============================================================

void test_at() {
    Deque<int> d;

    d.push_back(10);
    d.push_back(20);
    d.push_back(30);

    assert(d.at(0) == 10);
    assert(d.at(1) == 20);
    assert(d.at(2) == 30);

    d.at(1) = 99;
    assert(d[1] == 99);

    bool thrown = false;

    try {
        d.at(3);
    }
    catch (const std::out_of_range&) {
        thrown = true;
    }

    assert(thrown);
}


// ============================================================
// Const access
// ============================================================

void test_const_access() {
    Deque<int> d;

    d.push_back(10);
    d.push_back(20);
    d.push_back(30);

    const Deque<int>& cd = d;

    assert(cd.size() == 3);
    assert(!cd.empty());

    assert(cd[0] == 10);
    assert(cd.at(1) == 20);

    assert(cd.front() == 10);
    assert(cd.back() == 30);
}


// ============================================================
// Clear
// ============================================================

void test_clear() {
    Deque<int> d;

    for (int i = 0; i < 100; ++i)
        d.push_back(i);

    const auto old_capacity = d.capacity();

    d.clear();

    assert(d.empty());
    assert(d.size() == 0);

    // clear keeps allocated storage
    assert(d.capacity() == old_capacity);

    // deque should still be usable
    d.push_back(100);
    d.push_front(50);

    assert(d.size() == 2);
    assert(d.front() == 50);
    assert(d.back() == 100);
}


// ============================================================
// Capacity growth
// ============================================================

void test_growth() {
    Deque<int> d;

    std::size_t previous_capacity = d.capacity();

    for (int i = 0; i < 1000; ++i) {
        d.push_back(i);

        assert(d.capacity() >= d.size());

        if (d.capacity() != previous_capacity) {
            assert(d.capacity() > previous_capacity);
            previous_capacity = d.capacity();
        }
    }

    assert(d.size() == 1000);

    for (int i = 0; i < 1000; ++i)
        assert(d[i] == i);
}


// ============================================================
// Circular buffer wraparound
// ============================================================

void test_wraparound() {
    Deque<int> d;

    for (int i = 0; i < 16; ++i)
        d.push_back(i);

    // Move logical front forward.
    for (int i = 0; i < 10; ++i) {
        assert(d.front() == i);
        d.pop_front();
    }

    // These should eventually wrap around the physical array.
    for (int i = 16; i < 26; ++i)
        d.push_back(i);

    assert(d.size() == 16);

    for (int i = 0; i < 16; ++i)
        assert(d[i] == i + 10);

    assert(d.front() == 10);
    assert(d.back() == 25);
}


// ============================================================
// Wraparound from push_front
// ============================================================

void test_front_wraparound() {
    Deque<int> d;

    for (int i = 0; i < 16; ++i)
        d.push_back(i);

    for (int i = 0; i < 10; ++i)
        d.pop_back();

    for (int i = 1; i <= 10; ++i)
        d.push_front(-i);

    assert(d.size() == 16);

    for (int i = 0; i < 10; ++i)
        assert(d[i] == -(10 - i));

    for (int i = 0; i < 6; ++i)
        assert(d[i + 10] == i);
}


// ============================================================
// Growth while wrapped
// ============================================================

void test_growth_after_wraparound() {
    Deque<int> d;

    // Reach capacity 16.
    for (int i = 0; i < 16; ++i)
        d.push_back(i);

    // Shift front into middle of buffer.
    for (int i = 0; i < 8; ++i)
        d.pop_front();

    // Fill freed physical slots by wrapping.
    for (int i = 16; i < 24; ++i)
        d.push_back(i);

    assert(d.size() == 16);

    // Next push forces growth while logical data is physically wrapped.
    d.push_back(24);

    assert(d.size() == 17);
    assert(d.front() == 8);
    assert(d.back() == 24);

    for (int i = 0; i < 17; ++i)
        assert(d[i] == i + 8);
}


// ============================================================
// Repeated wraparound
// ============================================================

void test_repeated_wraparound() {
    Deque<int> d;

    for (int i = 0; i < 100; ++i)
        d.push_back(i);

    for (int round = 0; round < 1000; ++round) {
        int expected = d.front();

        d.pop_front();
        d.push_back(expected + 100);

        assert(d.size() == 100);
    }
}


// ============================================================
// Iterator forward traversal
// ============================================================

void test_iterator_forward() {
    Deque<int> d;

    for (int i = 0; i < 20; ++i)
        d.push_back(i);

    int expected = 0;

    for (auto it = d.begin(); it != d.end(); ++it) {
        assert(*it == expected);
        ++expected;
    }

    assert(expected == 20);
}


// ============================================================
// Iterator mutation
// ============================================================

void test_iterator_mutation() {
    Deque<int> d;

    for (int i = 0; i < 10; ++i)
        d.push_back(i);

    for (auto it = d.begin(); it != d.end(); ++it)
        *it *= 2;

    for (int i = 0; i < 10; ++i)
        assert(d[i] == i * 2);
}


// ============================================================
// Iterator prefix/postfix
// ============================================================

void test_iterator_increment_decrement() {
    Deque<int> d;

    d.push_back(10);
    d.push_back(20);
    d.push_back(30);

    auto it = d.begin();

    assert(*it == 10);

    auto old = it++;
    assert(*old == 10);
    assert(*it == 20);

    ++it;
    assert(*it == 30);

    auto old2 = it--;
    assert(*old2 == 30);
    assert(*it == 20);

    --it;
    assert(*it == 10);
}


// ============================================================
// Random access iterator operations
// ============================================================

void test_random_access_iterator() {
    Deque<int> d;

    for (int i = 0; i < 20; ++i)
        d.push_back(i * 10);

    auto it = d.begin();

    assert(*(it + 5) == 50);

    it += 10;
    assert(*it == 100);

    it -= 3;
    assert(*it == 70);

    assert(it[2] == 90);

    auto end = d.end();

    assert(end - d.begin() == 20);
    assert(it - d.begin() == 7);

    assert(d.begin() < d.end());
    assert(d.begin() <= d.end());
    assert(d.end() > d.begin());
    assert(d.end() >= d.begin());
}


// ============================================================
// Iterator on wrapped storage
// ============================================================

void test_iterator_wraparound() {
    Deque<int> d;

    for (int i = 0; i < 16; ++i)
        d.push_back(i);

    for (int i = 0; i < 10; ++i)
        d.pop_front();

    for (int i = 16; i < 26; ++i)
        d.push_back(i);

    int expected = 10;

    for (auto it = d.begin(); it != d.end(); ++it) {
        assert(*it == expected);
        ++expected;
    }

    assert(expected == 26);
}


// ============================================================
// const_iterator
// ============================================================

void test_const_iterator() {
    Deque<int> d;

    for (int i = 0; i < 10; ++i)
        d.push_back(i);

    const Deque<int>& cd = d;

    int expected = 0;

    for (auto it = cd.begin(); it != cd.end(); ++it) {
        assert(*it == expected);
        ++expected;
    }

    assert(expected == 10);

    auto it = cd.begin();

    assert(*(it + 5) == 5);

    it += 7;
    assert(*it == 7);

    it -= 2;
    assert(*it == 5);

    assert(cd.end() - cd.begin() == 10);
}


// ============================================================
// iterator -> const_iterator conversion
// ============================================================

void test_iterator_conversion() {
    static_assert(
            std::is_constructible_v<
                    Deque<int>::const_iterator,
                    Deque<int>::iterator
            >
    );

    static_assert(
            !std::is_constructible_v<
                    Deque<int>::iterator,
                    Deque<int>::const_iterator
            >
    );

    Deque<int> d;

    d.push_back(10);
    d.push_back(20);

    Deque<int>::iterator it = d.begin();
    Deque<int>::const_iterator cit = it;

    assert(*cit == 10);
}


// ============================================================
// STL algorithm compatibility
// ============================================================

void test_algorithm_compatibility() {
    Deque<int> d;

    d.push_back(5);
    d.push_back(1);
    d.push_back(4);
    d.push_back(2);
    d.push_back(3);

    std::sort(d.begin(), d.end());

    for (int i = 0; i < 5; ++i)
        assert(d[i] == i + 1);

    auto it = std::find(d.begin(), d.end(), 4);

    assert(it != d.end());
    assert(*it == 4);
}


// ============================================================
// Copy constructor
// ============================================================

void test_copy_constructor() {
    Deque<int> original;

    for (int i = 0; i < 100; ++i)
        original.push_back(i);

    Deque<int> copy(original);

    assert(copy.size() == original.size());

    for (std::size_t i = 0; i < original.size(); ++i)
        assert(copy[i] == original[i]);

    // Verify deep copy.
    copy[0] = 999;

    assert(copy[0] == 999);
    assert(original[0] == 0);
}


// ============================================================
// Copy wrapped deque
// ============================================================

void test_copy_wrapped_deque() {
    Deque<int> original;

    for (int i = 0; i < 16; ++i)
        original.push_back(i);

    for (int i = 0; i < 8; ++i)
        original.pop_front();

    for (int i = 16; i < 24; ++i)
        original.push_back(i);

    Deque<int> copy(original);

    assert(copy.size() == original.size());

    for (std::size_t i = 0; i < original.size(); ++i)
        assert(copy[i] == original[i]);
}


// ============================================================
// Copy assignment
// ============================================================

void test_copy_assignment() {
    Deque<int> a;
    Deque<int> b;

    for (int i = 0; i < 50; ++i)
        a.push_back(i);

    b.push_back(999);
    b.push_back(888);

    b = a;

    assert(b.size() == a.size());

    for (std::size_t i = 0; i < a.size(); ++i)
        assert(b[i] == a[i]);

    b[0] = 500;

    assert(a[0] == 0);
    assert(b[0] == 500);
}


// ============================================================
// Self copy assignment
// ============================================================

void test_self_copy_assignment() {
    Deque<int> d;

    for (int i = 0; i < 20; ++i)
        d.push_back(i);

    d = d;

    assert(d.size() == 20);

    for (int i = 0; i < 20; ++i)
        assert(d[i] == i);
}


// ============================================================
// Move constructor
// ============================================================

void test_move_constructor() {
    Deque<int> original;

    for (int i = 0; i < 50; ++i)
        original.push_back(i);

    Deque<int> moved(std::move(original));

    assert(moved.size() == 50);

    for (int i = 0; i < 50; ++i)
        assert(moved[i] == i);

    assert(original.empty());
    assert(original.size() == 0);
}


// ============================================================
// Move assignment
// ============================================================

void test_move_assignment() {
    Deque<int> a;
    Deque<int> b;

    for (int i = 0; i < 50; ++i)
        a.push_back(i);

    b.push_back(1000);
    b.push_back(2000);

    b = std::move(a);

    assert(b.size() == 50);

    for (int i = 0; i < 50; ++i)
        assert(b[i] == i);

    assert(a.empty());
}


// ============================================================
// Self move assignment
// ============================================================

void test_self_move_assignment() {
    Deque<int> d;

    for (int i = 0; i < 20; ++i)
        d.push_back(i);

    d = std::move(d);

    assert(d.size() == 20);

    for (int i = 0; i < 20; ++i)
        assert(d[i] == i);
}


// ============================================================
// Swap
// ============================================================

void test_swap() {
    Deque<int> a;
    Deque<int> b;

    a.push_back(1);
    a.push_back(2);

    b.push_back(10);
    b.push_back(20);
    b.push_back(30);

    a.swap(b);

    assert(a.size() == 3);
    assert(a[0] == 10);
    assert(a[1] == 20);
    assert(a[2] == 30);

    assert(b.size() == 2);
    assert(b[0] == 1);
    assert(b[1] == 2);
}


// ============================================================
// Equality / inequality
// ============================================================

void test_equality() {
    Deque<int> a;
    Deque<int> b;

    for (int i = 0; i < 10; ++i) {
        a.push_back(i);
        b.push_back(i);
    }

    assert(a == b);
    assert(!(a != b));

    b.pop_back();
    b.push_back(100);

    assert(a != b);
    assert(!(a == b));
}


// ============================================================
// Lexicographical comparisons
// ============================================================

void test_comparisons() {
    Deque<int> a;
    Deque<int> b;
    Deque<int> c;

    a.push_back(1);
    a.push_back(2);

    b.push_back(1);
    b.push_back(3);

    c.push_back(1);
    c.push_back(2);
    c.push_back(0);

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
// emplace_back
// ============================================================

void test_emplace_back() {
    Deque<std::pair<int, std::string>> d;

    auto& p = d.emplace_back(10, "hello");

    assert(d.size() == 1);

    assert(p.first == 10);
    assert(p.second == "hello");

    assert(d.back().first == 10);
    assert(d.back().second == "hello");
}


// ============================================================
// emplace_front
// ============================================================

void test_emplace_front() {
    Deque<std::pair<int, std::string>> d;

    d.emplace_back(20, "world");

    auto& p = d.emplace_front(10, "hello");

    assert(d.size() == 2);

    assert(p.first == 10);
    assert(p.second == "hello");

    assert(d.front().first == 10);
    assert(d.back().first == 20);
}


// ============================================================
// Move-only type
// ============================================================

void test_move_only_type() {
    Deque<MoveOnly> d;

    d.push_back(MoveOnly(10));
    d.push_front(MoveOnly(5));

    d.emplace_back(20);
    d.emplace_front(1);

    assert(d.size() == 4);

    assert(d[0].value == 1);
    assert(d[1].value == 5);
    assert(d[2].value == 10);
    assert(d[3].value == 20);

    d.pop_front();
    d.pop_back();

    assert(d.size() == 2);
    assert(d.front().value == 5);
    assert(d.back().value == 10);
}


// ============================================================
// Object lifetime
// ============================================================

void test_object_lifetime() {
    Tracker::reset();

    {
        Deque<Tracker> d;

        d.emplace_back(1);
        d.emplace_back(2);
        d.emplace_front(0);

        assert(Tracker::alive == 3);

        d.pop_back();

        assert(Tracker::alive == 2);

        d.pop_front();

        assert(Tracker::alive == 1);

        d.clear();

        assert(Tracker::alive == 0);

        d.emplace_back(100);

        assert(Tracker::alive == 1);
    }

    assert(Tracker::alive == 0);
    assert(Tracker::constructed == Tracker::destroyed);
}


// ============================================================
// Lifetime during growth
// ============================================================

void test_lifetime_during_growth() {
    Tracker::reset();

    {
        Deque<Tracker> d;

        for (int i = 0; i < 1000; ++i) {
            d.emplace_back(i);

            // Only the logical elements should currently be alive.
            assert(Tracker::alive == static_cast<int>(d.size()));
        }

        assert(d.size() == 1000);
        assert(Tracker::alive == 1000);
    }

    assert(Tracker::alive == 0);
    assert(Tracker::constructed == Tracker::destroyed);
}


// ============================================================
// Strings
// ============================================================

void test_strings() {
    Deque<std::string> d;

    d.push_back("two");
    d.push_front("one");
    d.emplace_back("three");
    d.emplace_front("zero");

    assert(d.size() == 4);

    assert(d[0] == "zero");
    assert(d[1] == "one");
    assert(d[2] == "two");
    assert(d[3] == "three");

    d.pop_front();
    d.pop_back();

    assert(d.front() == "one");
    assert(d.back() == "two");
}


// ============================================================
// Deterministic comparison against std::deque
// ============================================================

void test_vs_std_deque() {
    Deque<int> mine;
    std::deque<int> standard;

    for (int i = 0; i < 1000; ++i) {
        if (i % 2 == 0) {
            mine.push_back(i);
            standard.push_back(i);
        }
        else {
            mine.push_front(i);
            standard.push_front(i);
        }
    }

    assert(mine.size() == standard.size());

    for (std::size_t i = 0; i < standard.size(); ++i)
        assert(mine[i] == standard[i]);

    for (int i = 0; i < 250; ++i) {
        mine.pop_front();
        standard.pop_front();

        mine.pop_back();
        standard.pop_back();
    }

    assert(mine.size() == standard.size());

    for (std::size_t i = 0; i < standard.size(); ++i)
        assert(mine[i] == standard[i]);
}


// ============================================================
// Randomized stress test against std::deque
// ============================================================

void test_random_stress() {
    Deque<int> mine;
    std::deque<int> standard;

    std::mt19937 rng(42);

    std::uniform_int_distribution<int> operation(0, 5);
    std::uniform_int_distribution<int> value(-100000, 100000);

    constexpr int OPERATIONS = 100000;

    for (int step = 0; step < OPERATIONS; ++step) {
        int op = operation(rng);

        switch (op) {
            case 0: {
                int v = value(rng);

                mine.push_back(v);
                standard.push_back(v);

                break;
            }

            case 1: {
                int v = value(rng);

                mine.push_front(v);
                standard.push_front(v);

                break;
            }

            case 2: {
                if (!standard.empty()) {
                    mine.pop_back();
                    standard.pop_back();
                }

                break;
            }

            case 3: {
                if (!standard.empty()) {
                    mine.pop_front();
                    standard.pop_front();
                }

                break;
            }

            case 4: {
                if (!standard.empty()) {
                    assert(mine.front() == standard.front());
                    assert(mine.back() == standard.back());
                }

                break;
            }

            case 5: {
                if (!standard.empty()) {
                    std::uniform_int_distribution<std::size_t> index(
                            0,
                            standard.size() - 1
                    );

                    std::size_t i = index(rng);

                    assert(mine[i] == standard[i]);
                }

                break;
            }
        }

        assert(mine.size() == standard.size());
        assert(mine.empty() == standard.empty());

        if (!standard.empty()) {
            assert(mine.front() == standard.front());
            assert(mine.back() == standard.back());
        }

        // Periodically verify the entire logical sequence.
        if (step % 1000 == 0) {
            for (std::size_t i = 0; i < standard.size(); ++i)
                assert(mine[i] == standard[i]);
        }
    }

    assert(mine.size() == standard.size());

    for (std::size_t i = 0; i < standard.size(); ++i)
        assert(mine[i] == standard[i]);
}


// ============================================================
// Large sequential stress
// ============================================================

void test_large_stress() {
    constexpr int N = 100000;

    Deque<int> d;

    for (int i = 0; i < N; ++i)
        d.push_back(i);

    assert(d.size() == N);

    for (int i = 0; i < N; ++i)
        assert(d[i] == i);

    for (int i = 0; i < N / 2; ++i) {
        assert(d.front() == i);
        d.pop_front();
    }

    assert(d.size() == N / 2);

    for (int i = N; i < N + N / 2; ++i)
        d.push_back(i);

    assert(d.size() == N);

    for (int i = N / 2; i < N + N / 2; ++i) {
        assert(d.front() == i);
        d.pop_front();
    }

    assert(d.empty());
}


// ============================================================
// main
// ============================================================

int main() {
    test_default_construction();

    test_push_back();
    test_push_front();
    test_mixed_push();

    test_pop_back();
    test_pop_front();
    test_mixed_operations();

    test_index_access();
    test_at();
    test_const_access();

    test_clear();
    test_growth();

    test_wraparound();
    test_front_wraparound();
    test_growth_after_wraparound();
    test_repeated_wraparound();

    test_iterator_forward();
    test_iterator_mutation();
    test_iterator_increment_decrement();
    test_random_access_iterator();
    test_iterator_wraparound();
    test_const_iterator();
    test_iterator_conversion();
    test_algorithm_compatibility();

    test_copy_constructor();
    test_copy_wrapped_deque();
    test_copy_assignment();
    test_self_copy_assignment();

    test_move_constructor();
    test_move_assignment();
    test_self_move_assignment();

    test_swap();

    test_equality();
    test_comparisons();

    test_emplace_back();
    test_emplace_front();

    test_move_only_type();

    test_object_lifetime();
    test_lifetime_during_growth();

    test_strings();

    test_vs_std_deque();
    test_random_stress();
    test_large_stress();

    std::cout << "All Deque tests passed!\n";

    return 0;
}
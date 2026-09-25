#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>

#include "../include/ministl/containers/vector.hpp"

using ministl::Vector;

/** AI GENERATED TESTS **/

// ============================================================================
// Helper type for testing object lifetime
// ============================================================================

struct Tracker {
    static int alive;
    static int constructed;
    static int destroyed;
    static int copied;
    static int moved;

    int value;

    Tracker(int v = 0) : value(v) {
        ++alive;
        ++constructed;
    }

    Tracker(const Tracker& other) : value(other.value) {
        ++alive;
        ++constructed;
        ++copied;
    }

    Tracker(Tracker&& other) noexcept : value(other.value) {
        ++alive;
        ++constructed;
        ++moved;

        other.value = -1;
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


// ============================================================================
// Constructors
// ============================================================================

void test_default_constructor() {
    Vector<int> v;

    assert(v.empty());
    assert(v.size() == 0);
    assert(v.capacity() == 0);
}

void test_count_constructor() {
    Vector<int> v(5);

    assert(v.size() == 5);
    assert(v.capacity() == 5);

    for (size_t i = 0; i < v.size(); ++i)
        assert(v[i] == 0);
}

void test_count_value_constructor() {
    Vector<int> v(5, 42);

    assert(v.size() == 5);

    for (size_t i = 0; i < v.size(); ++i)
        assert(v[i] == 42);
}

void test_initializer_list_constructor() {
    Vector<int> v{1, 2, 3, 4};

    assert(v.size() == 4);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    assert(v[3] == 4);
}

void test_range_constructor() {
    int values[] = {10, 20, 30, 40};

    Vector<int> v(values, values + 4);

    assert(v.size() == 4);
    assert(v[0] == 10);
    assert(v[1] == 20);
    assert(v[2] == 30);
    assert(v[3] == 40);
}


// ============================================================================
// push_back / pop_back / emplace_back
// ============================================================================

void test_push_back() {
    Vector<int> v;

    int x = 1;

    // lvalue -> copy version
    v.push_back(x);

    // rvalues -> move version
    v.push_back(2);
    v.push_back(3);

    assert(v.size() == 3);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
}

void test_capacity_growth() {
    Vector<int> v;

    for (int i = 0; i < 100; ++i)
        v.push_back(i);

    assert(v.size() == 100);
    assert(v.capacity() >= 100);

    for (int i = 0; i < 100; ++i)
        assert(v[i] == i);
}

void test_pop_back() {
    Vector<int> v{1, 2, 3};

    v.pop_back();

    assert(v.size() == 2);
    assert(v.back() == 2);

    v.pop_back();

    assert(v.size() == 1);
    assert(v.back() == 1);
}

void test_emplace_back() {
    Vector<std::string> v;

    v.emplace_back("Adam");
    v.emplace_back(5, 'x');

    assert(v.size() == 2);
    assert(v[0] == "Adam");
    assert(v[1] == "xxxxx");
}


// ============================================================================
// Element access
// ============================================================================

void test_front_back() {
    Vector<int> v{10, 20, 30};

    assert(v.front() == 10);
    assert(v.back() == 30);

    v.front() = 100;
    v.back() = 300;

    assert(v[0] == 100);
    assert(v[2] == 300);
}

void test_operator_brackets() {
    Vector<int> v{1, 2, 3};

    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);

    v[1] = 42;

    assert(v[1] == 42);
}

void test_at() {
    Vector<int> v{10, 20, 30};

    assert(v.at(0) == 10);
    assert(v.at(1) == 20);
    assert(v.at(2) == 30);

    bool threw = false;

    try {
        v.at(3);
    }
    catch (const std::out_of_range&) {
        threw = true;
    }

    assert(threw);
}

void test_const_access() {
    const Vector<int> v{1, 2, 3};

    assert(v[0] == 1);
    assert(v.at(1) == 2);
    assert(v.front() == 1);
    assert(v.back() == 3);
    assert(*v.data() == 1);
}


// ============================================================================
// data
// ============================================================================

void test_data() {
    Vector<int> v{1, 2, 3};

    int* ptr = v.data();

    assert(ptr[0] == 1);
    assert(ptr[1] == 2);
    assert(ptr[2] == 3);

    ptr[1] = 50;

    assert(v[1] == 50);
}


// ============================================================================
// Iterators
// ============================================================================

void test_iterator() {
    Vector<int> v{1, 2, 3, 4};

    int sum = 0;

    for (auto it = v.begin(); it != v.end(); ++it)
        sum += *it;

    assert(sum == 10);
}

void test_const_iterator() {
    const Vector<int> v{1, 2, 3};

    int sum = 0;

    for (auto it = v.cbegin(); it != v.cend(); ++it)
        sum += *it;

    assert(sum == 6);
}

void test_iterator_random_access() {
    Vector<int> v{10, 20, 30, 40, 50};

    auto it = v.begin();

    assert(*it == 10);

    ++it;
    assert(*it == 20);

    it += 2;
    assert(*it == 40);

    it -= 1;
    assert(*it == 30);

    assert(it[1] == 40);

    assert(*(v.begin() + 4) == 50);
    assert(*(v.end() - 1) == 50);

    assert(v.end() - v.begin() == 5);

    assert(v.begin() < v.end());
    assert(v.end() > v.begin());
    assert(v.begin() <= v.begin());
    assert(v.end() >= v.begin());
}

void test_range_loop() {
    Vector<int> v{1, 2, 3};

    int sum = 0;

    for (int x : v)
        sum += x;

    assert(sum == 6);
}


// ============================================================================
// insert
// ============================================================================

void test_insert_copy() {
    Vector<int> v{1, 3};

    int value = 2;

    auto it = v.insert(v.begin() + 1, value);

    assert(v.size() == 3);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);

    assert(*it == 2);
}

void test_insert_move() {
    Vector<std::string> v{"A", "C"};

    std::string value = "B";

    auto it = v.insert(v.begin() + 1, std::move(value));

    assert(v.size() == 3);
    assert(v[0] == "A");
    assert(v[1] == "B");
    assert(v[2] == "C");

    assert(*it == "B");
}

void test_insert_begin() {
    Vector<int> v{2, 3};

    v.insert(v.begin(), 1);

    assert(v.size() == 3);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
}

void test_insert_end() {
    Vector<int> v{1, 2};

    auto it = v.insert(v.end(), 3);

    assert(v.size() == 3);
    assert(v[2] == 3);
    assert(*it == 3);
}

void test_insert_empty_vector() {
    Vector<int> v;

    auto it = v.insert(v.begin(), 42);

    assert(v.size() == 1);
    assert(v[0] == 42);
    assert(*it == 42);
}

void test_insert_count() {
    Vector<int> v{1, 5};

    auto it = v.insert(v.begin() + 1, 3, 2);

    assert(v.size() == 5);

    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 2);
    assert(v[3] == 2);
    assert(v[4] == 5);

    assert(*it == 2);
}

void test_insert_range() {
    Vector<int> v{1, 5};

    int values[] = {2, 3, 4};

    auto it = v.insert(
            v.begin() + 1,
            values,
            values + 3
    );

    assert(v.size() == 5);

    for (int i = 0; i < 5; ++i)
        assert(v[i] == i + 1);

    assert(*it == 2);
}

void test_insert_initializer_list() {
    Vector<int> v{1, 5};

    auto it = v.insert(
            v.begin() + 1,
            {2, 3, 4}
    );

    assert(v.size() == 5);

    for (int i = 0; i < 5; ++i)
        assert(v[i] == i + 1);

    assert(*it == 2);
}


// ============================================================================
// emplace
// ============================================================================

void test_emplace() {
    Vector<std::string> v{"Adam", "Saleh"};

    auto it = v.emplace(
            v.begin() + 1,
            5,
            'x'
    );

    assert(v.size() == 3);

    assert(v[0] == "Adam");
    assert(v[1] == "xxxxx");
    assert(v[2] == "Saleh");

    assert(*it == "xxxxx");
}


// ============================================================================
// erase
// ============================================================================

void test_erase() {
    Vector<int> v{1, 2, 3};

    auto it = v.erase(v.begin() + 1);

    assert(v.size() == 2);
    assert(v[0] == 1);
    assert(v[1] == 3);

    assert(it == v.begin() + 1);
    assert(*it == 3);
}

void test_erase_first() {
    Vector<int> v{1, 2, 3};

    v.erase(v.begin());

    assert(v.size() == 2);
    assert(v[0] == 2);
    assert(v[1] == 3);
}

void test_erase_last() {
    Vector<int> v{1, 2, 3};

    auto it = v.erase(v.end() - 1);

    assert(v.size() == 2);
    assert(v[0] == 1);
    assert(v[1] == 2);

    assert(it == v.end());
}

void test_erase_range() {
    Vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(
            v.begin() + 1,
            v.begin() + 4
    );

    assert(v.size() == 2);
    assert(v[0] == 1);
    assert(v[1] == 5);

    assert(it == v.begin() + 1);
    assert(*it == 5);
}

void test_erase_empty_range() {
    Vector<int> v{1, 2, 3};

    auto it = v.erase(
            v.begin() + 1,
            v.begin() + 1
    );

    assert(v.size() == 3);
    assert(it == v.begin() + 1);
}


// ============================================================================
// assign
// ============================================================================

void test_assign_count() {
    Vector<int> v{1, 2, 3};

    v.assign(5, 42);

    assert(v.size() == 5);

    for (size_t i = 0; i < v.size(); ++i)
        assert(v[i] == 42);
}

void test_assign_range() {
    Vector<int> v{1, 2};

    int values[] = {10, 20, 30, 40};

    v.assign(values, values + 4);

    assert(v.size() == 4);
    assert(v[0] == 10);
    assert(v[1] == 20);
    assert(v[2] == 30);
    assert(v[3] == 40);
}

void test_assign_initializer_list() {
    Vector<int> v;

    v.assign({5, 6, 7});

    assert(v.size() == 3);
    assert(v[0] == 5);
    assert(v[1] == 6);
    assert(v[2] == 7);
}

void test_initializer_list_assignment() {
    Vector<int> v{1, 2};

    v = {10, 20, 30};

    assert(v.size() == 3);
    assert(v[0] == 10);
    assert(v[1] == 20);
    assert(v[2] == 30);
}


// ============================================================================
// resize / reserve / shrink_to_fit
// ============================================================================

void test_resize_grow() {
    Vector<int> v{1, 2};

    v.resize(5);

    assert(v.size() == 5);

    assert(v[0] == 1);
    assert(v[1] == 2);

    assert(v[2] == 0);
    assert(v[3] == 0);
    assert(v[4] == 0);
}

void test_resize_shrink() {
    Vector<int> v{1, 2, 3, 4, 5};

    v.resize(2);

    assert(v.size() == 2);
    assert(v[0] == 1);
    assert(v[1] == 2);
}

void test_resize_value() {
    Vector<int> v{1, 2};

    v.resize(5, 42);

    assert(v.size() == 5);

    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 42);
    assert(v[3] == 42);
    assert(v[4] == 42);
}

void test_reserve() {
    Vector<int> v{1, 2, 3};

    v.reserve(100);

    assert(v.size() == 3);
    assert(v.capacity() >= 100);

    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
}

void test_reserve_smaller_does_nothing() {
    Vector<int> v;

    v.reserve(100);

    size_t old_capacity = v.capacity();

    v.reserve(10);

    assert(v.capacity() == old_capacity);
}

void test_shrink_to_fit() {
    Vector<int> v;

    v.reserve(100);

    for (int i = 0; i < 10; ++i)
        v.push_back(i);

    assert(v.capacity() >= 100);

    v.shrink_to_fit();

    assert(v.size() == 10);
    assert(v.capacity() == 10);

    for (int i = 0; i < 10; ++i)
        assert(v[i] == i);
}


// ============================================================================
// clear / empty
// ============================================================================

void test_clear() {
    Vector<int> v{1, 2, 3};

    size_t old_capacity = v.capacity();

    v.clear();

    assert(v.empty());
    assert(v.size() == 0);

    // clear() should not release capacity
    assert(v.capacity() == old_capacity);

    // storage should still be reusable
    v.push_back(42);

    assert(v.size() == 1);
    assert(v[0] == 42);
}


// ============================================================================
// copy / move
// ============================================================================

void test_copy_constructor() {
    Vector<std::string> original{"A", "B", "C"};

    Vector<std::string> copy(original);

    assert(copy.size() == 3);
    assert(copy[0] == "A");
    assert(copy[1] == "B");
    assert(copy[2] == "C");

    // Make sure this is a deep copy
    copy[0] = "Changed";

    assert(original[0] == "A");
    assert(copy[0] == "Changed");
}

void test_copy_assignment() {
    Vector<std::string> original{"A", "B", "C"};

    Vector<std::string> copy{"X", "Y"};

    copy = original;

    assert(copy.size() == 3);
    assert(copy[0] == "A");
    assert(copy[1] == "B");
    assert(copy[2] == "C");

    copy[0] = "Changed";

    assert(original[0] == "A");
}

void test_self_copy_assignment() {
    Vector<int> v{1, 2, 3};

    v = v;

    assert(v.size() == 3);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
}

void test_move_constructor() {
    Vector<std::string> original{"A", "B", "C"};

    Vector<std::string> moved(std::move(original));

    assert(moved.size() == 3);
    assert(moved[0] == "A");
    assert(moved[1] == "B");
    assert(moved[2] == "C");

    assert(original.size() == 0);
    assert(original.capacity() == 0);
    assert(original.data() == nullptr);
}

void test_move_assignment() {
    Vector<std::string> original{"A", "B", "C"};

    Vector<std::string> moved{"X", "Y"};

    moved = std::move(original);

    assert(moved.size() == 3);
    assert(moved[0] == "A");
    assert(moved[1] == "B");
    assert(moved[2] == "C");

    assert(original.size() == 0);
    assert(original.capacity() == 0);
    assert(original.data() == nullptr);
}


// ============================================================================
// swap
// ============================================================================

void test_swap() {
    Vector<int> a{1, 2};
    Vector<int> b{10, 20, 30};

    a.swap(b);

    assert(a.size() == 3);
    assert(a[0] == 10);
    assert(a[1] == 20);
    assert(a[2] == 30);

    assert(b.size() == 2);
    assert(b[0] == 1);
    assert(b[1] == 2);
}


// ============================================================================
// comparisons
// ============================================================================

void test_comparisons() {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2, 3};
    Vector<int> c{1, 2, 4};
    Vector<int> d{1, 2};

    assert(a == b);
    assert(!(a != b));

    assert(a != c);

    assert(a < c);
    assert(c > a);

    assert(d < a);
    assert(d <= a);

    assert(c >= a);
}


// ============================================================================
// sorting
// ============================================================================

void test_sort() {
    Vector<int> v{5, 1, 4, 2, 3};

    v.sort();

    for (int i = 0; i < 5; ++i)
        assert(v[i] == i + 1);
}

void test_custom_sort() {
    Vector<int> v{1, 5, 2, 4, 3};

    v.sort([](int a, int b) {
        return a > b;
    });

    assert(v[0] == 5);
    assert(v[1] == 4);
    assert(v[2] == 3);
    assert(v[3] == 2);
    assert(v[4] == 1);
}


// ============================================================================
// max_size
// ============================================================================

void test_max_size() {
    Vector<int> v;

    assert(v.max_size() > 0);
    assert(v.max_size() >= v.capacity());
}


// ============================================================================
// OBJECT LIFETIME TESTS
//
// These are especially important now that Vector uses raw storage.
// ============================================================================

void test_lifetime_push_pop() {
    Tracker::reset();

    {
        Vector<Tracker> v;

        v.emplace_back(1);
        v.emplace_back(2);
        v.emplace_back(3);

        assert(Tracker::alive == 3);

        v.pop_back();

        assert(Tracker::alive == 2);

        v.clear();

        assert(Tracker::alive == 0);
    }

    assert(Tracker::alive == 0);
    assert(Tracker::constructed == Tracker::destroyed);
}

void test_lifetime_reallocation() {
    Tracker::reset();

    {
        Vector<Tracker> v;

        for (int i = 0; i < 100; ++i)
            v.emplace_back(i);

        assert(v.size() == 100);
        assert(Tracker::alive == 100);

        for (int i = 0; i < 100; ++i)
            assert(v[i].value == i);
    }

    assert(Tracker::alive == 0);
    assert(Tracker::constructed == Tracker::destroyed);
}

void test_lifetime_resize() {
    Tracker::reset();

    {
        Vector<Tracker> v;

        v.resize(20);

        assert(v.size() == 20);
        assert(Tracker::alive == 20);

        v.resize(5);

        assert(v.size() == 5);
        assert(Tracker::alive == 5);

        v.resize(10);

        assert(v.size() == 10);
        assert(Tracker::alive == 10);
    }

    assert(Tracker::alive == 0);
    assert(Tracker::constructed == Tracker::destroyed);
}

void test_lifetime_insert_erase() {
    Tracker::reset();

    {
        Vector<Tracker> v;

        v.emplace_back(1);
        v.emplace_back(3);

        v.emplace(v.begin() + 1, 2);

        assert(v.size() == 3);
        assert(v[0].value == 1);
        assert(v[1].value == 2);
        assert(v[2].value == 3);

        assert(Tracker::alive == 3);

        v.erase(v.begin() + 1);

        assert(v.size() == 2);
        assert(v[0].value == 1);
        assert(v[1].value == 3);

        assert(Tracker::alive == 2);
    }

    assert(Tracker::alive == 0);
    assert(Tracker::constructed == Tracker::destroyed);
}


// ============================================================================
// Comparison against std::vector
// ============================================================================

void test_against_std_vector() {
    Vector<int> mine;
    std::vector<int> standard;

    for (int i = 0; i < 10000; ++i) {
        mine.push_back(i);
        standard.push_back(i);
    }

    assert(mine.size() == standard.size());

    for (size_t i = 0; i < mine.size(); ++i)
        assert(mine[i] == standard[i]);

    for (int i = 0; i < 1000; ++i) {
        size_t index = mine.size() / 2;

        mine.insert(mine.begin() + index, 42);
        standard.insert(standard.begin() + index, 42);
    }

    assert(mine.size() == standard.size());

    for (size_t i = 0; i < mine.size(); ++i)
        assert(mine[i] == standard[i]);

    for (int i = 0; i < 1000; ++i) {
        size_t index = mine.size() / 2;

        mine.erase(mine.begin() + index);
        standard.erase(standard.begin() + index);
    }

    assert(mine.size() == standard.size());

    for (size_t i = 0; i < mine.size(); ++i)
        assert(mine[i] == standard[i]);
}


// ============================================================================
// Stress
// ============================================================================

void test_stress() {
    Vector<int> v;

    constexpr int N = 100000;

    for (int i = 0; i < N; ++i)
        v.push_back(i);

    assert(v.size() == N);

    for (int i = 0; i < N; ++i)
        assert(v[i] == i);

    for (int i = 0; i < N / 2; ++i)
        v.pop_back();

    assert(v.size() == N / 2);

    for (int i = 0; i < 1000; ++i)
        v.insert(v.begin() + v.size() / 2, 42);

    for (int i = 0; i < 1000; ++i)
        v.erase(v.begin() + v.size() / 2);

    assert(v.size() == N / 2);
}

void test_sort_stress() {
    Vector<int> v;

    // 100,000 is more than enough for a unit/stress test.
    // Your old 100,000,000-element test was enormous.
    constexpr int N = 100000;

    for (int i = N; i > 0; --i)
        v.push_back(i);

    v.sort();

    for (int i = 0; i < N; ++i)
        assert(v[i] == i + 1);
}


// ============================================================================
// Main
// ============================================================================

int main() {

    // Constructors
    test_default_constructor();
    test_count_constructor();
    test_count_value_constructor();
    test_initializer_list_constructor();
    test_range_constructor();

    // Basic modifiers
    test_push_back();
    test_capacity_growth();
    test_pop_back();
    test_emplace_back();

    // Access
    test_front_back();
    test_operator_brackets();
    test_at();
    test_const_access();
    test_data();

    // Iterators
    test_iterator();
    test_const_iterator();
    test_iterator_random_access();
    test_range_loop();

    // Insert / emplace
    test_insert_copy();
    test_insert_move();
    test_insert_begin();
    test_insert_end();
    test_insert_empty_vector();
    test_insert_count();
    test_insert_range();
    test_insert_initializer_list();
    test_emplace();

    // Erase
    test_erase();
    test_erase_first();
    test_erase_last();
    test_erase_range();
    test_erase_empty_range();

    // Assignment
    test_assign_count();
    test_assign_range();
    test_assign_initializer_list();
    test_initializer_list_assignment();

    // Capacity
    test_resize_grow();
    test_resize_shrink();
    test_resize_value();
    test_reserve();
    test_reserve_smaller_does_nothing();
    test_shrink_to_fit();

    // Clear
    test_clear();

    // Rule of Five
    test_copy_constructor();
    test_copy_assignment();
    test_self_copy_assignment();
    test_move_constructor();
    test_move_assignment();

    // Other operations
    test_swap();
    test_comparisons();
    test_sort();
    test_custom_sort();
    test_max_size();

    // Raw-memory / lifetime correctness
    test_lifetime_push_pop();
    test_lifetime_reallocation();
    test_lifetime_resize();
    test_lifetime_insert_erase();

    // Comparison / stress
    test_against_std_vector();
    test_stress();
    test_sort_stress();

    std::cout << "All MiniSTL Vector tests passed!\n";

    return 0;
}
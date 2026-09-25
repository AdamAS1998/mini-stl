#include <iostream>
#include <cassert>
#include <list>
#include <string>
#include <utility>
#include <random>
#include <type_traits>

#include "../include/ministl/containers/linked_list.hpp"

using ministl::List;

/** AI GENERATED TESTS **/

// ============================================================
// Helper: compare MiniSTL List with std::list
// ============================================================

template<typename T>
void assert_lists_equal(const List<T>& actual,
                        const std::list<T>& expected) {

    assert(actual.size() == expected.size());
    assert(actual.empty() == expected.empty());

    auto it1 = actual.begin();
    auto it2 = expected.begin();

    while (it1 != actual.end() && it2 != expected.end()) {
        assert(*it1 == *it2);
        ++it1;
        ++it2;
    }

    assert(it1 == actual.end());
    assert(it2 == expected.end());

    if (!actual.empty()) {
        assert(actual.front() == expected.front());
        assert(actual.back() == expected.back());
    }
}

// ============================================================
// Custom object used for Rule of Five / emplace tests
// ============================================================

struct Tracker {
    static int alive;
    static int copies;
    static int moves;

    int value;

    explicit Tracker(int v = 0)
            : value(v) {
        ++alive;
    }

    Tracker(const Tracker& other)
            : value(other.value) {
        ++alive;
        ++copies;
    }

    Tracker(Tracker&& other) noexcept
            : value(other.value) {
        ++alive;
        ++moves;
        other.value = -1;
    }

    Tracker& operator=(const Tracker& other) {
        value = other.value;
        ++copies;
        return *this;
    }

    Tracker& operator=(Tracker&& other) noexcept {
        value = other.value;
        other.value = -1;
        ++moves;
        return *this;
    }

    ~Tracker() {
        --alive;
    }

    bool operator==(const Tracker& other) const {
        return value == other.value;
    }

    bool operator<(const Tracker& other) const {
        return value < other.value;
    }
};

int Tracker::alive = 0;
int Tracker::copies = 0;
int Tracker::moves = 0;


// ============================================================
// 1. Default construction
// ============================================================

void test_default_constructor() {

    List<int> l;

    assert(l.empty());
    assert(l.size() == 0);

    assert(l.begin() == l.end());
    assert(l.cbegin() == l.cend());
}


// ============================================================
// 2. push_back / push_front
// ============================================================

void test_push() {

    List<int> l;

    l.push_back(2);
    l.push_front(1);
    l.push_back(3);

    assert(l.size() == 3);
    assert(!l.empty());

    assert(l.front() == 1);
    assert(l.back() == 3);

    int expected[] = {1, 2, 3};
    int i = 0;

    for (int x : l)
        assert(x == expected[i++]);

    assert(i == 3);
}


// ============================================================
// 3. pop_back / pop_front
// ============================================================

void test_pop() {

    List<int> l;

    // Your implementation intentionally treats pop on an
    // empty list as a no-op.
    l.pop_front();
    l.pop_back();

    assert(l.empty());

    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    l.pop_front();

    assert(l.size() == 2);
    assert(l.front() == 2);
    assert(l.back() == 3);

    l.pop_back();

    assert(l.size() == 1);
    assert(l.front() == 2);
    assert(l.back() == 2);

    l.pop_back();

    assert(l.empty());
    assert(l.size() == 0);

    // Again test empty behavior.
    l.pop_back();
    l.pop_front();

    assert(l.empty());
}


// ============================================================
// 4. front / back mutation
// ============================================================

void test_front_back_mutation() {

    List<int> l;

    l.push_back(10);
    l.push_back(20);

    l.front() = 100;
    l.back() = 200;

    assert(l.front() == 100);
    assert(l.back() == 200);
}


// ============================================================
// 5. const front / back
// ============================================================

void test_const_access() {

    List<int> l;

    l.push_back(10);
    l.push_back(20);

    const List<int>& cl = l;

    assert(cl.front() == 10);
    assert(cl.back() == 20);
    assert(cl.size() == 2);
    assert(!cl.empty());
}


// ============================================================
// 6. Forward iterator
// ============================================================

void test_forward_iterator() {

    List<int> l;

    for (int i = 1; i <= 5; ++i)
        l.push_back(i);

    auto it = l.begin();

    for (int expected = 1; expected <= 5; ++expected) {
        assert(it != l.end());
        assert(*it == expected);
        ++it;
    }

    assert(it == l.end());
}


// ============================================================
// 7. Bidirectional iterator
// ============================================================

void test_backward_iterator() {

    List<int> l;

    for (int i = 1; i <= 5; ++i)
        l.push_back(i);

    auto it = l.end();

    for (int expected = 5; expected >= 1; --expected) {
        --it;
        assert(*it == expected);
    }

    assert(it == l.begin());
}


// ============================================================
// 8. Prefix / postfix iterator operators
// ============================================================

void test_iterator_operators() {

    List<int> l;

    l.push_back(10);
    l.push_back(20);
    l.push_back(30);

    auto it = l.begin();

    auto old = it++;

    assert(*old == 10);
    assert(*it == 20);

    old = it--;

    assert(*old == 20);
    assert(*it == 10);

    ++it;
    assert(*it == 20);

    --it;
    assert(*it == 10);

    assert(it.operator->() != nullptr);
    assert(*it.operator->() == 10);
}


// ============================================================
// 9. const_iterator
// ============================================================

void test_const_iterator() {

    List<int> l;

    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    const List<int>& cl = l;

    auto it = cl.begin();

    assert(*it == 1);

    ++it;
    assert(*it == 2);

    ++it;
    assert(*it == 3);

    ++it;
    assert(it == cl.end());
}


// ============================================================
// 10. iterator -> const_iterator conversion
// ============================================================

void test_iterator_const_conversion() {

    static_assert(
            std::is_convertible_v<
                    List<int>::iterator,
                    List<int>::const_iterator
            >
    );

    static_assert(
            !std::is_convertible_v<
                    List<int>::const_iterator,
                    List<int>::iterator
            >
    );

    List<int> l;

    l.push_back(10);
    l.push_back(20);

    List<int>::iterator it = l.begin();
    List<int>::const_iterator cit = it;

    assert(it == cit);
    assert(cit == it);
    assert(*cit == 10);
}


// ============================================================
// 11. Copy constructor
// ============================================================

void test_copy_constructor() {

    List<int> original;

    original.push_back(1);
    original.push_back(2);
    original.push_back(3);

    List<int> copy(original);

    assert(copy.size() == 3);

    auto a = original.begin();
    auto b = copy.begin();

    while (a != original.end()) {
        assert(*a == *b);
        ++a;
        ++b;
    }

    // Verify deep copy.
    copy.front() = 100;

    assert(original.front() == 1);
    assert(copy.front() == 100);
}


// ============================================================
// 12. Copy assignment
// ============================================================

void test_copy_assignment() {

    List<int> a;

    a.push_back(1);
    a.push_back(2);
    a.push_back(3);

    List<int> b;

    b.push_back(100);
    b.push_back(200);

    b = a;

    assert(b.size() == 3);
    assert(b.front() == 1);
    assert(b.back() == 3);

    b.front() = 999;

    assert(a.front() == 1);

    // Self assignment.
    a = a;

    assert(a.size() == 3);
    assert(a.front() == 1);
    assert(a.back() == 3);
}


// ============================================================
// 13. Move constructor
// ============================================================

void test_move_constructor() {

    List<int> source;

    source.push_back(1);
    source.push_back(2);
    source.push_back(3);

    List<int> moved(std::move(source));

    assert(moved.size() == 3);
    assert(moved.front() == 1);
    assert(moved.back() == 3);

    assert(source.empty());
    assert(source.size() == 0);
}


// ============================================================
// 14. Move assignment
// ============================================================

void test_move_assignment() {

    List<int> source;

    source.push_back(1);
    source.push_back(2);
    source.push_back(3);

    List<int> destination;

    destination.push_back(100);
    destination.push_back(200);

    destination = std::move(source);

    assert(destination.size() == 3);
    assert(destination.front() == 1);
    assert(destination.back() == 3);

    assert(source.empty());

    // Self move assignment should remain valid.
    destination = std::move(destination);

    assert(destination.size() == 3);
    assert(destination.front() == 1);
    assert(destination.back() == 3);
}


// ============================================================
// 15. emplace_back / emplace_front
// ============================================================

void test_emplace_front_back() {

    List<std::pair<int, std::string>> l;

    l.emplace_back(2, "two");
    l.emplace_front(1, "one");
    l.emplace_back(3, "three");

    assert(l.size() == 3);

    assert(l.front().first == 1);
    assert(l.front().second == "one");

    assert(l.back().first == 3);
    assert(l.back().second == "three");
}


// ============================================================
// 16. insert
// ============================================================

void test_insert() {

    List<int> l;

    l.push_back(2);
    l.push_back(4);

    // Insert at beginning.
    auto it = l.insert(l.begin(), 1);

    assert(*it == 1);
    assert(l.front() == 1);

    // Insert in middle.
    auto pos = l.begin();
    ++pos;
    ++pos;

    it = l.insert(pos, 3);

    assert(*it == 3);

    // Insert at end.
    it = l.insert(l.end(), 5);

    assert(*it == 5);
    assert(l.back() == 5);

    int expected[] = {1, 2, 3, 4, 5};
    int i = 0;

    for (int x : l)
        assert(x == expected[i++]);

    assert(i == 5);
}


// ============================================================
// 17. emplace
// ============================================================

void test_emplace() {

    List<std::pair<int, std::string>> l;

    l.emplace_back(1, "one");
    l.emplace_back(3, "three");

    auto pos = l.begin();
    ++pos;

    auto inserted = l.emplace(pos, 2, "two");

    assert(inserted->first == 2);
    assert(inserted->second == "two");

    assert(l.size() == 3);
}


// ============================================================
// 18. erase
// ============================================================

void test_erase() {

    List<int> l;

    for (int i = 1; i <= 5; ++i)
        l.push_back(i);

    // Erase beginning.
    auto next = l.erase(l.begin());

    assert(*next == 2);
    assert(l.front() == 2);
    assert(l.size() == 4);

    // Erase middle (3).
    auto it = l.begin();
    ++it;

    next = l.erase(it);

    assert(*next == 4);
    assert(l.size() == 3);

    // Erase last.
    it = l.end();
    --it;

    next = l.erase(it);

    assert(next == l.end());
    assert(l.back() == 4);
    assert(l.size() == 2);

    // Your implementation treats erase(end()) as a no-op.
    next = l.erase(l.end());

    assert(next == l.end());
    assert(l.size() == 2);
}


// ============================================================
// 19. clear
// ============================================================

void test_clear() {

    List<int> l;

    for (int i = 0; i < 100; ++i)
        l.push_back(i);

    assert(l.size() == 100);

    l.clear();

    assert(l.empty());
    assert(l.size() == 0);
    assert(l.begin() == l.end());

    // Clear already empty list.
    l.clear();

    assert(l.empty());

    // List should remain reusable.
    l.push_back(42);

    assert(l.size() == 1);
    assert(l.front() == 42);
}


// ============================================================
// 20. resize default
// ============================================================

void test_resize_default() {

    List<int> l;

    l.push_back(1);
    l.push_back(2);

    l.resize(5);

    assert(l.size() == 5);

    int expected[] = {1, 2, 0, 0, 0};
    int i = 0;

    for (int x : l)
        assert(x == expected[i++]);

    l.resize(1);

    assert(l.size() == 1);
    assert(l.front() == 1);
    assert(l.back() == 1);

    l.resize(0);

    assert(l.empty());
}


// ============================================================
// 21. resize with value
// ============================================================

void test_resize_value() {

    List<int> l;

    l.push_back(1);
    l.push_back(2);

    l.resize(5, 9);

    int expected[] = {1, 2, 9, 9, 9};
    int i = 0;

    for (int x : l)
        assert(x == expected[i++]);

    assert(l.size() == 5);

    l.resize(3, 100);

    assert(l.size() == 3);
    assert(l.back() == 9);
}


// ============================================================
// 22. swap
// ============================================================

void test_swap() {

    List<int> a;
    List<int> b;

    a.push_back(1);
    a.push_back(2);

    b.push_back(10);
    b.push_back(20);
    b.push_back(30);

    a.swap(b);

    assert(a.size() == 3);
    assert(a.front() == 10);
    assert(a.back() == 30);

    assert(b.size() == 2);
    assert(b.front() == 1);
    assert(b.back() == 2);

    // Empty/non-empty swap.
    List<int> empty;

    a.swap(empty);

    assert(a.empty());
    assert(empty.size() == 3);
    assert(empty.front() == 10);
    assert(empty.back() == 30);
}


// ============================================================
// 23. remove
// ============================================================

void test_remove() {

    List<int> l;

    int values[] = {1, 2, 3, 2, 4, 2, 5};

    for (int x : values)
        l.push_back(x);

    l.remove(2);

    int expected[] = {1, 3, 4, 5};
    int i = 0;

    for (int x : l)
        assert(x == expected[i++]);

    assert(i == 4);
    assert(l.size() == 4);

    // Value not present.
    l.remove(999);

    assert(l.size() == 4);
}


// ============================================================
// 24. remove_if
// ============================================================

void test_remove_if() {

    List<int> l;

    for (int i = 1; i <= 10; ++i)
        l.push_back(i);

    l.remove_if([](int x) {
        return x % 2 == 0;
    });

    int expected[] = {1, 3, 5, 7, 9};
    int i = 0;

    for (int x : l)
        assert(x == expected[i++]);

    assert(i == 5);
}


// ============================================================
// 25. reverse
// ============================================================

void test_reverse() {

    List<int> l;

    // Empty.
    l.reverse();
    assert(l.empty());

    // One element.
    l.push_back(1);
    l.reverse();

    assert(l.front() == 1);
    assert(l.back() == 1);

    l.clear();

    for (int i = 1; i <= 5; ++i)
        l.push_back(i);

    l.reverse();

    int expected = 5;

    for (int x : l)
        assert(x == expected--);

    assert(l.front() == 5);
    assert(l.back() == 1);

    // Reverse twice should restore original order.
    l.reverse();

    expected = 1;

    for (int x : l)
        assert(x == expected++);
}


// ============================================================
// 26. unique
// ============================================================

void test_unique() {

    List<int> l;

    int values[] = {
            1, 1, 1,
            2, 2,
            3,
            4, 4, 4,
            5
    };

    for (int x : values)
        l.push_back(x);

    l.unique();

    int expected[] = {1, 2, 3, 4, 5};
    int i = 0;

    for (int x : l)
        assert(x == expected[i++]);

    assert(i == 5);

    // Empty list.
    List<int> empty;
    empty.unique();
    assert(empty.empty());

    // Single element.
    List<int> single;
    single.push_back(1);
    single.unique();

    assert(single.size() == 1);
}


// ============================================================
// 27. unique custom predicate
// ============================================================

void test_unique_predicate() {

    List<int> l;

    // Consecutive values considered equivalent if
    // they have the same last digit.
    l.push_back(1);
    l.push_back(11);
    l.push_back(21);
    l.push_back(2);
    l.push_back(12);
    l.push_back(3);

    l.unique([](int a, int b) {
        return a % 10 == b % 10;
    });

    int expected[] = {1, 2, 3};
    int i = 0;

    for (int x : l)
        assert(x == expected[i++]);

    assert(i == 3);
}


// ============================================================
// 28. splice at end
// ============================================================

void test_splice_end() {

    List<int> a;
    List<int> b;

    a.push_back(1);
    a.push_back(2);

    b.push_back(3);
    b.push_back(4);

    a.splice(a.end(), b);

    int expected[] = {1, 2, 3, 4};
    int i = 0;

    for (int x : a)
        assert(x == expected[i++]);

    assert(i == 4);

    assert(b.empty());
    assert(b.size() == 0);
}


// ============================================================
// 29. splice at beginning
// ============================================================

void test_splice_begin() {

    List<int> a;
    List<int> b;

    a.push_back(3);
    a.push_back(4);

    b.push_back(1);
    b.push_back(2);

    a.splice(a.begin(), b);

    int expected[] = {1, 2, 3, 4};
    int i = 0;

    for (int x : a)
        assert(x == expected[i++]);

    assert(i == 4);
    assert(b.empty());

    assert(a.front() == 1);
    assert(a.back() == 4);
}


// ============================================================
// 30. splice in middle
// ============================================================

void test_splice_middle() {

    List<int> a;
    List<int> b;

    a.push_back(1);
    a.push_back(4);

    b.push_back(2);
    b.push_back(3);

    auto pos = a.begin();
    ++pos;

    a.splice(pos, b);

    int expected[] = {1, 2, 3, 4};
    int i = 0;

    for (int x : a)
        assert(x == expected[i++]);

    assert(i == 4);
    assert(b.empty());
}


// ============================================================
// 31. splice into empty list
// ============================================================

void test_splice_into_empty() {

    List<int> a;
    List<int> b;

    b.push_back(1);
    b.push_back(2);
    b.push_back(3);

    a.splice(a.end(), b);

    assert(a.size() == 3);
    assert(a.front() == 1);
    assert(a.back() == 3);

    assert(b.empty());
}


// ============================================================
// 32. splice empty source
// ============================================================

void test_splice_empty_source() {

    List<int> a;
    List<int> b;

    a.push_back(1);
    a.push_back(2);

    a.splice(a.end(), b);

    assert(a.size() == 2);
    assert(a.front() == 1);
    assert(a.back() == 2);

    assert(b.empty());
}


// ============================================================
// 33. self splice
// ============================================================

void test_self_splice() {

    List<int> l;

    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    l.splice(l.end(), l);

    assert(l.size() == 3);

    int expected = 1;

    for (int x : l)
        assert(x == expected++);
}


// ============================================================
// 34. sort
// ============================================================

void test_sort() {

    List<int> l;

    int values[] = {
            5, 1, 8, 3, 3, 9, 2, 7, 4, 6, 0
    };

    for (int x : values)
        l.push_back(x);

    l.sort();

    int expected[] = {
            0, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9
    };

    int i = 0;

    for (int x : l)
        assert(x == expected[i++]);

    assert(i == 11);

    assert(l.front() == 0);
    assert(l.back() == 9);
}


// ============================================================
// 35. sort comparator
// ============================================================

void test_sort_comparator() {

    List<int> l;

    for (int x : {1, 5, 2, 4, 3})
        l.push_back(x);

    l.sort([](int a, int b) {
        return a > b;
    });

    int expected = 5;

    for (int x : l)
        assert(x == expected--);
}


// ============================================================
// 36. sort edge cases
// ============================================================

void test_sort_edge_cases() {

    List<int> empty;

    empty.sort();

    assert(empty.empty());

    List<int> single;

    single.push_back(42);
    single.sort();

    assert(single.size() == 1);
    assert(single.front() == 42);

    List<int> already_sorted;

    for (int i = 0; i < 10; ++i)
        already_sorted.push_back(i);

    already_sorted.sort();

    int expected = 0;

    for (int x : already_sorted)
        assert(x == expected++);

    List<int> reversed;

    for (int i = 9; i >= 0; --i)
        reversed.push_back(i);

    reversed.sort();

    expected = 0;

    for (int x : reversed)
        assert(x == expected++);
}


// ============================================================
// 37. sort stability
// ============================================================

struct StableItem {
    int key;
    int original_order;
};

void test_sort_stability() {

    List<StableItem> l;

    l.push_back({2, 0});
    l.push_back({1, 1});
    l.push_back({2, 2});
    l.push_back({1, 3});
    l.push_back({2, 4});

    l.sort([](const StableItem& a, const StableItem& b) {
        return a.key < b.key;
    });

    auto it = l.begin();

    assert(it->key == 1);
    assert(it->original_order == 1);
    ++it;

    assert(it->key == 1);
    assert(it->original_order == 3);
    ++it;

    assert(it->key == 2);
    assert(it->original_order == 0);
    ++it;

    assert(it->key == 2);
    assert(it->original_order == 2);
    ++it;

    assert(it->key == 2);
    assert(it->original_order == 4);
}


// ============================================================
// 38. Tracker lifetime / destructor
// ============================================================

void test_object_lifetime() {

    assert(Tracker::alive == 0);

    {
        List<Tracker> l;

        l.emplace_back(1);
        l.emplace_back(2);
        l.emplace_front(0);

        assert(Tracker::alive == 3);

        l.pop_front();

        assert(Tracker::alive == 2);

        l.pop_back();

        assert(Tracker::alive == 1);

        l.clear();

        assert(Tracker::alive == 0);

        l.emplace_back(100);

        assert(Tracker::alive == 1);
    }

    assert(Tracker::alive == 0);
}


// ============================================================
// 39. Copy with custom object
// ============================================================

void test_custom_object_copy() {

    Tracker::copies = 0;
    Tracker::moves = 0;

    {
        List<Tracker> a;

        a.emplace_back(1);
        a.emplace_back(2);
        a.emplace_back(3);

        List<Tracker> b(a);

        assert(a.size() == 3);
        assert(b.size() == 3);

        assert(Tracker::copies >= 3);

        b.front().value = 999;

        assert(a.front().value == 1);
        assert(b.front().value == 999);
    }

    assert(Tracker::alive == 0);
}


// ============================================================
// 40. Move-only type
// ============================================================

struct MoveOnly {
    int value;

    explicit MoveOnly(int v)
            : value(v) {}

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) noexcept = default;
    MoveOnly& operator=(MoveOnly&&) noexcept = default;
};

void test_move_only_type() {

    List<MoveOnly> l;

    l.emplace_back(1);
    l.emplace_front(0);

    MoveOnly x(2);

    l.push_back(std::move(x));

    assert(l.size() == 3);

    auto it = l.begin();

    assert(it->value == 0);
    ++it;

    assert(it->value == 1);
    ++it;

    assert(it->value == 2);
}


// ============================================================
// 41. Strings
// ============================================================

void test_strings() {

    List<std::string> l;

    l.push_back("banana");
    l.push_back("apple");
    l.push_back("orange");

    l.sort();

    auto it = l.begin();

    assert(*it++ == "apple");
    assert(*it++ == "banana");
    assert(*it++ == "orange");

    l.reverse();

    it = l.begin();

    assert(*it++ == "orange");
    assert(*it++ == "banana");
    assert(*it++ == "apple");
}


// ============================================================
// 42. Compare deterministic operations with std::list
// ============================================================

void test_against_std_list() {

    List<int> mine;
    std::list<int> stl;

    for (int i = 0; i < 1000; ++i) {

        mine.push_back(i);
        stl.push_back(i);
    }

    assert_lists_equal(mine, stl);

    for (int i = 0; i < 200; ++i) {

        mine.pop_front();
        stl.pop_front();

        mine.pop_back();
        stl.pop_back();
    }

    assert_lists_equal(mine, stl);

    mine.reverse();
    stl.reverse();

    assert_lists_equal(mine, stl);

    mine.sort();
    stl.sort();

    assert_lists_equal(mine, stl);

    mine.remove_if([](int x) {
        return x % 3 == 0;
    });

    stl.remove_if([](int x) {
        return x % 3 == 0;
    });

    assert_lists_equal(mine, stl);
}


// ============================================================
// 43. Randomized stress test against std::list
// ============================================================

void test_randomized_stress() {

    List<int> mine;
    std::list<int> stl;

    std::mt19937 rng(123456789);

    std::uniform_int_distribution<int> operation(0, 9);
    std::uniform_int_distribution<int> value_dist(-10000, 10000);

    constexpr int OPERATIONS = 100000;

    for (int step = 0; step < OPERATIONS; ++step) {

        int op = operation(rng);
        int value = value_dist(rng);

        switch (op) {

            case 0:
                mine.push_back(value);
                stl.push_back(value);
                break;

            case 1:
                mine.push_front(value);
                stl.push_front(value);
                break;

            case 2:
                if (!stl.empty()) {
                    mine.pop_back();
                    stl.pop_back();
                }
                break;

            case 3:
                if (!stl.empty()) {
                    mine.pop_front();
                    stl.pop_front();
                }
                break;

            case 4:
                mine.reverse();
                stl.reverse();
                break;

            case 5:
                if (step % 100 == 0) {
                    mine.sort();
                    stl.sort();
                }
                break;

            case 6:
                if (!stl.empty()) {

                    mine.front() = value;
                    stl.front() = value;
                }
                break;

            case 7:
                if (!stl.empty()) {

                    mine.back() = value;
                    stl.back() = value;
                }
                break;

            case 8:
                if (step % 250 == 0) {

                    mine.remove(value);
                    stl.remove(value);
                }
                break;

            case 9:
                if (step % 500 == 0) {

                    mine.unique();
                    stl.unique();
                }
                break;
        }

        // Check periodically instead of traversing both lists
        // after every single operation.
        if (step % 100 == 0)
            assert_lists_equal(mine, stl);
    }

    assert_lists_equal(mine, stl);
}


// ============================================================
// 44. Large sequential stress test
// ============================================================

void test_large_stress() {

    List<int> l;

    constexpr int N = 100000;

    for (int i = 0; i < N; ++i)
        l.push_back(i);

    assert(l.size() == N);

    int expected = 0;

    for (int x : l)
        assert(x == expected++);

    assert(expected == N);

    for (int i = 0; i < N / 2; ++i)
        l.pop_front();

    assert(l.size() == N / 2);
    assert(l.front() == N / 2);

    for (int i = 0; i < N / 2; ++i)
        l.pop_back();

    assert(l.empty());
    assert(l.size() == 0);
}


// ============================================================
// MAIN
// ============================================================

int main() {

    test_default_constructor();

    test_push();
    test_pop();
    test_front_back_mutation();
    test_const_access();

    test_forward_iterator();
    test_backward_iterator();
    test_iterator_operators();
    test_const_iterator();
    test_iterator_const_conversion();

    test_copy_constructor();
    test_copy_assignment();
    test_move_constructor();
    test_move_assignment();

    test_emplace_front_back();
    test_insert();
    test_emplace();
    test_erase();

    test_clear();
    test_resize_default();
    test_resize_value();
    test_swap();

    test_remove();
    test_remove_if();
    test_reverse();

    test_unique();
    test_unique_predicate();

    test_splice_end();
    test_splice_begin();
    test_splice_middle();
    test_splice_into_empty();
    test_splice_empty_source();
    test_self_splice();

    test_sort();
    test_sort_comparator();
    test_sort_edge_cases();
    test_sort_stability();

    test_object_lifetime();
    test_custom_object_copy();
    test_move_only_type();
    test_strings();

    test_against_std_list();
    test_randomized_stress();
    test_large_stress();

    std::cout
            << "====================================\n"
            << "All MiniSTL List tests passed!\n"
            << "====================================\n";

    return 0;
}
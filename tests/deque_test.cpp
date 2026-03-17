#include <iostream>
#include <cassert>
#include <deque>
#include <cstdlib>
#include <ctime>

#include "../include/ministl/containers/deque.hpp"

using namespace ministl;

void test_basic() {
    Deque<int> d;

    assert(d.empty());
    assert(d.size() == 0);

    d.push_back(1);
    d.push_back(2);
    d.push_front(0);

    assert(d.size() == 3);
    assert(d[0] == 0);
    assert(d[1] == 1);
    assert(d[2] == 2);

    assert(d.front() == 0);
    assert(d.back() == 2);

    d.pop_front();
    assert(d.front() == 1);

    d.pop_back();
    assert(d.back() == 1);
}

void test_copy() {
    Deque<int> d;
    for (int i = 0; i < 10; i++) d.push_back(i);

    Deque<int> copy = d;

    assert(copy.size() == d.size());
    for (size_t i = 0; i < d.size(); i++)
        assert(copy[i] == d[i]);

    copy.pop_back();
    assert(copy.size() != d.size());
}

void test_move() {
    Deque<int> d;
    for (int i = 0; i < 10; i++) d.push_back(i);

    Deque<int> moved = std::move(d);

    assert(moved.size() == 10);
    assert(moved[0] == 0);
    assert(moved[9] == 9);
}
void test_iterator() {
    Deque<int> d;
    for (int i = 0; i < 5; i++) d.push_back(i);

    int expected = 0;
    for (auto it = d.begin(); it != d.end(); ++it) {
        assert(*it == expected++);
    }
}

void test_random_access() {
    Deque<int> d;
    for (int i = 0; i < 100; i++) d.push_back(i);

    for (int i = 0; i < 100; i++) {
        assert(d[i] == i);
    }
}

void test_wraparound() {
    Deque<int> d;

    for (int i = 0; i < 50; i++) d.push_back(i);
    for (int i = 0; i < 25; i++) d.pop_front();
    for (int i = 50; i < 75; i++) d.push_back(i);

    for (int i = 0; i < 50; i++) {
        assert(d[i] == i + 25);
    }
}

void test_comparison() {
    Deque<int> a, b;

    for (int i = 0; i < 5; i++) {
        a.push_back(i);
        b.push_back(i);
    }

    assert(a == b);

    b.push_back(100);

    assert(a != b);
    assert(a < b);
    assert(b > a);
}

void test_against_std() {
    Deque<int> my;
    std::deque<int> ref;

    for (int i = 0; i < 1000; i++) {
        my.push_back(i);
        ref.push_back(i);
    }

    for (int i = 0; i < 500; i++) {
        my.pop_front();
        ref.pop_front();
    }

    for (int i = 0; i < 500; i++) {
        assert(my[i] == ref[i]);
    }
}

void test_stress() {
    Deque<int> d;
    std::deque<int> ref;

    const int OPS = 1000000;

    for (int i = 0; i < OPS; i++) {
        int action = rand() % 4;

        if (action == 0) {
            int val = rand();
            d.push_back(val);
            ref.push_back(val);
        } else if (action == 1) {
            int val = rand();
            d.push_front(val);
            ref.push_front(val);
        } else if (action == 2 && !ref.empty()) {
            d.pop_back();
            ref.pop_back();
        } else if (action == 3 && !ref.empty()) {
            d.pop_front();
            ref.pop_front();
        }

        assert(d.size() == ref.size());

        for (size_t j = 0; j < ref.size(); j++) {
            assert(d[j] == ref[j]);
        }
    }
}

int main() {
    srand(time(nullptr));

    test_basic();
    test_copy();
    test_move();
    test_iterator();
    test_random_access();
    test_wraparound();
    test_comparison();
    test_against_std();
    test_stress();

    std::cout << "All List tests passed!\n";
    return 0;
}
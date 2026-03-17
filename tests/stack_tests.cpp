#include <iostream>
#include <cassert>
#include <stack>
#include <cstdlib>
#include <ctime>

#include "../include/ministl/containers/stack.hpp"

using namespace ministl;

void test_basic() {
    Stack<int> s;

    assert(s.empty());
    assert(s.size() == 0);

    s.push(10);
    s.push(20);
    s.push(30);

    assert(!s.empty());
    assert(s.size() == 3);
    assert(s.top() == 30);

    s.pop();
    assert(s.top() == 20);

    s.pop();
    s.pop();

    assert(s.empty());
}

void test_copy() {
    Stack<int> s;
    for (int i = 0; i < 5; i++) s.push(i);

    Stack<int> copy = s;

    assert(copy.size() == s.size());
    assert(copy.top() == s.top());

    copy.pop();
    assert(copy.top() != s.top()); // deep copy
}

void test_move() {
    Stack<int> s;
    for (int i = 0; i < 5; i++) s.push(i);

    Stack<int> moved = std::move(s);

    assert(moved.size() == 5);
    assert(moved.top() == 4);
}

void test_emplace() {
    Stack<std::pair<int,int>> s;

    s.emplace(1,2);
    s.emplace(3,4);

    assert(s.top().first == 3);
    assert(s.top().second == 4);
}

void test_large() {
    Stack<int> s;
    const int N = 10000000;

    for (int i = 0; i < N; i++) s.push(i);

    assert(s.size() == N);

    for (int i = N - 1; i >= 0; i--) {
        assert(s.top() == i);  // LIFO check
        s.pop();
    }

    assert(s.empty());
}

void test_comparison() {
    Stack<int> a, b;

    for (int i = 0; i < 5; i++) {
        a.push(i);
        b.push(i);
    }

    assert(a == b);

    b.push(100);

    assert(a != b);
    assert(a < b);
    assert(b > a);
}

void test_swap() {
    Stack<int> a, b;

    a.push(1);
    a.push(2);

    b.push(10);

    a.swap(b);

    assert(a.size() == 1);
    assert(a.top() == 10);

    assert(b.size() == 2);
    assert(b.top() == 2);
}

void test_against_std() {
    Stack<int> my;
    std::stack<int> ref;

    for (int i = 0; i < 1000; i++) {
        my.push(i);
        ref.push(i);
    }

    while (!ref.empty()) {
        assert(my.top() == ref.top());
        my.pop();
        ref.pop();
    }

    assert(my.empty());
}

void test_stress() {
    Stack<int> s;
    std::stack<int> ref;

    const int OPS = 100000000;

    for (int i = 0; i < OPS; i++) {
        int action = rand() % 3;

        if (action == 0 || ref.empty()) {
            int val = rand();
            s.push(val);
            ref.push(val);
        } else if (action == 1) {
            s.pop();
            ref.pop();
        } else {
            assert(s.top() == ref.top());
        }

        assert(s.size() == ref.size());
        assert(s.empty() == ref.empty());
    }

    while (!ref.empty()) {
        assert(s.top() == ref.top());
        s.pop();
        ref.pop();
    }

    assert(s.empty());
}

int main() {
    srand(time(nullptr));

    test_basic();
    test_copy();
    test_move();
    test_emplace();
    test_large();
    test_comparison();
    test_swap();
    test_against_std();
    test_stress();

    std::cout << "All MiniSTL stack tests passed!\n";
}
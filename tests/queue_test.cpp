#include <cassert>
#include <queue>
#include <random>
#include <iostream>

#include "../include/ministl/containers/queue.hpp"

using namespace ministl;

void test_basic() {
    Queue<int> q;

    assert(q.empty());
    assert(q.size() == 0);

    q.push(10);
    q.push(20);
    q.push(30);

    assert(!q.empty());
    assert(q.size() == 3);

    assert(q.front() == 10);
    assert(q.back() == 30);

    q.pop();
    assert(q.front() == 20);

    q.pop();
    assert(q.front() == 30);

    q.pop();
    assert(q.empty());
}

void test_fifo() {
    Queue<int> q;

    for (int i = 0; i < 100; i++)
        q.push(i);

    for (int i = 0; i < 100; i++) {
        assert(q.front() == i);
        q.pop();
    }

    assert(q.empty());
}

void test_emplace() {
    Queue<std::pair<int, int>> q;

    q.emplace(1, 2);
    q.emplace(3, 4);

    assert(q.front().first == 1);
    assert(q.front().second == 2);

    q.pop();

    assert(q.front().first == 3);
    assert(q.front().second == 4);
}

void test_move() {
    Queue<std::string> q;

    std::string s = "hello";
    q.push(std::move(s));

    assert(q.front() == "hello");
}

void test_comparison() {
    Queue<int> q1, q2;

    for (int i = 0; i < 10; i++) {
        q1.push(i);
        q2.push(i);
    }

    assert(q1 == q2);

    q2.pop();
    assert(q1 != q2);
}

void test_swap() {
    Queue<int> q1, q2;

    q1.push(1);
    q1.push(2);

    q2.push(10);

    q1.swap(q2);

    assert(q1.size() == 1);
    assert(q1.front() == 10);

    assert(q2.size() == 2);
    assert(q2.front() == 1);
}

void test_vs_std_queue() {
    Queue<int> myq;
    std::queue<int> stdq;

    for (int i = 0; i < 1000; i++) {
        myq.push(i);
        stdq.push(i);
    }

    while (!stdq.empty()) {
        assert(myq.front() == stdq.front());
        myq.pop();
        stdq.pop();
    }

    assert(myq.empty());
}

void test_random_stress() {
    Queue<int> myq;
    std::queue<int> stdq;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> op(0, 2);
    std::uniform_int_distribution<int> val(0, 10000);

    for (int i = 0; i < 100000; i++) {
        int operation = op(rng);

        if (operation == 0 && !stdq.empty()) {
            // pop
            assert(myq.front() == stdq.front());
            myq.pop();
            stdq.pop();
        } else {
            // push
            int v = val(rng);
            myq.push(v);
            stdq.push(v);
        }

        assert(myq.size() == stdq.size());

        if (!stdq.empty()) {
            assert(myq.front() == stdq.front());
            assert(myq.back() == stdq.back());
        }
    }
}

int main() {
    test_basic();
    test_fifo();
    test_emplace();
    test_move();
    test_comparison();
    test_swap();
    test_vs_std_queue();
    test_random_stress();

    std::cout << "All List tests passed!\n";
    return 0;
}
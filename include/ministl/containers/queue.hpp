#pragma once

#include "deque.hpp"

namespace ministl {

    template<typename T, typename Container = Deque<T>>
    class Queue {
    private:
        Container c;

    public:
        bool empty() const { return c.empty(); }

        size_t size() const { return c.size(); }

        T &front() { return c.front(); }

        T &back() { return c.back(); }

        void push(const T &val) { c.push_back(val); }

        void pop() { c.pop_front(); }
    };
} // namespace ministl
#pragma once

#include "deque.hpp"
#include <utility>

/*
    MiniSTL - Queue Adapter
    -----------------------
    A simplified implementation of std::queue for educational purposes.

    Features:
    - FIFO (First-In-First-Out) semantics
    - Container adapter (uses underlying container, default: Deque)
    - Rule of Zero (no manual memory management)
    - Supports push / emplace / pop
    - Constant time front/back access

    Requirements on underlying Container:
    - empty()
    - size()
    - front()
    - back()
    - push_back()
    - pop_front()
    - emplace_back()

    Complexity:
    - push / emplace: amortized O(1)
    - pop: O(1)
    - front / back: O(1)

    Author: Adam Abu Saleh
*/

namespace ministl {

    template<typename T, typename Container = Deque<T>>
    class Queue {
    private:
        // underlying container storing the elements
        Container c;

    public:

        bool empty() const { return c.empty(); }

        // Returns the number of elements
        size_t size() const { return c.size(); }

        // Access the first element
        T& front() { return c.front(); }

        // Const access to the first element
        const T& front() const { return c.front(); }

        // Access the last element
        T& back() { return c.back(); }

        // Const access to the last element
        const T& back() const { return c.back(); }

        // Insert element at the back (copy)
        void push(const T& val) { c.push_back(val); }

        // Insert element at the back (move)
        void push(T&& val) { c.push_back(std::move(val)); }

        // Construct element in-place at the back
        template<typename... Args>
        void emplace(Args&&... args) { c.emplace_back(std::forward<Args>(args)...); }

        // Remove the front element (does not return it)
        void pop() { c.pop_front(); }

        // Swap contents with another queue
        void swap(Queue& other) noexcept { std::swap(c, other.c); }

        // Equality comparison
        bool operator==(const Queue& other) const { return c == other.c; }

        // Inequality comparison
        bool operator!=(const Queue& other) const { return !(*this == other); }
    };

} // namespace ministl
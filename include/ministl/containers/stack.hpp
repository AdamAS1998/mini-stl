/*
    MiniSTL - Stack Adapter
    -----------------------
    A simplified implementation of std::stack.

    Stack is a container adapter providing LIFO behavior (Last In First Out).
    It does NOT store elements itself, but delegates storage to an underlying container.

    Conceptually:
        Stack<T, Container>
            ↓
        wraps Container and restricts access

    Required operations from the underlying container:
        - back()        → access last element
        - push_back()   → insert at end
        - pop_back()    → remove last
        - size()        → number of elements
        - empty()       → check if empty

    Default container:
        Vector<T>
    (In STL it's deque<T> because it's more flexible for front/back operations so maybe in the future ill fix this :D)

    Author: Adam Abu Saleh
*/

#pragma once

#include <cstddef>   // std::size_t
#include <utility>   // std::move, std::forward
#include "vector.hpp"

namespace ministl {

/*
    Stack<T, Container>

    Template Parameters:
        T         → type of elements
        Container → underlying storage (default: Vector<T>)

    The container must support:
        back(), push_back(), pop_back(), size(), empty()
*/
    template<typename T, typename Container = Vector<T>>
    class Stack {

    public:

        using value_type = T;              // type of elements stored
        using container_type = Container; // underlying container type
        using size_type = std::size_t;    // size representation

    private:
    /*
        Underlying container

        This is where all elements are actually stored.

        Why private?
            - Prevents users from breaking LIFO behavior
            - Forces usage through stack interface (push/pop/top)

        STL uses 'protected' here to allow inheritance,
        but for simplicity and safety we keep it private.
    */
        container_type c;

    public:

        // Default constructor → creates empty stack
        Stack() = default;

        // Construct from a copy of an existing container
        explicit Stack(const container_type& cont)
                : c(cont) {}

        // Construct by moving an existing container (efficient)
        explicit Stack(container_type&& cont)
                : c(std::move(cont)) {}

        // Check if stack is empty
        bool empty() const { return c.empty(); }

        // Number of elements
        size_type size() const { return c.size(); }

        // Access top element
        value_type& top() { return c.back(); }

        // Access top element (read-only)
        const value_type& top() const { return c.back(); }

        // Push by copy
        void push(const value_type& value) { c.push_back(value); }

        // Push by move (avoids copy, more efficient)
        void push(value_type&& value) { c.push_back(std::move(value)); }

    /*
        Construct element in-place at top
        Perfect forwarding:
            Allows constructing T directly inside container
            without temporary objects
    */
        template<class... Args>
        void emplace(Args&&... args) {
            c.emplace_back(std::forward<Args>(args)...);
        }

        // Remove top element
        void pop() { c.pop_back(); }


        void swap(Stack& other) noexcept {std::swap(c, other.c);}

        // Delegated to underlying container comparisons
        friend bool operator==(const Stack& l, const Stack& r) {
            return l.c == r.c;
        }

        friend bool operator!=(const Stack& l, const Stack& r) {
            return !(l == r);
        }

        friend bool operator<(const Stack& l, const Stack& r) {
            return l.c < r.c;
        }

        friend bool operator<=(const Stack& l, const Stack& r) {
            return !(r < l);
        }

        friend bool operator>(const Stack& l, const Stack& r) {
            return r < l;
        }

        friend bool operator>=(const Stack& l, const Stack& r) {
            return !(l < r);
        }
    };

} // namespace ministl
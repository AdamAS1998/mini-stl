/*
    MiniSTL - Stack Adapter
    -----------------------
    A simplified implementation of std::stack.

    Stack is a container adapter providing LIFO behavior
    (Last In, First Out).

    It does not manage storage itself. Instead, it delegates
    storage and operations to an underlying container.

    Conceptually:

        Stack<T, Container>
                    |
                    v
        underlying Container

    Required operations from the underlying container:
        - back()        -> access last element
        - push_back()   -> insert at end
        - pop_back()    -> remove last element
        - emplace_back()-> construct element at end
        - size()        -> number of elements
        - empty()       -> check if empty
        - swap()        -> exchange contents

    Default container:
        Deque<T>

    Complexity with Deque<T>:
        - top()     : O(1)
        - push()    : amortized O(1)
        - emplace() : amortized O(1)
        - pop()     : O(1)
        - size()    : O(1)
        - empty()   : O(1)
        - swap()    : O(1)

    Author: Adam Abu Saleh
*/

#pragma once

#include <utility>   // std::move, std::forward
#include "deque.hpp"

namespace ministl {

/*
    Stack<T, Container>

    Template parameters:
        T         -> type of elements stored
        Container -> underlying container type

    The default underlying container is MiniSTL Deque<T>.

    Stack restricts access to the underlying container in order
    to preserve LIFO behavior. Elements can only be added,
    removed, or accessed from the top of the stack.
*/
    template<typename T, typename Container = Deque<T>>
    class Stack {

    public:

        using value_type = T;
        using container_type = Container;
        using size_type = typename container_type::size_type;
        using reference = value_type&;
        using const_reference = const value_type&;

    private:

    /*
        Underlying container.

        All elements are actually stored here.

        Keeping it private prevents users from directly modifying
        the container and breaking the LIFO behavior of the stack.
    */
        container_type c;

    public:

        // Default constructor -> creates an empty stack
        Stack() = default;

        // Construct stack by copying an existing container
        explicit Stack(const container_type& cont)
                : c(cont) {}

        // Construct stack by moving an existing container
        explicit Stack(container_type&& cont)
                : c(std::move(cont)) {}


        // Check whether the stack is empty
        bool empty() const {
            return c.empty();
        }

        // Return number of elements
        size_type size() const {
            return c.size();
        }


        // Access top element
        reference top() {
            return c.back();
        }

        // Access top element (read-only)
        const_reference top() const {
            return c.back();
        }


        // Push element by copy
        void push(const value_type& value) {
            c.push_back(value);
        }

        // Push element by move
        void push(value_type&& value) {
            c.push_back(std::move(value));
        }


    /*
        emplace()

        Constructs a new element directly at the top of the stack.

        Arguments are perfectly forwarded to the constructor of T,
        avoiding the need to create a temporary object first.

        Example:
            Stack<std::pair<int, int>> s;
            s.emplace(10, 20);
    */
        template<typename... Args>
        void emplace(Args&&... args) {
            c.emplace_back(std::forward<Args>(args)...);
        }


        // Remove top element
        void pop() {
            c.pop_back();
        }


        // Exchange contents with another stack
        void swap(Stack& other) noexcept {
            c.swap(other.c);
        }


    /*
        Comparison operators

        Stack comparisons are delegated to the underlying container.

        Since Deque compares elements lexicographically,
        Stack comparisons behave the same way.
    */

        friend bool operator==(const Stack& lhs, const Stack& rhs) {
            return lhs.c == rhs.c;
        }

        friend bool operator!=(const Stack& lhs, const Stack& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const Stack& lhs, const Stack& rhs) {
            return lhs.c < rhs.c;
        }

        friend bool operator<=(const Stack& lhs, const Stack& rhs) {
            return !(rhs < lhs);
        }

        friend bool operator>(const Stack& lhs, const Stack& rhs) {
            return rhs < lhs;
        }

        friend bool operator>=(const Stack& lhs, const Stack& rhs) {
            return !(lhs < rhs);
        }
    };

} // namespace ministl
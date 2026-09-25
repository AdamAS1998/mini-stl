/*
    MiniSTL - Queue Adapter
    -----------------------
    A simplified implementation of std::queue.

    Queue is a container adapter providing FIFO behavior
    (First In, First Out).

    It does not manage storage itself. Instead, it delegates
    storage and operations to an underlying container.

    Conceptually:

        Queue<T, Container>
                    |
                    v
        underlying Container

    Required operations from the underlying container:
        - front()       -> access first element
        - back()        -> access last element
        - push_back()   -> insert at end
        - pop_front()   -> remove first element
        - emplace_back()-> construct element at end
        - size()        -> number of elements
        - empty()       -> check if empty
        - swap()        -> exchange contents

    Default container:
        Deque<T>

    Complexity with Deque<T>:
        - front()   : O(1)
        - back()    : O(1)
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
    Queue<T, Container>

    Template parameters:
        T         -> type of elements stored
        Container -> underlying container type

    The default underlying container is MiniSTL Deque<T>.

    Queue restricts access to the underlying container in order
    to preserve FIFO behavior.

    Elements are inserted at the back and removed from the front.
*/
    template<typename T, typename Container = Deque<T>>
    class Queue {

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
        the container and breaking the FIFO behavior of the queue.
    */
        container_type c;

    public:

        // Default constructor -> creates an empty queue
        Queue() = default;

        // Construct queue by copying an existing container
        explicit Queue(const container_type& cont)
                : c(cont) {}

        // Construct queue by moving an existing container
        explicit Queue(container_type&& cont)
                : c(std::move(cont)) {}


        // Check whether the queue is empty
        bool empty() const {
            return c.empty();
        }

        // Return number of elements
        size_type size() const {
            return c.size();
        }


        // Access first element
        reference front() {
            return c.front();
        }

        // Access first element (read-only)
        const_reference front() const {
            return c.front();
        }


        // Access last element
        reference back() {
            return c.back();
        }

        // Access last element (read-only)
        const_reference back() const {
            return c.back();
        }


        // Insert element at the back by copy
        void push(const value_type& value) {
            c.push_back(value);
        }

        // Insert element at the back by move
        void push(value_type&& value) {
            c.push_back(std::move(value));
        }


    /*
        emplace()

        Constructs a new element directly at the back of the queue.

        Arguments are perfectly forwarded to the constructor of T,
        avoiding the need to create a temporary object first.

        Example:
            Queue<std::pair<int, int>> q;
            q.emplace(10, 20);
    */
        template<typename... Args>
        void emplace(Args&&... args) {
            c.emplace_back(std::forward<Args>(args)...);
        }


        // Remove first element
        void pop() {
            c.pop_front();
        }


        // Exchange contents with another queue
        void swap(Queue& other) noexcept {
            c.swap(other.c);
        }


    /*
        Comparison operators

        Queue comparisons are delegated to the underlying container.

        Since Deque compares elements lexicographically,
        Queue comparisons behave the same way.
    */

        friend bool operator==(const Queue& lhs, const Queue& rhs) {
            return lhs.c == rhs.c;
        }

        friend bool operator!=(const Queue& lhs, const Queue& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const Queue& lhs, const Queue& rhs) {
            return lhs.c < rhs.c;
        }

        friend bool operator<=(const Queue& lhs, const Queue& rhs) {
            return !(rhs < lhs);
        }

        friend bool operator>(const Queue& lhs, const Queue& rhs) {
            return rhs < lhs;
        }

        friend bool operator>=(const Queue& lhs, const Queue& rhs) {
            return !(lhs < rhs);
        }
    };

} // namespace ministl
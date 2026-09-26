/*
    MiniSTL - Priority Queue Adapter
    --------------------------------
    A simplified implementation of std::priority_queue.

    PriorityQueue is a container adapter that maintains its elements
    as a binary heap.

    By default, the largest element has the highest priority and is
    stored at the top of the heap.

    Conceptually, the underlying container represents a complete
    binary tree.

    For an element at index i:
        parent      = (i - 1) / 2
        left child  = 2 * i + 1
        right child = 2 * i + 2

    Default container:
        Vector<T>

    Default comparator:
        std::less<T>

    With std::less<T>, the largest element is placed at the top.

    Main operations:
        - top()     : O(1)
        - push()    : O(log n)
        - emplace() : O(log n)
        - pop()     : O(log n)
        - size()    : O(1)
        - empty()   : O(1)

    Space:
        - O(n) for stored elements
        - O(1) auxiliary space for heap operations

    Author: Adam Abu Saleh
*/

#pragma once

#include <cstddef>
#include <functional>
#include <utility>

#include "vector.hpp"

namespace ministl {

/*
    PriorityQueue<T, Container, Compare>

    Template parameters:

        T Type of elements stored.

        Container
            Underlying container used to store the heap.
            Default: MiniSTL Vector<T>.

        Compare
            Determines element priority.
            Default: std::less<T> -> max heap.

    The comparator follows the same idea as std::priority_queue:

        comp(a, b) == true

    means that 'a' has lower priority than 'b'.

    Therefore:

        std::less<T>    -> max heap
        std::greater<T> -> min heap
*/
    template<
            typename T,
            typename Container = Vector<T>,
            typename Compare = std::less<T>
    >
    class PriorityQueue {

    public:

        using value_type = T;
        using container_type = Container;
        using value_compare = Compare;
        using const_reference = const value_type&;

    private:

        // Underlying container storing the binary heap.
        container_type c;

        // Comparator controlling element priority.
        value_compare comp;


    /*
        sift_up()

        Restores the heap property after inserting a new element.

        The new element begins at the last position of the heap.
        While it has higher priority than its parent, the two
        elements are swapped.

        Complexity: Time: O(log n) Space: O(1)
    */
        void sift_up(size_t index) {

            while (index > 0) {

                size_t parent = (index - 1) / 2;

                // If parent already has at least as much priority,
                // the heap property is satisfied.
                if (!comp(c[parent], c[index]))
                    break;

                std::swap(c[parent], c[index]);

                index = parent;
            }
        }


    /*
        sift_down()

        Restores the heap property after removing the top element.

        Starting from the root, the element is repeatedly swapped
        with the higher-priority child until the heap property is
        restored.

        Complexity: Time: O(log n) Space: O(1)
    */
        void sift_down(size_t index) {

            const size_t n = c.size();

            while (true) {

                size_t left = 2 * index + 1;
                size_t right = 2 * index + 2;

                // Assume the current node has the highest priority.
                size_t best = index;

                // Check left child.
                if (left < n && comp(c[best], c[left]))
                    best = left;

                // Check right child.
                if (right < n && comp(c[best], c[right]))
                    best = right;

                // Heap property already satisfied.
                if (best == index)
                    break;

                std::swap(c[index], c[best]);

                index = best;
            }
        }


    /*
        build_heap()

        Converts the current contents of the underlying container
        into a valid binary heap.

        Leaves are already valid one-element heaps, so we begin at
        the last internal node and sift each internal node downward.

        Complexity: Time : O(n) Space: O(1)
    */
        void build_heap() {

            if (c.size() < 2)
                return;

            // Last internal node = floor(n / 2) - 1
            // Writing the loop this way avoids unsigned underflow.
            for (size_t i = c.size() / 2; i > 0; --i)
                sift_down(i - 1);
        }


    public:

        // Default constructor -> creates an empty priority queue.
        PriorityQueue() = default;


        // Construct an empty priority queue with a custom comparator.
        explicit PriorityQueue(const value_compare& compare)
                : c(), comp(compare) {}


    /*
        Construct from an existing container by copy.

        The container may contain elements in any order.
        build_heap() rearranges them into a valid heap.

        Complexity: Time : O(n) Space: O(n) for the copied container
    */
        PriorityQueue(
                const value_compare& compare,
                const container_type& cont
        )
                : c(cont), comp(compare) {

            build_heap();
        }


    /*
        Construct from an existing container by move.

        The container storage is transferred and then rearranged
        into a valid heap.

        Complexity: Time : O(n) Extra heap-building space: O(1)
    */
        PriorityQueue(
                const value_compare& compare,
                container_type&& cont
        )
                : c(std::move(cont)), comp(compare) {

            build_heap();
        }


        // Check whether the priority queue is empty.
        bool empty() const {
            return c.empty();
        }


        // Return the number of stored elements.
        size_t size() const {
            return c.size();
        }


    /*
        Return the highest-priority element.

        For the default std::less<T> comparator,
        this is the largest element.

        Complexity: Time: O(1)
    */
        const_reference top() const {
            return c.front();
        }


    /*
        Insert an element by copy.

        The element is first inserted at the end of the
        underlying container and then moved upward until
        the heap property is restored.

        Complexity: Time: O(log n)
    */
        void push(const value_type& value) {

            c.push_back(value);

            sift_up(c.size() - 1);
        }


    /*
        Insert an element by move.

        Complexity: Time: O(log n)
    */
        void push(value_type&& value) {

            c.push_back(std::move(value));

            sift_up(c.size() - 1);
        }


    /*
        Construct a new element directly inside the underlying
        container and restore the heap property.

        Complexity: Time: O(log n)
    */
        template<typename... Args>
        void emplace(Args&&... args) {

            c.emplace_back(std::forward<Args>(args)...);

            sift_up(c.size() - 1);
        }


    /*
        Remove the highest-priority element.

        The root is swapped with the last element.

        The last element is then removed and the new root
        is moved downward until the heap property is restored.

        Complexity: Time: O(log n)
    */
        void pop() {

            if (c.empty())
                return;

            if (c.size() == 1) {
                c.pop_back();
                return;
            }

            std::swap(c.front(), c.back());

            c.pop_back();

            sift_down(0);
        }


    /*
        Exchange the contents and comparator with another
        priority queue.

        Complexity: Container swap: O(1) for MiniSTL Vector
    */
        void swap(PriorityQueue& other) {

            c.swap(other.c);
            std::swap(comp, other.comp);
        }
    };

} // namespace ministl
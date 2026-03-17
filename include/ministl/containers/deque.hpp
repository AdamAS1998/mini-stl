/*
    MiniSTL - Deque Implementation
    ------------------------------
    A simplified implementation of std::deque using a circular buffer.

    Features:
    - Double-ended queue (push/pop front and back in O(1))
    - Random access in O(1)
    - Rule of Five (copy/move semantics)
    - Iterator support (random-access style)
    - Lexicographical comparison operators

    Design:
    - Uses a dynamic circular buffer
    - Maintains a front index and size
    - Logical indexing maps to physical index via modulo arithmetic

    Complexity guarantees:
    - push_front / push_back: amortized O(1)
    - pop_front / pop_back: O(1)
    - operator[]: O(1)

    Notes:
    - Simpler than std::deque (does not use segmented blocks)

    Author: Adam Abu Saleh
*/

#pragma once

#include <cstddef>
#include <utility>
#include <cassert>
#include <algorithm>
#include <stdexcept>

namespace ministl {

/*
    Deque<T>

    A double-ended queue implemented using a circular buffer.

    Supports:
    - O(1) push_front / push_back
    - O(1) pop_front / pop_back
    - O(1) random access via operator[]

    Internally:
    - data_     → raw dynamic array
    - size_     → number of elements
    - capacity_ → allocated storage
    - front_    → index of logical first element

    Logical index i maps to:
        (front_ + i) % capacity_
*/
    template<typename T>
    class Deque {

    private:
        T* data_;          // underlying storage
        size_t size_;      // number of elements
        size_t capacity_;  // allocated capacity
        size_t front_;     // index of first element

        // Maps logical index to physical index in array
        size_t index(size_t i) const {
            return (front_ + i) % capacity_;
        }

    /*
        grow()

        Resizes the container when full.
        - Allocates new array (usually double capacity)
        - Moves elements in correct logical order
        - Resets front_ to 0 for clean layout
    */
        void grow() {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            T* new_data = new T[new_capacity];

            for (size_t i = 0; i < size_; i++)
                new_data[i] = std::move((*this)[i]);

            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
            front_ = 0;
        }

    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = T&;
        using const_reference = const T&;

    /*
        iterator

        Random-access style iterator over deque.
        Internally stores logical position (not raw pointer),
        so it works correctly with circular buffer layout.
    */
        class iterator {
        private:
            Deque* d_;
            size_t pos_;

        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;

            iterator(Deque* d = nullptr, size_t pos = 0)
                    : d_(d), pos_(pos) {}

            reference operator*() const { return (*d_)[pos_]; }
            pointer operator->() const { return &(*d_)[pos_]; }

            iterator& operator++() { pos_++; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

            iterator& operator--() { pos_--; return *this; }

            iterator operator+(difference_type n) const { return iterator(d_, pos_ + n); }
            iterator operator-(difference_type n) const { return iterator(d_, pos_ - n); }

            difference_type operator-(const iterator& other) const { return pos_ - other.pos_; }

            bool operator==(const iterator& other) const { return pos_ == other.pos_; }
            bool operator!=(const iterator& other) const { return pos_ != other.pos_; }
            bool operator<(const iterator& other) const { return pos_ < other.pos_; }
            bool operator>(const iterator& other) const { return pos_ > other.pos_; }
            bool operator<=(const iterator& other) const { return pos_ <= other.pos_; }
            bool operator>=(const iterator& other) const { return pos_ >= other.pos_; }
        };

    /*
        const_iterator
        Same as iterator but provides read-only access.
    */
        class const_iterator {
        private:
            const Deque* d_;
            size_t pos_;

        public:
            const_iterator(const Deque* d = nullptr, size_t pos = 0)
                    : d_(d), pos_(pos) {}

            const_reference operator*() const { return (*d_)[pos_]; }

            const_iterator& operator++() { pos_++; return *this; }

            bool operator==(const const_iterator& other) const { return pos_ == other.pos_; }
            bool operator!=(const const_iterator& other) const { return pos_ != other.pos_; }
        };

        // Default constructor → empty deque
        Deque() : data_(nullptr), size_(0), capacity_(0), front_(0) {}

        // Destructor
        ~Deque() { delete[] data_; }

        // Copy constructor (deep copy)
        Deque(const Deque& other)
                : data_(nullptr), size_(other.size_), capacity_(other.capacity_), front_(0) {
            data_ = new T[capacity_];
            for (size_t i = 0; i < size_; i++)
                data_[i] = other[i];
        }

        // Copy assignment
        Deque& operator=(const Deque& other) {
            if (this == &other) return *this;

            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            front_ = 0;

            data_ = new T[capacity_];
            for (size_t i = 0; i < size_; i++)
                data_[i] = other[i];

            return *this;
        }

        // Move constructor (transfers ownership)
        Deque(Deque&& other) noexcept
                : data_(other.data_), size_(other.size_),
                  capacity_(other.capacity_), front_(other.front_) {
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
            other.front_ = 0;
        }

        // Move assignment (releases current and steals resources)
        Deque& operator=(Deque&& other) noexcept {
            if (this == &other) return *this;

            delete[] data_;

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            front_ = other.front_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
            other.front_ = 0;

            return *this;
        }

        size_type size() const { return size_; }
        bool empty() const { return size_ == 0; }

        reference operator[](size_type i) {
            assert(i < size_);
            return data_[index(i)];
        }

        const_reference operator[](size_type i) const {
            assert(i < size_);
            return data_[index(i)];
        }

        // Bounds-checked access
        reference at(size_type i) {
            if (i >= size_) throw std::out_of_range("Deque::at");
            return (*this)[i];
        }

        // Access first element
        reference front() {
            assert(!empty());
            return data_[front_];
        }

        // Access last element
        reference back() {
            assert(!empty());
            return data_[index(size_ - 1)];
        }

        iterator begin() { return iterator(this, 0); }
        iterator end() { return iterator(this, size_); }

        const_iterator begin() const { return const_iterator(this, 0); }
        const_iterator end() const { return const_iterator(this, size_); }

        // Insert at back
        void push_back(const T& value) {
            if (size_ == capacity_) grow();
            data_[index(size_)] = value;
            size_++;
        }

        // Insert at front
        void push_front(const T& value) {
            if (size_ == capacity_) grow();
            front_ = (front_ == 0 ? capacity_ - 1 : front_ - 1);
            data_[front_] = value;
            size_++;
        }

        // Remove last element
        void pop_back() {
            assert(!empty());
            size_--;
        }

        // Remove first element
        void pop_front() {
            assert(!empty());
            front_ = (front_ + 1) % capacity_;
            size_--;
        }

        // Clear all elements (keeps capacity)
        void clear() {
            size_ = 0;
            front_ = 0;
        }

        // Swap contents with another deque
        void swap(Deque& other) noexcept {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
            std::swap(front_, other.front_);
        }

        // Equal comparison
        friend bool operator==(const Deque& a, const Deque& b) {
            if (a.size_ != b.size_) return false;
            for (size_t i = 0; i < a.size_; i++)
                if (!(a[i] == b[i])) return false;
            return true;
        }

        // Lexicographical comparison
        friend bool operator<(const Deque& a, const Deque& b) {
            size_t n = std::min(a.size_, b.size_);
            for (size_t i = 0; i < n; i++) {
                if (a[i] < b[i]) return true;
                if (b[i] < a[i]) return false;
            }
            return a.size_ < b.size_;
        }

        friend bool operator!=(const Deque& a, const Deque& b) { return !(a == b); }
        friend bool operator<=(const Deque& a, const Deque& b) { return !(b < a); }
        friend bool operator>(const Deque& a, const Deque& b) { return b < a; }
        friend bool operator>=(const Deque& a, const Deque& b) { return !(a < b); }
    };

} // namespace ministl
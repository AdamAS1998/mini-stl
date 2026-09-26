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
    - Simpler than std::deque (does not use segmented blocks) Something to optimize in the future

    Author: Adam Abu Saleh
*/

#pragma once

#include <cstddef>
#include <utility>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <new>

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

            T* new_data = static_cast<T*>(
                    ::operator new(sizeof(T) * new_capacity)
            );

            for (size_t i = 0; i < size_; ++i) {
                new (new_data + i) T(std::move((*this)[i]));
            }

            for (size_t i = 0; i < size_; ++i) {
                data_[index(i)].~T();
            }

            ::operator delete(data_);

            data_ = new_data;
            capacity_ = new_capacity;
            front_ = 0;
        }

    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = T&;
        using const_reference = const T&;

        class const_iterator; //forward declaration
    /*
        iterator

        Random-access style iterator over deque.
        Internally stores logical position (not raw pointer),
        so it works correctly with circular buffer layout.
    */
        class iterator {
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::random_access_iterator_tag;
        private:
            Deque* d_;
            difference_type pos_;
        public:
            iterator(Deque* d = nullptr, difference_type pos = 0)
                    : d_(d), pos_(pos) {}

            reference operator*() const {return (*d_)[static_cast<size_type>(pos_)];}
            pointer operator->() const {return &(*d_)[static_cast<size_type>(pos_)];}
            iterator& operator++() { pos_++; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

            iterator& operator--() { pos_--; return *this; }
            iterator operator--(int) {
                iterator tmp = *this;
                --(*this);
                return tmp;
            }

            iterator& operator+=(difference_type n) {
                pos_ += n;
                return *this;
            }

            iterator& operator-=(difference_type n) {
                pos_ -= n;
                return *this;
            }

            reference operator[](difference_type n) const {
                return (*d_)[static_cast<size_type>(pos_ + n)];
            }
            iterator operator+(difference_type n) const { return iterator(d_, pos_ + n); }
            iterator operator-(difference_type n) const { return iterator(d_, pos_ - n); }

            difference_type operator-(const iterator& other) const {return pos_ - other.pos_;}

            bool operator==(const iterator& other) const {
                return d_ == other.d_ && pos_ == other.pos_;
            }

            bool operator!=(const iterator& other) const {
                return !(*this == other);
            }

            bool operator<(const iterator& other) const { return pos_ < other.pos_; }
            bool operator>(const iterator& other) const { return pos_ > other.pos_; }
            bool operator<=(const iterator& other) const { return pos_ <= other.pos_; }
            bool operator>=(const iterator& other) const { return pos_ >= other.pos_; }

            friend class const_iterator;
        };

    /*
        const_iterator
        Same as iterator but provides read-only access.
    */
    class const_iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::random_access_iterator_tag;

    private:
        const Deque* d_;
        difference_type pos_;

    public:
        const_iterator(const Deque* d = nullptr, difference_type pos = 0)
                : d_(d), pos_(pos) {}
        const_iterator(const iterator& other)
                : d_(other.d_), pos_(other.pos_) {}
        reference operator*() const {return (*d_)[static_cast<size_type>(pos_)];}

        pointer operator->() const {return &(*d_)[static_cast<size_type>(pos_)];}

        const_iterator& operator++() {
            ++pos_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        const_iterator& operator--() {
            --pos_;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        const_iterator& operator+=(difference_type n) {
            pos_ += n;
            return *this;
        }

        const_iterator& operator-=(difference_type n) {
            pos_ -= n;
            return *this;
        }

        const_iterator operator+(difference_type n) const {return const_iterator(d_, pos_ + n);}

        const_iterator operator-(difference_type n) const {return const_iterator(d_, pos_ - n);}

        difference_type operator-(const const_iterator& other) const {return pos_ - other.pos_;}

        reference operator[](difference_type n) const {return (*d_)[static_cast<size_type>(pos_ + n)];}

        bool operator==(const const_iterator& other) const {return d_ == other.d_ && pos_ == other.pos_;}

        bool operator!=(const const_iterator& other) const {return !(*this == other);}

        bool operator<(const const_iterator& other) const {return pos_ < other.pos_;}

        bool operator>(const const_iterator& other) const {return other < *this;}

        bool operator<=(const const_iterator& other) const {return !(other < *this);}

        bool operator>=(const const_iterator& other) const {return !(*this < other);}
    };

        // Default constructor → empty deque
        Deque() : data_(nullptr), size_(0), capacity_(0), front_(0) {}

        // Destructor
        ~Deque() {
            clear();
            ::operator delete(data_);
        }

        // Copy constructor (deep copy)
        Deque(const Deque& other)
                : data_(nullptr),
                  size_(0),
                  capacity_(other.capacity_),
                  front_(0) {

            if (capacity_ != 0) {
                data_ = static_cast<T*>(
                        ::operator new(sizeof(T) * capacity_)
                );
            }

            for (size_t i = 0; i < other.size_; ++i) {
                new (data_ + i) T(other[i]);
                ++size_;
            }
        }

        // Copy assignment
        Deque& operator=(const Deque& other) {
            if (this == &other)
                return *this;

            Deque temp(other);
            swap(temp);

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
            if (this == &other)
                return *this;

            clear();
            ::operator delete(data_);

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
        size_type capacity() const { return capacity_; }
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

        const_reference at(size_type i) const {
            if (i >= size_)
                throw std::out_of_range("Deque::at");
            return (*this)[i];
        }

        // Access first element
        reference front() {
            assert(!empty());
            return data_[front_];
        }

        const_reference front() const {
            assert(!empty());
            return data_[front_];
        }

        // Access last element
        reference back() {
            assert(!empty());
            return data_[index(size_ - 1)];
        }

        const_reference back() const {
            assert(!empty());
            return data_[index(size_ - 1)];
        }

        iterator begin() { return iterator(this, 0); }
        iterator end() { return iterator(this, size_); }

        const_iterator begin() const { return const_iterator(this, 0); }
        const_iterator end() const { return const_iterator(this, size_); }

        const_iterator cbegin() const {return const_iterator(this, 0);}
        const_iterator cend() const {return const_iterator(this, size_);}

        // Insert at back
        void push_back(const T& value) {
            if (size_ == capacity_)
                grow();

            new (data_ + index(size_)) T(value);
            ++size_;
        }

        void push_back(T&& value) {
            if (size_ == capacity_)
                grow();

            new (data_ + index(size_)) T(std::move(value));
            ++size_;
        }

        // Insert at front
        void push_front(const T& value) {
            if (size_ == capacity_)
                grow();

            front_ = (front_ == 0 ? capacity_ - 1 : front_ - 1);

            new (data_ + front_) T(value);
            ++size_;
        }

        void push_front(T&& value) {
            if (size_ == capacity_)
                grow();

            front_ = (front_ == 0 ? capacity_ - 1 : front_ - 1);

            new (data_ + front_) T(std::move(value));
            ++size_;
        }

        // Remove last element
        void pop_back() {
            assert(!empty());

            data_[index(size_ - 1)].~T();
            --size_;
        }

        // Remove first element
        void pop_front() {
            assert(!empty());

            data_[front_].~T();
            front_ = (front_ + 1) % capacity_;
            --size_;
        }

        // Clear all elements (keeps capacity)
        void clear() {
            for (size_t i = 0; i < size_; ++i) {
                data_[index(i)].~T();
            }

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

        template<typename... Args>
        reference emplace_back(Args&&... args) {
            if (size_ == capacity_)
                grow();

            size_type pos = index(size_);

            new (data_ + pos) T(std::forward<Args>(args)...);
            ++size_;

            return data_[pos];
        }

        template<typename... Args>
        reference emplace_front(Args&&... args) {
            if (size_ == capacity_)
                grow();

            front_ = (front_ == 0 ? capacity_ - 1 : front_ - 1);

            new (data_ + front_) T(std::forward<Args>(args)...);
            ++size_;

            return data_[front_];
        }
    };

} // namespace ministl
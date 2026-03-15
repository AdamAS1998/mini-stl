/*
    MiniSTL - Vector Implementation
    --------------------------------
    A simplified implementation of std::vector for educational purposes.

    Features:
    - Dynamic contiguous storage
    - Rule of Five (copy/move semantics)
    - Random access iterators
    - STL algorithm compatibility
    - push_back / emplace_back
    - insert / erase
    - resize / reserve / shrink_to_fit

    Complexity guarantees follow std::vector where possible.

    Author: Adam Abu Saleh
*/

// ideas for later, prolly should add const with size with initial 0's and option to initialize with another num
#pragma once
#include <cstddef>  // size_t, ptrdiff_t
#include <cassert>  // assert()
#include <utility>  // std::move, std::forward, std::swap
#include <type_traits>  // std::remove_pointer_t for iterator value_type
#include <algorithm> // std::sort, std::max
namespace ministl {

/*
    Vector<T>

    A dynamic array container similar to std::vector.

    Properties:
    - contiguous memory layout
    - amortized O(1) push_back
    - constant time random access
*/
    template<typename T>
    class Vector {

    private:

        template<typename Ptr>
        class VectorIterator;   // forward declaration

        // pointer to the beginning of the allocated storage
        T* data_;

        // number of elements currently stored
        size_t size_;

        // total allocated storage capacity
        size_t capacity_;

        void reallocate(size_t new_capacity);

    public:

        using Iterator = VectorIterator<T*>;
        using ConstIterator = VectorIterator<const T*>;

        Vector();
        Vector(const Vector& other);
        Vector(Vector&& other) noexcept; //move constructor
        ~Vector();

        void push_back(const T& value);
        void push_back(T&& value);

        void pop_back();
        template<typename... Args>
        void emplace_back(Args&&... args);

        void insert(size_t index, const T& value);
        void insert(size_t index, T&& value);
        template<typename... Args>
        Iterator emplace(Iterator pos, Args&&... args);

        void erase(size_t index);

        void reserve(size_t new_capacity);

        size_t size() const;
        size_t capacity() const;

        bool empty() const;
        void clear();

        void resize(size_t new_size);
        void resize(size_t new_size, const T& value);

        void shrink_to_fit();

        void sort();
        template<typename Compare>
        void sort(Compare comp) {
            std::sort(begin(), end(), comp);
        }

        T& operator[](size_t index);
        const T& operator[](size_t index) const;

        T& at(size_t index);
        const T& at(size_t index) const;

        Vector& operator=(const Vector& other);
        Vector& operator=(Vector&& other) noexcept;

        // Returns a pointer to the first element in the vector's contiguous storage.
        T* data() noexcept { return data_; }

        // Const overload returning a pointer to the first element.
        const T* data() const noexcept { return data_; }

        void swap(Vector& other) noexcept {
            using std::swap;
            swap(data_, other.data_);
            swap(size_, other.size_);
            swap(capacity_, other.capacity_);
        }

        Iterator begin() {
            return Iterator(data_);
        }

        Iterator end() {
            return Iterator(data_ + size_);
        }

        ConstIterator begin() const {
            return ConstIterator(data_);
        }

        ConstIterator end() const {
            return ConstIterator(data_ + size_);
        }

        ConstIterator cbegin() const {
            return ConstIterator(data_);
        }

        ConstIterator cend() const {
            return ConstIterator(data_ + size_);
        }

        T& front();
        const T& front() const;

        T& back();
        const T& back() const;

/*
    Random-access iterator for Vector.

    Internally wraps a raw pointer since Vector stores elements
    in contiguous memory.

    Supports full random-access iterator operations so STL
    algorithms like std::sort work.
*/
    private:
        template<typename Ptr>
        class VectorIterator {
        private:
            Ptr ptr;

        public:

        /*
            Standard iterator trait definitions.
            These aliases allow std::iterator_traits to detect the iterator's
            value type, reference type, pointer type, distance type, and category,
            enabling full compatibility with STL algorithms.
        */
            using value_type = std::remove_pointer_t<Ptr>; // the type of element the iterator refers to (T)

            using difference_type = std::ptrdiff_t; // represent the distance between two iterators (can be negative)

            using pointer = Ptr; // Pointer type to the element (T*)

            using reference = value_type&; // Reference type returned when de-referencing the iterator (*it)

            // Declares this as a random-access iterator so STL algorithms
            // like std::sort can use optimized implementations
            using iterator_category = std::random_access_iterator_tag;

            explicit VectorIterator(Ptr p) : ptr(p) {}

            reference operator*() const { return *ptr; }
            pointer operator->() const { return ptr; }

            VectorIterator& operator++() {
                ++ptr;
                return *this;
            }

            VectorIterator operator++(int) {
                VectorIterator tmp = *this;
                ++ptr;
                return tmp;
            }

            VectorIterator& operator--() {
                --ptr;
                return *this;
            }

            VectorIterator operator--(int) {
                VectorIterator tmp = *this;
                --ptr;
                return tmp;
            }

            VectorIterator operator+(std::ptrdiff_t n) const {
                return VectorIterator(ptr + n);
            }

            VectorIterator operator-(std::ptrdiff_t n) const {
                return VectorIterator(ptr - n);
            }

            std::ptrdiff_t operator-(const VectorIterator& other) const {
                return ptr - other.ptr;
            }

            VectorIterator& operator+=(std::ptrdiff_t n) {
                ptr += n;
                return *this;
            }

            VectorIterator& operator-=(std::ptrdiff_t n) {
                ptr -= n;
                return *this;
            }

            reference operator[](std::ptrdiff_t n) const {
                return *(ptr + n);
            }

            bool operator==(const VectorIterator& other) const {
                return ptr == other.ptr;
            }

            bool operator!=(const VectorIterator& other) const {
                return ptr != other.ptr;
            }

            bool operator<(const VectorIterator& other) const {
                return ptr < other.ptr;
            }

            bool operator>(const VectorIterator& other) const {
                return ptr > other.ptr;
            }

            bool operator<=(const VectorIterator& other) const {
                return ptr <= other.ptr;
            }

            bool operator>=(const VectorIterator& other) const {
                return ptr >= other.ptr;
            }
        };

    };

    //simple constructor
    template<typename T>
    Vector<T>::Vector()
            : data_(nullptr), size_(0), capacity_(0)
    {
    }

    //copy constructor
    template<typename T>
    Vector<T>:: Vector(const Vector& other)
            : data_(new T[other.capacity_]),
              size_(other.size_),
              capacity_(other.capacity_)
    {
        for (size_t i = 0; i < size_; ++i)
            data_[i] = other.data_[i];
    }

    //move constructor
    // && is rvalue reference, a reference to a temporary object that is about to be destroyed.
    // basically its stealing the allocated memory instead of allocating and copying which is slow
    template<typename T>
    Vector<T>::Vector(Vector&& other) noexcept
            : data_(other.data_),
              size_(other.size_),
              capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    //simple destructor
    template<typename T>
    Vector<T>::~Vector()
    {
        delete[] data_;
    }

/*
    Reallocates storage to a new capacity.

    Allocates new memory and move-constructs the existing elements
    into the new storage.

    Complexity: O(n)
*/
    template<typename T>
    void Vector<T>::reallocate(size_t new_capacity)
    {
        T* new_data = new T[new_capacity];

        for (size_t i = 0; i < size_; ++i)
        {
            new_data[i] = std::move(data_[i]);
        }

        delete[] data_;

        data_ = new_data;
        capacity_ = new_capacity;
    }

/*
    Appends an element to the end of the vector.

    If capacity is exhausted, the storage grows (doubling strategy).

    Complexity:
    - Amortized O(1)
    - Worst case O(n) when reallocation occurs
*/
    template<typename T>
    void Vector<T>::push_back(const T& value)
    {
        if (size_ == capacity_)
        {
            // geometric growth to guarantee amortized O(1) push_back
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }

        data_[size_] = value;
        ++size_;
    }

    //move version
    template<typename T>
    void Vector<T>::push_back(T&& value)
    {
        if (size_ == capacity_){
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }

        data_[size_] = std::move(value);
        ++size_;
    }

/*
    Removes the last element.

    Complexity: O(1)
*/
    template<typename T>
    void Vector<T>::pop_back()
    {
        assert(size_ > 0);
        --size_;
        data_[size_].~T();
    }

    // Constructs a new element directly at the end of the vector by forwarding the
    // provided arguments to T's constructor, avoiding temporary objects.
    template<typename T>
    template<typename... Args>
    void Vector<T>::emplace_back(Args&&... args)
    {
        if (size_ == capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }

        data_[size_] = T(std::forward<Args>(args)...);
        ++size_;
    }

    //insert value at index
    template<typename T>
    void Vector<T>::insert(size_t index, const T& value)
    {
        assert(index <= size_);

        if (size_ == capacity_)
        {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }

        for (size_t i = size_; i > index; --i)
        {
            data_[i] = std::move(data_[i - 1]);
        }

        data_[index] = value;
        ++size_;
    }

    template<typename T>
    void Vector<T>::insert(size_t index, T&& value)
    {
        assert(index <= size_);

        if(size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);

        for(size_t i = size_; i > index; --i)
            data_[i] = std::move(data_[i-1]);

        data_[index] = std::move(value);
        ++size_;
    }

    //emplace version of insert
    template<typename T>
    template<typename... Args>
    typename Vector<T>::Iterator
    Vector<T>::emplace(Iterator pos, Args&&... args)
    {
        size_t index = pos - begin();

        if (size_ == capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }

        for (size_t i = size_; i > index; --i) {
            data_[i] = std::move(data_[i - 1]);
        }

        data_[index] = T(std::forward<Args>(args)...);

        ++size_;

        return Iterator(data_ + index);
    }

    //erase element at index
    template<typename T>
    void Vector<T>::erase(size_t index)
    {
        assert(index < size_);

        for (size_t i = index; i < size_ - 1; ++i)
        {
            data_[i] = std::move(data_[i + 1]);
        }

        --size_;
        data_[size_].~T();
    }


    //reallocating the vector with given capacity
    template<typename T>
    void Vector<T>::reserve(size_t new_capacity)
    {
        if (new_capacity > capacity_)
        {
            reallocate(new_capacity);
        }
    }


    //size getter
    template<typename T>
    size_t Vector<T>::size() const
    {
        return size_;
    }

    //capacity getter
    template<typename T>
    size_t Vector<T>::capacity() const
    {
        return capacity_;
    }

    //checks if vector is empty
    template<typename T>
    bool Vector<T>::empty() const
    {
        return size_ == 0;
    }

    //clearing the vector
    template<typename T>
    void Vector<T>::clear()
    {
        for (size_t i = 0; i < size_; ++i)
            data_[i].~T();

        size_ = 0;
    }

/*
    Resizes the container.

    If new_size > size:
        new elements are default constructed.

    If new_size < size:
        extra elements are destroyed.

    Complexity: O(n)
*/
    template<typename T>
    void Vector<T>::resize(size_t new_size)
    {
        if(new_size > capacity_) //prevents many reallocations
            reallocate(std::max(new_size, capacity_ * 2));

        if (new_size > size_) {
            for (size_t i = size_; i < new_size; ++i) {
                data_[i] = T();
            }
        }
        else {
            for (size_t i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
        }

        size_ = new_size;
    }

    //instead if default const we give it a value
    template<typename T>
    void Vector<T>::resize(size_t new_size, const T& value)
    {
        if (new_size > capacity_) {
            reallocate(new_size);
        }

        if (new_size > size_) {
            for (size_t i = size_; i < new_size; ++i) {
                data_[i] = value;
            }
        }
        else {
            for (size_t i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
        }

        size_ = new_size;
    }

    template<typename T>
    void Vector<T>::shrink_to_fit()
    {
        if (capacity_ == size_)
            return;

        T* new_data = new T[size_];

        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }

        delete[] data_;

        data_ = new_data;
        capacity_ = size_;
    }

/*
    Sorts the vector using std::sort.

    Complexity: O(n log n)
*/
    template<typename T>
    void Vector<T>::sort() {
        std::sort(begin(), end());
    }

    //access to element in index
    template<typename T>
    T& Vector<T>::operator[](size_t index)
    {
        assert(index < size_);
        return data_[index];
    }

    //const version
    template<typename T>
    const T& Vector<T>::operator[](size_t index) const
    {
        assert(index < size_);
        return data_[index];
    }

    //access element with no index check (throws exception )
    template<typename T>
    T& Vector<T>::at(size_t index)
    {
        return data_[index];
    }

    //const version
    template<typename T>
    const T& Vector<T>::at(size_t index) const
    {
        return data_[index];
    }

    //copy assignment operator
    template<typename T>
    Vector<T>& Vector<T>::operator=(const Vector& other)
    {
        if (this == &other)
            return *this;   // self-assignment protection

        delete[] data_;     // free current memory

        size_ = other.size_;
        capacity_ = other.capacity_;

        data_ = new T[capacity_];

        for (size_t i = 0; i < size_; ++i)
        {
            data_[i] = other.data_[i];
        }

        return *this;
    }

    //move version
    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
    {
        if (this == &other)
            return *this;

        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    template<typename T>
    T& Vector<T>::front()
    {
        assert(size_ > 0);
        return data_[0];
    }

    template<typename T>
    const T& Vector<T>::front() const
    {
        assert(size_ > 0);
        return data_[0];
    }

    template<typename T>
    T& Vector<T>::back()
    {
        assert(size_ > 0);
        return data_[size_ - 1];
    }

    template<typename T>
    const T& Vector<T>::back() const
    {
        assert(size_ > 0);
        return data_[size_ - 1];
    }

}// namespace ministl

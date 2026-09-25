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

#pragma once
#include <cstddef>  // size_t, ptrdiff_t
#include <cassert>  // assert()
#include <utility>  // std::move, std::forward, std::swap
#include <type_traits>  // std::remove_pointer_t for iterator value_type
#include <algorithm> // std::sort, std::max
#include <initializer_list> // std::initializer_list<T>
#include <new>        // placement new, ::operator new/delete
#include <stdexcept>  // std::out_of_range
#include <iterator>  // std::random_access_iterator_tag

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

        // forward declaration
        template<typename Ptr>
        class VectorIterator; //VectorIterator<Ptr>, Ptr is T* or const T*

        // pointer to the beginning of the allocated storage
        T* data_;

        // number of elements currently stored
        size_t size_;

        // total allocated storage capacity
        size_t capacity_;

        // internal helper that changes the amount of allocated storage
        void reallocate(size_t new_capacity);
        static T* allocate(size_t capacity);
        static void deallocate(T* ptr);

    public:
        //aliases - no object is created here
        using Iterator = VectorIterator<T*>; //the class is using type of pointer to T (T*)
        using ConstIterator = VectorIterator<const T*>; //same but const

        Vector(); // default constructor
        explicit Vector(size_t count);  // count default elements (explicit to prevent Vector<int> v = 5; to Vector<int> v(5); confusion)
        Vector(size_t count, const T& value); // count copies of value
        Vector(std::initializer_list<T>);     // {1, 2, 3}
        template<std::input_iterator InputIt>
        Vector(InputIt first, InputIt last);  // range initialization

        //Rule of 5
        Vector(const Vector& other); //copy-constructor
        Vector(Vector&& other) noexcept; //move constructor
        ~Vector(); //destructor
        Vector& operator=(const Vector& other); // copy assignment
        Vector& operator=(Vector&& other) noexcept; // move assignment

        // assignment
        void assign(size_t count, const T& value);

        template<std::input_iterator InputIt>
        void assign(InputIt first, InputIt last);

        void assign(std::initializer_list<T> init);

        Vector& operator=(std::initializer_list<T> init);

        void push_back(const T& value);  // copy version
        void push_back(T&& value);       // move version

        void pop_back();

        template<typename... Args>
        void emplace_back(Args&&... args);

        Iterator insert(Iterator pos, const T& value);
        Iterator insert(Iterator pos, T&& value);
        Iterator insert(Iterator pos, size_t count, const T& value);

        template<std::input_iterator InputIt>
        Iterator insert(Iterator pos, InputIt first, InputIt last);

        Iterator insert(Iterator pos, std::initializer_list<T> init);

        Iterator erase(Iterator pos);
        Iterator erase(Iterator first, Iterator last);

        template<typename... Args>
        Iterator emplace(Iterator pos, Args&&... args);

        void reserve(size_t new_capacity);

        size_t size() const;
        size_t max_size() const noexcept;
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

        friend bool operator==(const Vector& a, const Vector& b) {
            if (a.size_ != b.size_) return false;

            for (size_t i = 0; i < a.size_; i++) {
                if (a.data_[i] != b.data_[i])
                    return false;
            }
            return true;
        }

        friend bool operator<(const Vector& a, const Vector& b) {
            size_t n = std::min(a.size_, b.size_);

            for (size_t i = 0; i < n; i++) {
                if (a.data_[i] < b.data_[i]) return true;
                if (b.data_[i] < a.data_[i]) return false;
            }

            return a.size_ < b.size_;
        }

        friend bool operator!=(const Vector& a, const Vector& b) {
            return !(a == b);
        }

        friend bool operator<=(const Vector& a, const Vector& b) {
            return !(b < a);
        }

        friend bool operator>(const Vector& a, const Vector& b) {
            return b < a;
        }

        friend bool operator>=(const Vector& a, const Vector& b) {
            return !(a < b);
        }

/*
    Random-access iterator for Vector.

    Internally wraps a raw pointer since Vector stores elements in contiguous memory.

    Supports full random-access iterator operations so STL algorithms like std::sort work.
*/
    private:
        template<typename Ptr>
        class VectorIterator {
        private:
            Ptr ptr;

        public:

        /*
            Standard iterator trait definitions.
            These aliases allow std::iterator_traits to detect the iterator's value type,
            reference type, pointer type, distance type, and category,
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
//====================================================================================================================//

/*
    Reallocates storage to a new capacity.

    Allocates raw storage and move-constructs the existing
    elements into the new storage.

    The old elements are then destroyed and the old storage
    is released.

    Complexity: O(n)
*/
    template<typename T>
    void Vector<T>::reallocate(size_t new_capacity)
    {
        T* new_data = allocate(new_capacity);

        for (size_t i = 0; i < size_; ++i)
        {
            new (new_data + i) T(std::move(data_[i]));
        }

        for (size_t i = 0; i < size_; ++i)
        {
            data_[i].~T();
        }

        deallocate(data_);

        data_ = new_data;
        capacity_ = new_capacity;
    }

/*
    Allocates raw uninitialized storage for 'capacity' elements.

    No T objects are constructed by this operation.

    Complexity: O(1)
*/
    template<typename T>
    T* Vector<T>::allocate(size_t capacity)
    {
        if (capacity == 0)
            return nullptr;

        return static_cast<T*>(
                ::operator new(sizeof(T) * capacity)
        );
    }


/*
    Releases raw storage previously allocated by allocate().

    The objects stored in the memory must already be destroyed.

    Complexity: O(1)
*/
    template<typename T>
    void Vector<T>::deallocate(T* ptr)
    {
        ::operator delete(ptr);
    }

/*
    Creates an empty vector.

    No storage is allocated initially.
    Size and capacity are both initialized to 0.

    Complexity: O(1)
*/
    template<typename T>
    Vector<T>::Vector()
            : data_(nullptr), size_(0), capacity_(0)
    {
    }

/*
    Creates a vector with 'count' default-constructed elements.

    Raw storage is allocated for count elements, then each
    element is constructed directly in that storage.

    Complexity: O(count)
*/
    template<typename T>
    Vector<T>::Vector(size_t count)
            : data_(allocate(count)),
              size_(0),
              capacity_(count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            new (data_ + i) T();
            ++size_;
        } //size_t ,so you don't need to check if count is positive, one issue if we do Vector(-5) compiler
    }//will convert it to huge positive number due to unsigned conversion then throw std::bad_alloc

/*
    Creates a vector with 'count' copies of the given value.

    Raw storage is allocated and each element is copy-constructed
    directly in that storage.

    Complexity: O(count)
*/
    template<typename T>
    Vector<T>::Vector(size_t count, const T& value)
            : data_(allocate(count)),
              size_(0),
              capacity_(count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            new (data_ + i) T(value);
            ++size_;
        }
    }


/*
    Creates a vector from an initializer list.

    Each element is copy-constructed directly in the
    vector's raw storage.

    Complexity: O(n)
*/
    template<typename T>
    Vector<T>::Vector(std::initializer_list<T> init)
            : data_(allocate(init.size())),
              size_(0),
              capacity_(init.size())
    {
        for (const T& value : init)
        {
            new (data_ + size_) T(value);
            ++size_;
        }
    }

/*
    Creates a vector containing the elements in the range [first, last).

    Elements are appended in their original order.

    Complexity:
    - O(n) element insertions
    - May perform multiple reallocations
*/
    template<typename T>
    template<std::input_iterator InputIt>
    Vector<T>::Vector(InputIt first, InputIt last)
            : data_(nullptr),
              size_(0),
              capacity_(0)
    {
        for (; first != last; ++first)
            push_back(*first);
    }

/*
    Creates a copy of another vector.

    Allocates raw storage with the same capacity and
    copy-constructs each existing element.

    Complexity: O(n)
*/
    template<typename T>
    Vector<T>::Vector(const Vector& other)
            : data_(allocate(other.capacity_)),
              size_(0),
              capacity_(other.capacity_)
    {
        for (size_t i = 0; i < other.size_; ++i)
        {
            new (data_ + i) T(other.data_[i]);
            ++size_;
        }
    }


/*
    Creates a vector by moving another vector.

    Takes ownership of the other vector's allocated storage
    without copying its elements. The other vector is left empty.

    Complexity: O(1)
*/
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


/*
    Destroys all elements stored in the vector and
    releases the allocated raw storage.

    Complexity: O(n)
*/
    template<typename T>
    Vector<T>::~Vector()
    {
        for (size_t i = 0; i < size_; ++i)
        {
            data_[i].~T();
        }

        deallocate(data_);
    }

/*
    Copies the contents of another vector into this vector.

    A temporary copy is created and then swapped with this vector.
    This also safely handles self-assignment.

    Complexity: O(n)
*/
    template<typename T>
    Vector<T>& Vector<T>::operator=(const Vector& other)
    {
        if (this == &other)
            return *this;

        Vector temp(other);
        swap(temp);

        return *this;
    }


/*
    Moves the contents of another vector into this vector.

    Destroys the current elements, releases the current storage,
    then takes ownership of the other vector's storage.
    The other vector is left empty.

    Complexity: O(n) for destroying the current elements,
    O(1) otherwise.
*/
    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();
        deallocate(data_);

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

/*
    Replaces the vector contents with 'count' copies of value.

    Existing elements are destroyed. Storage is reallocated
    if the current capacity is insufficient.

    Complexity: O(n + count)
*/
    template<typename T>
    void Vector<T>::assign(size_t count, const T& value)
    {
        clear();

        if (count > capacity_)
        {
            deallocate(data_);
            data_ = allocate(count);
            capacity_ = count;
        }

        for (size_t i = 0; i < count; ++i)
        {
            new (data_ + i) T(value);
            ++size_;
        }
    }

/*
    Replaces the vector contents with the elements in [first, last).

    Elements are copied from the given range in their original order.

    Complexity:
    - O(n) element insertions
    - May perform reallocations
*/
    template<typename T>
    template<std::input_iterator InputIt>
    void Vector<T>::assign(InputIt first, InputIt last)
    {
        clear();

        for (; first != last; ++first)
            push_back(*first);
    }

/*
    Replaces the vector contents with the elements
    from an initializer list.

    Complexity: O(n)
*/
    template<typename T>
    void Vector<T>::assign(std::initializer_list<T> init)
    {
        clear();

        if (init.size() > capacity_)
            reallocate(init.size());

        for (const T& value : init)
            push_back(value);
    }

/*
    Replaces the vector contents with the elements
    from an initializer list.

    Complexity: O(n)
*/
    template<typename T>
    Vector<T>& Vector<T>::operator=(std::initializer_list<T> init)
    {
        assign(init);
        return *this;
    }

/*
    Appends a copy of value to the end of the vector.

    If capacity is exhausted, the storage grows using
    the doubling strategy.

    Complexity:
    - Amortized O(1)
    - Worst case O(n) when reallocation occurs
*/
    template<typename T>
    void Vector<T>::push_back(const T& value)
    {
        if (size_ == capacity_)
        {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }

        new (data_ + size_) T(value);

        ++size_;
    }

// move version of push_back
    template<typename T>
    void Vector<T>::push_back(T&& value)
    {
        if (size_ == capacity_)
        {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }

        new (data_ + size_) T(std::move(value));

        ++size_;
    }

/*
    Removes and destroys the last element.

    The vector must not be empty.
    An assertion fails if pop_back() is called on an empty vector.

    Complexity: O(1)
*/
    template<typename T>
    void Vector<T>::pop_back()
    {
        assert(size_ > 0);

        --size_;
        data_[size_].~T();
    }

/*
    Constructs a new element directly at the end of the vector.

    The provided arguments are perfectly forwarded directly
    to T's constructor, avoiding the creation of a temporary T.

    If capacity is exhausted, the storage grows first.

    Complexity:
    - Amortized O(1)
    - Worst case O(n) when reallocation occurs
*/
    template<typename T>
    template<typename... Args>
    void Vector<T>::emplace_back(Args&&... args)
    {
        if (size_ == capacity_)
        {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }

        new (data_ + size_) T(std::forward<Args>(args)...);

        ++size_;
    }

/*
    Inserts a copy of value before pos.

    The last element is move-constructed into unused storage,
    then the remaining elements are shifted right.

    Returns an iterator to the inserted element.

    Complexity: O(n)
*/
    template<typename T>
    typename Vector<T>::Iterator
    Vector<T>::insert(Iterator pos, const T& value)
    {
        size_t index = pos - begin();

        assert(index <= size_);

        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);

        if (index == size_)
        {
            new (data_ + size_) T(value);
        }
        else
        {
            new (data_ + size_) T(std::move(data_[size_ - 1]));

            for (size_t i = size_ - 1; i > index; --i)
                data_[i] = std::move(data_[i - 1]);

            data_[index] = value;
        }

        ++size_;

        return Iterator(data_ + index);
    }

/*
    Move version of insert.

    Inserts value before pos by moving it into the vector.

    Returns an iterator to the inserted element.

    Complexity: O(n)
*/
    template<typename T>
    typename Vector<T>::Iterator
    Vector<T>::insert(Iterator pos, T&& value)
    {
        size_t index = pos - begin();

        assert(index <= size_);

        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);

        if (index == size_)
        {
            new (data_ + size_) T(std::move(value));
        }
        else
        {
            new (data_ + size_) T(std::move(data_[size_ - 1]));

            for (size_t i = size_ - 1; i > index; --i)
                data_[i] = std::move(data_[i - 1]);

            data_[index] = std::move(value);
        }

        ++size_;

        return Iterator(data_ + index);
    }

/*
    Inserts 'count' copies of value before pos.

    Elements are inserted in their original position and
    existing elements are shifted to the right.

    Returns an iterator to the first inserted element.

    Complexity: O(n * count) in this simplified implementation.
*/
    template<typename T>
    typename Vector<T>::Iterator
    Vector<T>::insert(Iterator pos, size_t count, const T& value)
    {
        size_t index = pos - begin();

        assert(index <= size_);

        for (size_t i = 0; i < count; ++i)
        {
            insert(begin() + index + i, value);
        }

        return Iterator(data_ + index);
    }

/*
    Inserts all elements from [first, last) before pos.

    Elements are inserted in their original order.

    Returns an iterator to the first inserted element.

    Complexity: O(n * m) with this simplified implementation,
    where m is the number of inserted elements.
*/
    template<typename T>
    template<std::input_iterator InputIt>
    typename Vector<T>::Iterator
    Vector<T>::insert(Iterator pos, InputIt first, InputIt last)
    {
        size_t index = pos - begin();
        size_t first_inserted = index;

        assert(index <= size_);

        for (; first != last; ++first)
        {
            insert(begin() + index, *first);
            ++index;
        }

        return Iterator(data_ + first_inserted);
    }

/*
    Inserts all elements from an initializer list before pos.

    Returns an iterator to the first inserted element.

    Complexity: O(n * m) with this simplified implementation.
*/
    template<typename T>
    typename Vector<T>::Iterator
    Vector<T>::insert(Iterator pos, std::initializer_list<T> init)
    {
        return insert(pos, init.begin(), init.end());
    }

/*
    Constructs a new element directly before pos.

    The last element is move-constructed into unused storage,
    existing elements are shifted right, and the new element
    is constructed from the forwarded arguments.

    Returns an iterator to the inserted element.

    Complexity: O(n)
*/
    template<typename T>
    template<typename... Args>
    typename Vector<T>::Iterator
    Vector<T>::emplace(Iterator pos, Args&&... args)
    {
        size_t index = pos - begin();

        assert(index <= size_);

        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);

        if (index == size_)
        {
            new (data_ + size_) T(std::forward<Args>(args)...);
        }
        else
        {
            new (data_ + size_) T(std::move(data_[size_ - 1]));

            for (size_t i = size_ - 1; i > index; --i)
                data_[i] = std::move(data_[i - 1]);

            data_[index].~T();
            new (data_ + index) T(std::forward<Args>(args)...);
        }

        ++size_;

        return Iterator(data_ + index);
    }

/*
    Removes the element at pos.

    Elements after pos are shifted one position to the left.

    Returns an iterator to the element that followed
    the erased element.

    pos must refer to a valid element and cannot be end().

    Complexity: O(n)
*/
    template<typename T>
    typename Vector<T>::Iterator
    Vector<T>::erase(Iterator pos)
    {
        size_t index = pos - begin();

        assert(index < size_);

        for (size_t i = index; i < size_ - 1; ++i)
            data_[i] = std::move(data_[i + 1]);

        --size_;
        data_[size_].~T();

        return Iterator(data_ + index);
    }

/*
    Removes all elements in the range [first, last).

    Elements after the erased range are shifted to the left.

    Returns an iterator to the element that followed
    the erased range.

    If first == last, nothing is removed.

    Complexity: O(n)
*/
    template<typename T>
    typename Vector<T>::Iterator
    Vector<T>::erase(Iterator first, Iterator last)
    {
        size_t first_index = first - begin();
        size_t last_index = last - begin();

        assert(first_index <= last_index);
        assert(last_index <= size_);

        size_t count = last_index - first_index;

        if (count == 0)
            return Iterator(data_ + first_index);

        for (size_t i = first_index; i + count < size_; ++i)
            data_[i] = std::move(data_[i + count]);

        for (size_t i = size_ - count; i < size_; ++i)
            data_[i].~T();

        size_ -= count;

        return Iterator(data_ + first_index);
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

/*
    Returns the maximum number of elements the vector
    could theoretically store.

    Complexity: O(1)
*/
    template<typename T>
    size_t Vector<T>::max_size() const noexcept
    {
        return static_cast<size_t>(-1) / sizeof(T);
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
    Resizes the vector.

    If new_size is larger, new elements are default-constructed.
    If new_size is smaller, excess elements are destroyed.

    Storage grows if necessary.

    Complexity: O(n)
*/
    template<typename T>
    void Vector<T>::resize(size_t new_size)
    {
        if (new_size > capacity_)
        {
            size_t new_capacity = std::max(
                    new_size,
                    capacity_ == 0 ? size_t(1) : capacity_ * 2
            );

            reallocate(new_capacity);
        }

        if (new_size > size_)
        {
            while (size_ < new_size)
            {
                new (data_ + size_) T();
                ++size_;
            }
        }
        else
        {
            while (size_ > new_size)
            {
                --size_;
                data_[size_].~T();
            }
        }
    }

/*
    Resizes the vector.

    If the vector grows, new elements are copy-constructed
    from value. If it shrinks, excess elements are destroyed.

    Storage grows if necessary.

    Complexity: O(n)
*/
    template<typename T>
    void Vector<T>::resize(size_t new_size, const T& value)
    {
        if (new_size > capacity_)
        {
            size_t new_capacity = std::max(
                    new_size,
                    capacity_ == 0 ? size_t(1) : capacity_ * 2
            );

            reallocate(new_capacity);
        }

        if (new_size > size_)
        {
            while (size_ < new_size)
            {
                new (data_ + size_) T(value);
                ++size_;
            }
        }
        else
        {
            while (size_ > new_size)
            {
                --size_;
                data_[size_].~T();
            }
        }
    }

/*
    Reduces capacity to match the current size.

    If capacity already equals size, nothing is done.

    Complexity: O(n)
*/
    template<typename T>
    void Vector<T>::shrink_to_fit()
    {
        if (capacity_ == size_)
            return;

        reallocate(size_);
    }

/*
    Sorts the vector using std::sort.

    Complexity: O(n log n)
*/
    template<typename T>
    void Vector<T>::sort() {
        std::sort(begin(), end());
    }

    template<typename T>
    T& Vector<T>::operator[](size_t index)
    {
        return data_[index];
    }

    template<typename T>
    const T& Vector<T>::operator[](size_t index) const
    {
        return data_[index];
    }

    template<typename T>
    T& Vector<T>::at(size_t index)
    {
        if (index >= size_)
            throw std::out_of_range("Vector::at index out of range");

        return data_[index];
    }

    template<typename T>
    const T& Vector<T>::at(size_t index) const
    {
        if (index >= size_)
            throw std::out_of_range("Vector::at index out of range");

        return data_[index];
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

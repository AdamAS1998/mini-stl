#pragma once
#include <cstddef>  //this is important c++ lib, includes size_t which is unsigned int (much larger rank than int)

namespace ministl {

    template<typename T>
    class Vector {
    private:
        T* data_;
        size_t size_;
        size_t capacity_;

        void reallocate(size_t new_capacity);

    public:
        Vector();
        ~Vector();

        void push_back(const T& value);
        void pop_back();

        void reserve(size_t new_capacity);

        size_t size() const;
        size_t capacity() const;

        bool empty() const;

        T& operator[](size_t index);
        const T& operator[](size_t index) const;

        // Iterators
        T* begin();
        T* end();

        const T* begin() const;
        const T* end() const;
    };

    //simple constructor
    template<typename T>
    Vector<T>::Vector()
            : data_(nullptr), size_(0), capacity_(0)
    {
    }

    //simple destructor
    template<typename T>
    Vector<T>::~Vector()
    {
        delete[] data_;
    }

    //handling resizing
    template<typename T>
    void Vector<T>::reallocate(size_t new_capacity)
    {
        T* new_data = new T[new_capacity];

        for (size_t i = 0; i < size_; i++)
        {
            new_data[i] = data_[i];
        }

        delete[] data_;

        data_ = new_data;
        capacity_ = new_capacity;
    }

    //inserting value to vector
    template<typename T>
    void Vector<T>::push_back(const T& value)
    {
        if (size_ == capacity_)
        {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }

        data_[size_] = value;
        size_++;
    }

    //remove value from vector
    template<typename T>
    void Vector<T>::pop_back()
    {
        if (size_ > 0)
        {
            size_--;
        }
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


    //access to element in index
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
    T* Vector<T>::begin()
    {
        return data_;
    }

    template<typename T>
    T* Vector<T>::end()
    {
        return data_ + size_;
    }

    template<typename T>
    const T* Vector<T>::begin() const
    {
        return data_;
    }

    template<typename T>
    const T* Vector<T>::end() const
    {
        return data_ + size_;
    }



}

#pragma once

#include <iterator>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>

class IntVector {
public:
    IntVector(size_t size = 0, size_t capacity = 0) : length_(size), capacity_(capacity) {
        if (size != 0 && capacity == 0)
        {
            capacity_ = size;
        }
        if (capacity_)
        {
            data_ = new int[capacity_];
            for (int i = 0; i < capacity_; ++i)
            {
                data_[i] = 0;
            }
        }
    }
    
    explicit IntVector(std::initializer_list<int> list) : IntVector(list.size()) {
        int counter = 0;
        for (auto ptr : list)
        {
            data_[counter++] = ptr;
        }
    }

    IntVector(IntVector& other) : IntVector(other.length_, other.capacity_) {
        if (other.data_ != nullptr)
            std::copy(other.data_, other.data_ + capacity_, data_);
    }

    IntVector(IntVector&& other) {
        length_ = std::exchange(other.length_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
        data_ = std::exchange(other.data_, nullptr);
    }

    ~IntVector() {
        if (capacity_)
            delete [] data_;
    }

    IntVector& operator =(IntVector other) {
        swap(*this, other);
        return *this;
    }

    size_t Size() const {
        return length_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void PushBack(int elem) {
        binary_reserve(length_ + 1);
        ++length_;
        data_[length_ - 1] = elem;
    }

    void PopBack() {
        --length_;
        if (length_ == 0)
        {
            delete data_;
            capacity_ = 0;
            data_ = nullptr;
        }
        ShrinkToFit();
    }


    void Clear() {
        length_ = 0;
    }

    void Reserve(size_t size) {
        if (size > capacity_)
        {
            capacity_ = size;
            IntVector moved(length_, capacity_);
            std::copy(data_, data_ + length_, moved.data_);
            std::swap(data_, moved.data_);
        }
    }

    void Swap(IntVector& rhs) {
        swap(*this, rhs);
    }

    void ShrinkToFit() {
        if (4 * length_ <= capacity_)
        {
            while (4 * length_ <= capacity_)
            {
                capacity_ >>= 1;
            }
            IntVector moved(*this);
            std::swap(data_, moved.data_);
        }
    }

    int& operator[](size_t ind) {
        if (ind > length_)
            throw std::logic_error("IntVector's subscript out of range");
        return data_[ind];
    }

    int operator[](size_t ind) const {
        if (ind >= length_)
            throw std::logic_error("IntVector's subscript out of range");
        return data_[ind];
    }

    friend void swap(IntVector & lhs, IntVector & rhs) {
        std::swap(lhs.length_, rhs.length_);
        std::swap(lhs.capacity_, rhs.capacity_);
        std::swap(lhs.data_, rhs.data_);
    }


private:
    int* data_ = nullptr;
    size_t length_ = 0, capacity_ = 0;

    void binary_reserve(size_t size)
    {
        if (size > capacity_)
        {
            if (size && capacity_ == 0)
                ++capacity_;
            while (size > capacity_)
            {
                capacity_ <<= 1;
            }
            IntVector moved(length_, capacity_);
            std::copy(data_, data_ + length_, moved.data_);
            std::swap(data_, moved.data_);
        }
    }
};

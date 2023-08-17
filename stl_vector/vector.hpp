#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstddef>
#include <cstring>
#include <time.h>

namespace stud_work
{
    template <typename T>
    class vector {
    private:
        T *array_ = nullptr;
        size_t size_;
        size_t capacity_;

    public:
        vector();
        vector(size_t size);
        vector(stud_work::vector<T> &vector);

        inline size_t size();
        inline size_t capacity();
        inline void pop_back();
        inline void clear();
        inline bool empty();
        inline T back();
        inline T front();

        T operator [](size_t index);
        T operator =(const stud_work::vector<T> &right);
        T at(size_t index);
        
        void push_back(T new_element);
        void reserve(size_t new_capacity);
        void resize(size_t new_size);
        void shrink_to_fit();
        void sort();

        ~vector();
    };

}

template <typename T>
stud_work::vector<T>::vector() : size_(0), capacity_(0) {}

template <typename T>
stud_work::vector<T>::vector(size_t size) : size_(size), capacity_(size)
{
    array_ = new T[capacity_];
}

template <typename T>
stud_work::vector<T>::vector(stud_work::vector<T> &vector) : size_(vector.size_), capacity_(vector.capacity_)
{
    array_ = new T[vector.capacity()];
    std::memcpy((void*)this->array_, (void*)vector.array_, sizeof(T) * vector.capacity_);
}

template <typename T>
inline size_t stud_work::vector<T>::size() { return size_; }

template <typename T>
inline size_t stud_work::vector<T>::capacity() { return capacity_; }

template <typename T>
inline void stud_work::vector<T>::pop_back() {if (size_ != 0) --size_;}

template <typename T>
inline void stud_work::vector<T>::clear() {size_ = 0;}

template <typename T>
inline bool stud_work::vector<T>::empty() {return (size_ == 0) ? true : false;}

template <typename T>
inline T stud_work::vector<T>::back() {return array_[size_ - 1];}

template <typename T>
inline T stud_work::vector<T>::front() {return array_[0];}

template <typename T>
T stud_work::vector<T>::operator [](size_t index)
{
    if (index > size_ - 1) throw std::out_of_range("Out of range!");
    return array_[index];
}

template <typename T>
T stud_work::vector<T>::operator =(const stud_work::vector<T> &right)
{
    size_ = right.size_;
    capacity_ = right.capacity_;
    array_ = new T[capacity_];

    std::memcpy(array_, right.array_, sizeof(T) * size_);
}

template <typename T>
T stud_work::vector<T>::at(size_t index)
{
    if (index > size_ - 1) throw std::out_of_range("Out of range!");
    return array_[index];    
}


template <typename T>
void stud_work::vector<T>::push_back(T new_element)
{
    if (size_ == capacity_) {
        capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
        T *new_array = new T[capacity_];
        std::memcpy((void*)new_array, (void*)array_, sizeof(T) * size_);

        if (array_ != nullptr) delete [] array_;
        array_ = new_array;
    }
    array_[size_++] = new_element;
}


template <typename T>
void stud_work::vector<T>::reserve(size_t new_capacity)
{
    capacity_ = new_capacity;
    T *new_array = new T[capacity_];
    std::memcpy((void*)new_array, (void*)array_, sizeof(T) * size_);
    if (array_ != nullptr) delete [] array_;
    array_ = new_array;
}

template <typename T>
void stud_work::vector<T>::resize(size_t new_size)
{
    if (new_size > capacity_) {
        capacity_ = new_size;
        T *new_array = new T[capacity_];
        std::memcpy((void*)new_array, (void*)array_, sizeof(T) * size_);
        if (array_ != nullptr) delete [] array_;
        array_ = new_array;    
    }
    size_ = new_size;
}

template <typename T>
void stud_work::vector<T>::shrink_to_fit()
{
    capacity_ = size_;
    T *new_array = new T[capacity_];
    std::memcpy((void*)new_array, (void*)array_, sizeof(T) * size_);
    if (array_ != nullptr) delete [] array_;
    array_ = new_array;   
}

template <typename T>
void stud_work::vector<T>::sort()
{
    for (size_t i = 0; i < size_; i++) {
        for (size_t j = 0; j < size_ - i - 1; j++) {
            if (array_[j] > array_[j + 1]) std::swap(array_[j], array_[j + 1]);
        }
    } 
}

template <typename T>
stud_work::vector<T>::~vector()
{
    delete [] array_;
}

#endif
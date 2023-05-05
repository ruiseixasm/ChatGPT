#include <iostream>

template<typename T>
class Iterator {
public:
    virtual bool hasNext() = 0;
    virtual T next() = 0;
};

template<typename T, size_t N>
class ArrayIterator : public Iterator<T> {
public:
    ArrayIterator(T* data, size_t size) : data_(data), size_(size), index_(0) {}

    bool hasNext() {
        return index_ < size_;
    }

    T next() {
        return data_[index_++];
    }

private:
    T* data_;
    size_t size_;
    size_t index_;
};

template<typename T, size_t N>
class Array {
public:
    T& operator[](size_t index) {
        return data_[index];
    }

    Iterator<T>* iterator() {
        return new ArrayIterator<T, N>(data_, N);
    }

private:
    T data_[N];
};

//int main() {
//
//    Array<int, 5> arr{ 1, 2, 3, 4, 5 };
//
//    Iterator<int>* it = arr.iterator();
//    while (it->hasNext()) {
//        std::cout << "i: " << it->next() << std::endl;
//    }
//    delete it;
//}

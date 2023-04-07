#include <iostream>

template <typename T>
class List {
public:
    List() {
        capacity_ = 2;
        size_ = 0;
        items_ = new T[capacity_];
    }

    ~List() {
        delete[] items_;
    }

    int Count() const {
        return size_;
    }

    void Add(const T& item) {
        if (size_ == capacity_) {
            Resize(capacity_ * 2);
        }
        items_[size_] = item;
        size_++;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return items_[index];
    }

private:
    T* items_;
    int capacity_;
    int size_;

    void Resize(int newCapacity) {
        T* newArray = new T[newCapacity];
        for (int i = 0; i < size_; i++) {
            newArray[i] = items_[i];
        }
        delete[] items_;
        items_ = newArray;
        capacity_ = newCapacity;
    }
};

int main() {
    List<int> numbers;
    numbers.Add(1);
    numbers.Add(2);
    numbers.Add(3);
    std::cout << "Count: " << numbers.Count() << std::endl;
    std::cout << "numbers[0]: " << numbers[0] << std::endl;
    std::cout << "numbers[1]: " << numbers[1] << std::endl;
    std::cout << "numbers[2]: " << numbers[2] << std::endl;

    List<std::string> names;
    names.Add("Alice");
    names.Add("Bob");
    names.Add("Charlie");
    std::cout << "Count: " << names.Count() << std::endl;
    std::cout << "names[0]: " << names[0] << std::endl;
    std::cout << "names[1]: " << names[1] << std::endl;
    std::cout << "names[2]: " << names[2] << std::endl;

    return 0;
}

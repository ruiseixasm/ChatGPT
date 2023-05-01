#include <iostream>

class MyIterator {
private:
    int* data;
    int size;
    int index;
public:
    MyIterator(int* data, int size) : data(data), size(size), index(0) {}
    int operator*() const { return data[index]; }
    MyIterator& operator++() { ++index; return *this; }
    bool operator!=(const MyIterator& other) const { return index != other.index; }
};

class MyContainer {
private:
    int* data;
    int size;
public:
    MyContainer(int* data, int size) : data(data), size(size) {}
    MyIterator begin() const { return MyIterator(data, size); }
    MyIterator end() const { return MyIterator(data, size); }
};

//int main() {
//    int arr[] = { 1, 2, 3, 4, 5 };
//    MyContainer container(arr, 5);
//    for (int i : container) {
//        std::cout << i << std::endl;
//    }
//    return 0;
//}

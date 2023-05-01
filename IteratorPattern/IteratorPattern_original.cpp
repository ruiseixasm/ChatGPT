#include <iostream>

class Iterator {
public:
    virtual bool hasNext() = 0;
    virtual int next() = 0;
};


class IntArray {
private:
    int* arr;
    int size;
public:
    IntArray(int* arr, int size) : arr(arr), size(size) {}

    //Iterator* createIterator() {
    //    return new ArrayIterator(arr, size);
    //}

    //class ArrayIterator : public Iterator {
    //private:
    //    int* arr;
    //    int size;
    //    int index;
    //public:
    //    ArrayIterator(int* arr, int size) : arr(arr), size(size), index(0) {}

    //    bool hasNext() {
    //        return index < size;
    //    }

    //    int next() {
    //        return arr[index++];
    //    }
    //};

    // The error in the code is that the begin() and end() methods are defined inside the ArrayIterator class,
    // which is a subclass of Iterator.Therefore, the begin() and end() methods are not accessible through
    // the Iterator* pointer in the main() function.

    int* begin() {
        return arr;
    }

    int* end() {
        return arr + size;
    }
};

int main() {
    int arr[] = { 1, 2, 3, 4, 5 };
    int size = sizeof(arr) / sizeof(arr[0]);

    IntArray intArray(arr, size);
    //Iterator* it = intArray.createIterator();

    for (auto i : intArray) {
        std::cout << "i: " << i << std::endl;
    }

    //delete it;
}
 
 
 
//class IntArray {
//private:
//    int* arr;
//    int size;
//public:
//    IntArray(int* arr, int size) : arr(arr), size(size) {}
//
//    Iterator* createIterator() {
//        return new ArrayIterator(arr, size);
//    }
//
//    class ArrayIterator : public Iterator {
//    private:
//        int* arr;
//        int size;
//        int index;
//    public:
//        ArrayIterator(int* arr, int size) : arr(arr), size(size), index(0) {}
//
//        bool hasNext() {
//            return index < size;
//        }
//
//        int next() {
//            return arr[index++];
//        }
//
//        int* begin() {
//            return arr;
//        }
//
//        int* end() {
//            return arr + size;
//        }
//    };
//};
//
//int main() {
//    int arr[] = { 1, 2, 3, 4, 5 };
//    int size = sizeof(arr) / sizeof(arr[0]);
//
//    IntArray intArray(arr, size);
//    Iterator* it = intArray.createIterator();
//
//    for (int i : *it) {
//        std::cout << "i: " << i << std::endl;
//    }
//
//    delete it;
//}

//class IntArray {
//private:
//    int* arr;
//    int size;
//public:
//    IntArray(int* arr, int size) : arr(arr), size(size) {}
//
//    Iterator* createIterator() {
//        return new ArrayIterator(arr, size);
//    }
//
//    class ArrayIterator : public Iterator {
//    private:
//        int* arr;
//        int size;
//        int index;
//    public:
//        ArrayIterator(int* arr, int size) : arr(arr), size(size), index(0) {}
//
//        bool hasNext() {
//            return index < size;
//        }
//
//        int next() {
//            return arr[index++];
//        }
//
//        int* begin() {
//            return arr;
//        }
//
//        int* end() {
//            return arr + size;
//        }
//    };
//};

#include <vector>
#include <algorithm>
#include <iostream>

template<typename T>
class Observer {
public:
    virtual void update(const T& data) = 0;
};

template<typename T>
class Subject {
public:
    void attach(Observer<T>* observer) {
        observers_.push_back(observer);
    }

    void detach(Observer<T>* observer) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
    }

    void notify(const T& data) {
        for (auto observer : observers_) {
            observer->update(data);
        }
    }

private:
    std::vector<Observer<T>*> observers_;
};

class ConcreteObserver : public Observer<int> {
public:
    void update(const int& data) override {
        std::cout << "Observer state updated to: " << data << std::endl;
    }
};

int main() {
    Subject<int> subject;
    ConcreteObserver observer1;
    ConcreteObserver observer2;
    subject.attach(&observer1);
    subject.attach(&observer2);
    subject.notify(42);
    subject.detach(&observer2);
    subject.notify(84);
    return 0;
}

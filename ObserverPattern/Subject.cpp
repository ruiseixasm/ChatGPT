#include "Subject.h"
#include "Observer.h"

void Subject::attach(Observer* observer) {
    observers_.push_back(observer);
}

void Subject::detach(Observer* observer) {
    observers_.remove(observer);
}

void Subject::notify() {
    for (auto observer : observers_) {
        observer->update();
    }
}

#include "Subject.h"
#include "Observer.h"

void Subject::attach(Observer* observer) {
    observers_.push_back(observer);
}

void Subject::detach(Observer* observer) {
    observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end()); // #include <algorithm> for std::remove
}

void Subject::notify() {
    for (auto observer : observers_) {
        observer->update();
    }
}

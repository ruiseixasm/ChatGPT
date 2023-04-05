// Subject.h

#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector> // Already includes the library <algorithm>. The library <string> also includes it!
#include <algorithm> // Not mandatory once the <vector> library already includes it

class Observer;

class Subject {
private:
    std::vector<Observer*> observers_;
public:
    virtual ~Subject() = default;
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify();
};

#endif // SUBJECT_H

// Subject.h

#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>

class Observer;

class Subject {
public:
    virtual ~Subject() = default;
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify();

private:
    std::vector<Observer*> observers_;
};

#endif // SUBJECT_H

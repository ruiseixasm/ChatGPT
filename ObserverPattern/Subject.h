// Subject.h

#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"

class Subject {
public:
    virtual ~Subject() {}
    virtual void attach(Observer* observer);
    virtual void detach(Observer* observer);
    virtual void notify();
private:
    std::vector<Observer*> m_observers;
};

#endif // SUBJECT_H

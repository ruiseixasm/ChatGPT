// ConcreteSubject.h

#ifndef CONCRETE_SUBJECT_H
#define CONCRETE_SUBJECT_H

#include "Subject.h"

class ConcreteSubject : public Subject {
public:
    int getState() const;
    void setState(int state);
private:
    int state_;
};

#endif // CONCRETE_SUBJECT_H

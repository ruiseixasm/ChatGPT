// ConcreteSubject.h

#ifndef CONCRETESUBJECT_H
#define CONCRETESUBJECT_H

#include "Subject.h"

class ConcreteSubject : public Subject {
public:
    int getState() const;
    void setState(int state);
private:
    int m_state;
};

#endif // CONCRETESUBJECT_H

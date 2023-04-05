// ConcreteSubject.h

#ifndef CONCRETESUBJECT_H
#define CONCRETESUBJECT_H

#include "Subject.h"

class ConcreteSubject : public Subject {
private:
    int m_state;
public:
    int getState() const;
    void setState(int state);
};

#endif // CONCRETESUBJECT_H

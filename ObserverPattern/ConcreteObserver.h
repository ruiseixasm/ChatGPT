// ConcreteObserver.h

#ifndef CONCRETEOBSERVER_H
#define CONCRETEOBSERVER_H

#include <iostream>
#include "Observer.h"
#include "ConcreteSubject.h"

class ConcreteObserver : public Observer {
private:
    ConcreteSubject* m_subject;
    int m_totalSubjectUpdates = 0;
public:
    ConcreteObserver(ConcreteSubject* subject);
    void update() override;
};

#endif // CONCRETEOBSERVER_H

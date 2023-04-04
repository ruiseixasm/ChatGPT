// ConcreteObserver.h

#ifndef CONCRETEOBSERVER_H
#define CONCRETEOBSERVER_H

#include <iostream>
#include "Observer.h"
#include "ConcreteSubject.h"

class ConcreteObserver : public Observer {
public:
    ConcreteObserver(ConcreteSubject* subject);
    void update() override;
private:
    ConcreteSubject* m_subject;
    int m_observerState;
};

#endif // CONCRETEOBSERVER_H

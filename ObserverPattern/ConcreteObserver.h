// ConcreteObserver.h

#ifndef CONCRETE_OBSERVER_H
#define CONCRETE_OBSERVER_H

#include "Observer.h"
#include "ConcreteSubject.h"

class ConcreteObserver : public Observer {
public:
    ConcreteObserver(ConcreteSubject* subject);
    void update();
private:
    ConcreteSubject* subject_;
};

#endif // CONCRETE_OBSERVER_H

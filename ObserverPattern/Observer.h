// Observer.h

#ifndef OBSERVER_H
#define OBSERVER_H

#include "Subject.h"

class Observer {
public:
    Observer(Subject* subject);
    virtual ~Observer();
    virtual void update() = 0;
protected:
    Subject* subject_;
};

#endif // OBSERVER_H

// Observer.h

#ifndef OBSERVER_H
#define OBSERVER_H

class Subject;

class Observer {
protected:
    Subject* subject_ = nullptr;
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
    void attachToSubject(Subject* subject);
    void detachFromSubject();
};

#endif // OBSERVER_H

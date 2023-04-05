// Observer.h

#ifndef OBSERVER_H
#define OBSERVER_H

class Subject;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
    void attachToSubject(Subject* subject);
    void detachFromSubject();

protected:
    Subject* subject_ = nullptr;
};

#endif // OBSERVER_H

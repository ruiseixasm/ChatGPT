// ConcreteObserver.cpp

#include "ConcreteObserver.h"

ConcreteObserver::ConcreteObserver(ConcreteSubject* subject) : m_subject(subject) {
    m_subject->attach(this);
}

void ConcreteObserver::update() {
    m_observerState = m_subject->getState();
    std::cout << "Observer state updated to: " << m_observerState << std::endl;
}

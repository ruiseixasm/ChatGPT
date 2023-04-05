// ConcreteObserver.cpp

#include "ConcreteObserver.h"

ConcreteObserver::ConcreteObserver(ConcreteSubject* subject) : m_subject(subject) {
    m_subject->attach(this);
}

void ConcreteObserver::update() {
    m_totalSubjectUpdates++;
    std::cout << "Subject state updated to " << m_subject->getState();
    std::cout << " making it the update number " << m_totalSubjectUpdates << std::endl;
}

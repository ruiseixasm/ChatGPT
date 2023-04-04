// ConcreteSubject.cpp

#include "ConcreteSubject.h"

int ConcreteSubject::getState() const {
    return m_state;
}

void ConcreteSubject::setState(int state) {
    m_state = state;
    notify();
}

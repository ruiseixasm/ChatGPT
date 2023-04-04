// ConcreteSubject.cpp

#include "ConcreteSubject.h"

int ConcreteSubject::getState() const {
	return state_;
}

void ConcreteSubject::setState(int state) {
	state_ = state;
	notify();
}

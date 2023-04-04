// ConcreteObserver.cpp

#include "ConcreteObserver.h"

ConcreteObserver::ConcreteObserver(ConcreteSubject* subject) {
	subject_ = subject;
	subject_->attach(this);
}

void ConcreteObserver::update() {
	int state = subject_->getState();
	// do something with the updated state
}

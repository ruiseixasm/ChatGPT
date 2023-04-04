#include "Observer.h"
#include "Subject.h"
#include "ConcreteObserver.h"
#include "ConcreteSubject.h"

int main() {
	ConcreteSubject subject;
	ConcreteObserver observer(&subject);

	subject.setState(42); // notifies the observer that the state has changed

}
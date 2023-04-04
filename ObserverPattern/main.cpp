#include "ConcreteObserver.h"
#include "ConcreteSubject.h"

int main() {
    ConcreteSubject subject;
    ConcreteObserver observer(&subject);

    subject.setState(42);

    return 0;
}

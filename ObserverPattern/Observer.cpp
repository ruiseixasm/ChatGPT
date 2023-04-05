#include "Observer.h"
#include "Subject.h"

Observer::Observer(Subject* subject) : subject_(subject) {
    subject_->attach(this);
}

Observer::~Observer() {
    subject_->detach(this);
}

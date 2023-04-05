#include "Observer.h"
#include "Subject.h"

void Observer::attachToSubject(Subject* subject) {
    subject_ = subject;
    subject_->attach(this);
}

void Observer::detachFromSubject() {
    if (subject_) {
        subject_->detach(this);
        subject_ = nullptr;
    }
}
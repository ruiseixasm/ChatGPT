#pragma once

// Existing C++ class
class MyCppClass {
public:
    MyCppClass();
    ~MyCppClass();

    void doSomething();
};

// C++/CLI wrapper class
#using <mscorlib.dll>
using namespace System;

namespace MyWrapperNamespace {
    public ref class MyWrapperClass : public Object {
    private:
        MyCppClass* m_cppObject;

    public:
        MyWrapperClass() {
            m_cppObject = new MyCppClass();
        }

        ~MyWrapperClass() {
            delete m_cppObject;
        }

        void DoSomething() {
            m_cppObject->doSomething();
        }
    };
}

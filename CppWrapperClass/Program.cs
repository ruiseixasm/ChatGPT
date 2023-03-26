using System;
using MyWrapperNamespace; // Namespace for the C++/CLI wrapper class

namespace CppWrapperClass
{
    class Program
    {
        static void Main(string[] args)
        {
            MyWrapperClass wrapper = new MyWrapperClass();
            wrapper.DoSomething();
        }
    }
}

using System;

namespace EventHandlingExampleWithClasses
{
    class Program
    {
        static void Main(string[] args)
        {
            var myClass = new MyClass();        // 1. Crates event generator
            myClass.MyEvent += HandleMyEvent;   // 2. Adds Event Listner
            myClass.RaiseMyEvent();             // 3. Triggers an event
        }

        static void HandleMyEvent(object sender, EventArgs e)
        {
            Console.WriteLine("MyClass's event was raised.");
        }
    }

    class MyClass
    {
        public event EventHandler MyEvent;

        public void RaiseMyEvent()
        {
            MyEvent?.Invoke(this, EventArgs.Empty); // Uses "this", can't use "null"
        }
    }
}
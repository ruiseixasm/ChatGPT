using System;

namespace EventHandlingExample
{
    class Program
    {
        // Declare an event delegate
        public delegate void EventHandler(object sender, EventArgs e);

        // Declare an event using the delegate
        public static event EventHandler MyEvent;

        static void Main(string[] args)
        {
            // Add an event handler to the event
            MyEvent += new EventHandler(MyEventHandler);

            // Raise the event
            //MyEvent?.Invoke(this, EventArgs.Empty);
            MyEvent(null, EventArgs.Empty);

            Console.ReadKey();
        }

        // Define the event handler method
        public static void MyEventHandler(object sender, EventArgs e)
        {
            Console.WriteLine("Event handled!");
        }
    }
}

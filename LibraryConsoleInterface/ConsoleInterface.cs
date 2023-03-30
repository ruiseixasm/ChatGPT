using System;

namespace LibraryConsoleInterface
{
    public class ConsoleInterface
    {
        public string ReadCommand()
        {
            Console.Write($"Enter command: ");
            return Console.ReadLine();
        }
    }
}

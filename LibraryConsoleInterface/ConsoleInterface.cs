using System;

namespace LibraryConsoleInterface
{
    public class ConsoleInterface
    {
        private bool readBlocked = false;
        private int lastReadWrite;

        public void allowInterfaceRead()
        {
            readBlocked = false;
        }

        public string ReadCommand()
        {
            if (!WriteReserved())
            {
                Console.Write($"Enter command: ");
                string command = Console.ReadLine();
                lastReadWrite = Environment.TickCount;
                return command;
            }
            return null;
        }

        public void WriteInterface(object text)
        {
            if (WriteReserved())
            {
                Console.Write(text);
                lastReadWrite = Environment.TickCount;
            }
        }

        private bool WriteReserved()
        {
            return (readBlocked == true && (uint)(Environment.TickCount - lastReadWrite) < 5 * 1000);
        }
    }
}

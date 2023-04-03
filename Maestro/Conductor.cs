using System;
using LibraryCommandProcessor;

namespace Maestro_v1
{
    class Conductor
    {
        private static string localDevice = "maestro";
        static void Main(string[] args)
        {
            CommandProcessor commandProcessor = new CommandProcessor(localDevice);
            while (CommandProcessor.ReadEnteredCommands()) { };
        }
    }
}

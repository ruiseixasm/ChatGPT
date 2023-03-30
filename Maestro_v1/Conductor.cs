using System;
using LibraryCommandProcessor;

namespace Maestro_v1
{
    class Conductor
    {
        static void Main(string[] args)
        {
            CommandProcessor commandProcessor = new CommandProcessor();
            while (CommandProcessor.ReadEnteredCommands()) { };
        }
    }
}

using System;
using LibraryNetworkCommunication;
using LibraryCommandProcessor;
using LibraryConsoleInterface;

namespace Maestro_v1
{
    class Conductor
    {
        static void Main(string[] args)
        {
            // Send a total Recall
            NetworkCommunication localNetwork = new NetworkCommunication();
            localNetwork.RecallAddresses();
        }
    }
}

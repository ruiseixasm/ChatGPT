using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace NetworkUDP_BroadcastReceiver
{
    class Receiver
    {
        static void Main(string[] args)
        {
            const int listenPort = 11000;

            UdpClient listener = new UdpClient(listenPort);
            Console.WriteLine($"Listening for broadcast messages on port {listenPort}...");

            while (true)
            {
                try
                {
                    // Wait for a broadcast message
                    IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, listenPort);
                    byte[] bytes = listener.Receive(ref groupEP);

                    // Display the message
                    string message = Encoding.UTF8.GetString(bytes, 0, bytes.Length);
                    Console.WriteLine($"Received broadcast message from {groupEP}: {message}");
                }
                catch (Exception e)
                {
                    Console.WriteLine($"Error receiving broadcast message: {e.Message}");
                }
            }
        }
    }
}

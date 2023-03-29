using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace NetworkUDP_BroadcastSender
{
    class Sender
    {
        static void Main(string[] args)
        {
            // Set the broadcast address and port
            IPAddress broadcastAddress = IPAddress.Parse("192.168.31.255");
            int port = 3108;

            // Create the socket
            Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            // Enable broadcast option
            socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, 1);
            while (true)
            {
                // Construct the message as a byte array
                Console.Write("Enter message: ");
                string message = Console.ReadLine();

                if (!string.IsNullOrEmpty(message))
                {
                    byte[] buffer = Encoding.ASCII.GetBytes(message);
                    // Create the endpoint for broadcasting to the specified address and port
                    IPEndPoint endPoint = new IPEndPoint(broadcastAddress, port);

                    // Broadcast the message
                    socket.SendTo(buffer, endPoint);
                }
                else
                {
                    break;
                }
            }

            // Close the socket
            socket.Close();

            Console.WriteLine("Message broadcasted.");
        }
    }
}

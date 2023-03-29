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
            int port = 11000;

            // Create the socket
            Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            // Enable broadcast option
            socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, 1);

            // Construct the message as a byte array
            string message = "Hello, World!";
            byte[] buffer = Encoding.ASCII.GetBytes(message);

            // Create the endpoint for broadcasting to the specified address and port
            IPEndPoint endPoint = new IPEndPoint(broadcastAddress, port);

            // Broadcast the message
            socket.SendTo(buffer, endPoint);

            // Close the socket
            socket.Close();

            Console.WriteLine("Message broadcasted.");
        }
    }
}

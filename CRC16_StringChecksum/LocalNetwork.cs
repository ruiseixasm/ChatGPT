using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Generic;

namespace NetworkInterface
{
    public class LocalNetwork
    {
        private static int communicationPort = 3108;
        private static UdpClient listener = new UdpClient(communicationPort);;
        private static Socket socketBroadcast = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp); // Create the socket
        private static Dictionary<string, IPAddress> localNetworkAddresses = new Dictionary<string, IPAddress>();

        public LocalNetwork()
        {
            socketBroadcast.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, 1);
        }

        public bool SendText(string[] textString)
        {
            return false;
        }

        public string[] ReadText()
        {
            string[] result = new string[2];



            return result;
        }

        public int RefreshAddresses()
        {

            return 0;
        }

        private Tuple<string, IPAddress> ReceiveMessage()
        {
            try
            {
                // Wait for a broadcast message
                IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, communicationPort);
                byte[] bytes = listener.Receive(ref groupEP);
                string message = Encoding.UTF8.GetString(bytes, 0, bytes.Length);

                return Tuple.Create(message, groupEP.Address);

                if (message == "Hello")
                {
                    if (localNetworkAddresses.ContainsValue(groupEP.Address))
                    {

                    }
                else
                {

                }

                // Display the message
                Console.WriteLine($"Received broadcast message from {groupEP}: {message}");
            }
            catch (Exception e)
            {
                Console.WriteLine($"Error receiving broadcast message: {e.Message}");
                return null;
            }
        }

        private void SendMessage(Tuple<string, IPAddress> message)
        {
            byte[] buffer = Encoding.ASCII.GetBytes(message.Item1);
            // Create the endpoint for broadcasting to the specified address and port
            IPEndPoint endPoint = new IPEndPoint(message.Item2, communicationPort);
            socketBroadcast.SendTo(buffer, endPoint); // Broadcast the message
        }

        private static ushort TextCRC16(string textString)
        {
            char[] text = textString.ToCharArray();
            int length = textString.Length;
            const uint polynomial = 0x1021;
            uint crc = 0xFFFF;

            for (int i = 0; i < length; i++)
            {
                crc ^= (uint)text[i] << 8;
                for (byte j = 0; j < 8; j++)
                {
                    crc = (crc & 0x8000) != 0 ? crc << 1 ^ polynomial : crc << 1;
                }
            }

            return (ushort)crc;
        }
    }
}
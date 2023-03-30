using System;
using System.Net;
using System.Net.Sockets;
using System.Net.NetworkInformation;
using System.Text;
using System.Collections.Generic;

namespace NetworkCommunication
{
    public class LocalNetwork
    {
        private static int communicationPort = 3108;
        private static UdpClient listener = new UdpClient(communicationPort);
        private static Socket socketBroadcast = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp); // Create the socket
        private static Dictionary<string, IPAddress> localNetworkAddresses = new Dictionary<string, IPAddress>();

        public LocalNetwork()
        {
            socketBroadcast.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, 1);
        }

        public bool SendString(string[] sendString)
        {
            if (sendString.Length == 1)
            {
                if (sendString[0] == "recall")
                {
                    RecallAddresses();
                    return true;
                }
            }
            else
            {
                var message = new Tuple<string, IPAddress>(sendString[0], localNetworkAddresses[sendString[1]]);
                SendMessage(message);
                return true;
            }
            return false;
        }

        public string[] ReceiveString()
        {
            Tuple<string, IPAddress> message = ReceiveMessage();

            if (message != null)
            {

                string[] words = message.Item1.Split(' ');

                if (words.Length == 1) // just the name means new regist
                {
                    localNetworkAddresses[words[0]] = message.Item2;
                }

                string[] result = { message.Item1, words[0] };
                return result;
            }

            return null;
        }

        public void RecallAddresses()
        {
            List<IPAddress> broadcastAddresses = new List<IPAddress>();

            // Get the local IP address and subnet mask
            IPAddress[] localIPs = Dns.GetHostAddresses(Dns.GetHostName());
            IPAddress subnetMask = IPAddress.None;
            foreach (NetworkInterface adapter in NetworkInterface.GetAllNetworkInterfaces())
            {
                if (adapter.OperationalStatus == OperationalStatus.Up && adapter.Supports(NetworkInterfaceComponent.IPv4))
                {
                    foreach (UnicastIPAddressInformation local_ip in adapter.GetIPProperties().UnicastAddresses)
                    {
                        if (local_ip.Address.AddressFamily == AddressFamily.InterNetwork)
                        {
                            subnetMask = local_ip.IPv4Mask;
                            byte[] subnetMaskBytes = subnetMask.GetAddressBytes();
                            if (subnetMaskBytes[1] == 255 || true) // Avoids 127.0.0.1 addresses!
                            {
                                IPAddress broadcastAddress = BroadcastAddress(local_ip.Address, subnetMask);
                                if (!broadcastAddresses.Contains(broadcastAddress))
                                {
                                    var message = new Tuple<string, IPAddress>("Recall", broadcastAddress);
                                    SendMessage(message);
                                    broadcastAddresses.Add(broadcastAddress);
                                }
                            }
                        }
                    }
                }
            }
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
            socketBroadcast.SendTo(buffer, endPoint); // Broadcast or Unicast the message
        }

        private static IPAddress BroadcastAddress(IPAddress localAddress, IPAddress subnetMask)
        {
            byte[] localAddressBytes = localAddress.GetAddressBytes();
            byte[] subnetMaskBytes = subnetMask.GetAddressBytes();
            Array.Reverse(localAddressBytes);
            Array.Reverse(subnetMaskBytes);

            UInt32 localAddressInt = BitConverter.ToUInt32(localAddressBytes, 0);
            UInt32 subnetMaskInt = BitConverter.ToUInt32(subnetMaskBytes, 0);

            UInt32 first_address = localAddressInt & subnetMaskInt; // Network Address
            UInt32 last_address = localAddressInt | ~subnetMaskInt; // Broadcast Address

            byte[] broadcastAddressBytes = BitConverter.GetBytes(last_address);
            Array.Reverse(broadcastAddressBytes);

            return new IPAddress(broadcastAddressBytes);
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
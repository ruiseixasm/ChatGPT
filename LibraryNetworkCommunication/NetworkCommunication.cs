using System;
using System.Net;
using System.Net.Sockets;
using System.Net.NetworkInformation;
using System.Text;
using System.Collections.Generic;

namespace LibraryNetworkCommunication
{
    public class NetworkCommunication
    {
        private static int communicationPort = 3108;
        private static UdpClient listener = new UdpClient(communicationPort);
        private static Socket socketBroadcast = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp); // Create the socket
        private static Dictionary<string, IPAddress> localNetworkAddresses = new Dictionary<string, IPAddress>();

        public NetworkCommunication()
        {
            socketBroadcast.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, 1);
        }

        public static List<string> GetNetworkNames()
        {
            List<string> networkNames = new List<string>();
            foreach (KeyValuePair<string, IPAddress> networkAddress in localNetworkAddresses)
            {
                networkNames.Add(networkAddress.Key);
            }
            return networkNames;
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

            if (message != null && !string.IsNullOrEmpty(message.Item1))
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

            uint localAddressInt = BitConverter.ToUInt32(localAddressBytes, 0);
            uint subnetMaskInt = BitConverter.ToUInt32(subnetMaskBytes, 0);

            uint first_address = localAddressInt & subnetMaskInt; // Network Address
            uint last_address = localAddressInt | ~subnetMaskInt; // Broadcast Address

            byte[] broadcastAddressBytes = BitConverter.GetBytes(last_address);
            Array.Reverse(broadcastAddressBytes);

            return new IPAddress(broadcastAddressBytes);
        }
    }
}
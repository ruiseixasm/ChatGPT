using System;
using System.Net;
using System.Net.Sockets;
using System.Net.NetworkInformation;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections.Generic;

namespace LibraryNetworkCommunication
{
    public class NetworkCommunication
    {
        public class NetworkCommand
        {
            public string Command { get; set; }
            public string LocalDevice { get; set; }
            public string RemoteDevice { get; set; }
            public IPAddress RemoteAddress { get; set; }

            public Dictionary<string, IPAddress> ThisSomething = localNetworkAddresses;

            public NetworkCommand (string command, string localDevice, string remoteDevice)
            {
                Command = command;
                LocalDevice = localDevice;
                RemoteDevice = remoteDevice;
                if (remoteDevice != null && localNetworkAddresses.ContainsKey(RemoteDevice))
                {
                    RemoteAddress = localNetworkAddresses[RemoteDevice];
                }
                else
                {
                    RemoteAddress = null;
                }
            }
        }

        private static int communicationPort = 3108;
        private static UdpClient listener = new UdpClient(communicationPort);
        private static Socket socketBroadcast = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp); // Create the socket
        protected static Dictionary<string, IPAddress> localNetworkAddresses = new Dictionary<string, IPAddress>();

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

        public void SendCommand(string command, string localDevice, string remoteDevice)
        {
            NetworkCommand networkCommand = new NetworkCommand(command, localDevice, remoteDevice);
            if (remoteDevice == null)
            {
                BroadcastNetworkCommand(networkCommand);
            }
            else
            {
                SendNetworkCommand(networkCommand);
            }
        }

        public NetworkCommand ReceiveCommand()
        {
            return ReceiveNetworkCommand();
        }

        private void BroadcastNetworkCommand(NetworkCommand networkCommand)
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

                                networkCommand.RemoteAddress = BroadcastAddress(local_ip.Address, subnetMask);
                                if (!broadcastAddresses.Contains(networkCommand.RemoteAddress))
                                {
                                    SendNetworkCommand(networkCommand);
                                    broadcastAddresses.Add(networkCommand.RemoteAddress);
                                }
                            }
                        }
                    }
                }
            }
        }

        private NetworkCommand ReceiveNetworkCommand()
        {
            try
            {
                // Wait for a broadcast message
                IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, communicationPort);
                byte[] bytes = listener.Receive(ref groupEP);
                string message = Encoding.UTF8.GetString(bytes, 0, bytes.Length);
                message = Regex.Replace(message, @"\s+", " ");
                string command = Regex.Replace(message, "^[^ ]+ ", "");
                string remoteDevice = Regex.Replace(message, " .*", "");
                localNetworkAddresses[remoteDevice] = groupEP.Address;
                return new NetworkCommand(command, null, remoteDevice);
            }
            catch (Exception e)
            {
                Console.WriteLine($"Error receiving broadcast message: {e.Message}");
                return null;
            }
        }

        private void SendNetworkCommand(NetworkCommand networkCommand)
        {
            string command = $"{networkCommand.LocalDevice} {networkCommand.Command}";
            byte[] buffer = Encoding.ASCII.GetBytes(command);
            // Create the endpoint for broadcasting to the specified address and port
            IPEndPoint endPoint = new IPEndPoint(networkCommand.RemoteAddress, communicationPort);
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

        public bool DropRemoteDevice (string remoteDevice)
        {
            if (localNetworkAddresses.ContainsKey(remoteDevice))
            {
                localNetworkAddresses.Remove(remoteDevice);
                return true;
            }
            return false;
        }
    }
}
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Threading;

namespace ScanningPort80
{
    class Program
    {
        static int port = 80;
        static List<UInt32> scannedNetworkAddresses = new List<UInt32>();

        static void Main(string[] args)
        {
            Console.WriteLine("Starting Scanner!\n");


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
                            if (subnetMaskBytes[1] == 255)
                            {
                                ScanLocalIPs(local_ip.Address.GetAddressBytes(), subnetMaskBytes);
                            }
                        }
                    }
                }
            }
        }

        public static void ScanLocalIPs(byte[] localAddressBytes, byte[] subnetMaskBytes)
        {
            Array.Reverse(localAddressBytes);
            Array.Reverse(subnetMaskBytes);
            UInt32 localAddress = BitConverter.ToUInt32(localAddressBytes, 0);
            UInt32 subnetMask = BitConverter.ToUInt32(subnetMaskBytes, 0);

            UInt32 first_address = localAddress & subnetMask;
            UInt32 last_address = localAddress | ~subnetMask;

            if (!scannedNetworkAddresses.Contains(first_address))
            {
                scannedNetworkAddresses.Add(first_address);

                for (UInt32 scan_ip = first_address + 1; scan_ip < last_address; scan_ip++)
                {
                    byte[] scannedAddressBytes = BitConverter.GetBytes(scan_ip);
                    Array.Reverse(scannedAddressBytes);
                    IPAddress destinationAddress = new IPAddress(scannedAddressBytes);

                    string ip = destinationAddress.ToString();
                    Console.Write($"{ip}:{port}");

                    using (Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp))
                    {
                        // set the socket options
                        socket.ReceiveTimeout = 50;
                        socket.SendTimeout = 50;

                        try
                        {
                            socket.Connect(new IPEndPoint(destinationAddress, port));
                            //tcpClient.Connect(ip, port);
                            Console.Write(" AVAILABLE\n");
                        }
                        catch (SocketException)
                        {
                            Console.Write(" Unavailable\n");
                        }
                    }
                }
            }
        }
    }



    //class Program
    //{
    //    static void Main(string[] args)
    //    {
    //        // Get the local IP address and subnet mask
    //        IPAddress[] localIPs = Dns.GetHostAddresses(Dns.GetHostName());
    //        IPAddress subnetMask = IPAddress.None;
    //        foreach (NetworkInterface adapter in NetworkInterface.GetAllNetworkInterfaces())
    //        {
    //            if (adapter.OperationalStatus == OperationalStatus.Up && adapter.Supports(NetworkInterfaceComponent.IPv4))
    //            {
    //                foreach (UnicastIPAddressInformation ip in adapter.GetIPProperties().UnicastAddresses)
    //                {
    //                    if (ip.Address.AddressFamily == AddressFamily.InterNetwork)
    //                    {
    //                        foreach (var address in localIPs)
    //                        {
    //                            if (address.Equals(ip.Address))
    //                            {
    //                                subnetMask = ip.IPv4Mask;
    //                                break;
    //                            }
    //                        }
    //                        if (!subnetMask.Equals(IPAddress.None))
    //                        {
    //                            break;
    //                        }
    //                    }
    //                }
    //                if (!subnetMask.Equals(IPAddress.None))
    //                {
    //                    break;
    //                }
    //            }
    //        }

    //        // Calculate the number of possible IP addresses
    //        byte[] subnetMaskBytes = subnetMask.GetAddressBytes();
    //        int numZeros = 0;
    //        for (int i = 0; i < subnetMaskBytes.Length; i++)
    //        {
    //            byte maskByte = subnetMaskBytes[i];
    //            for (int j = 7; j >= 0; j--)
    //            {
    //                if ((maskByte & (1 << j)) == 0)
    //                {
    //                    numZeros++;
    //                }
    //                else
    //                {
    //                    break;
    //                }
    //            }
    //        }
    //        int numAddresses = (int)Math.Pow(2, numZeros) - 2;

    //        // Scan all possible IP addresses
    //        List<string> availableIPs = new List<string>();
    //        for (int i = 1; i <= numAddresses; i++)
    //        {
    //            string ip = GetIPFromIndex(i, subnetMask);
    //            Console.Write($"Scanning {ip}... ");
    //            using (TcpClient tcpClient = new TcpClient())
    //            {
    //                try
    //                {
    //                    tcpClient.Connect(ip, 80);
    //                    Console.WriteLine("Available");
    //                    availableIPs.Add(ip);
    //                }
    //                catch (SocketException)
    //                {
    //                    Console.WriteLine("Unavailable");
    //                }
    //            }
    //        }

    //        Console.WriteLine();
    //        Console.WriteLine("Available IP addresses:");
    //        foreach (string ip in availableIPs)
    //        {
    //            Console.WriteLine(ip);
    //        }
    //    }

    //    static string GetIPFromIndex(int index, IPAddress subnetMask)
    //    {
    //        byte[] subnetMaskBytes = subnetMask.GetAddressBytes();
    //        byte[] indexBytes = BitConverter.GetBytes(index);
    //        byte[] ipAddressBytes = new byte[4];
    //        // NEED TO CONSIDER LOACL IP ADDRESS!!!
    //        for (int i = 0; i < 4; i++)
    //        {
    //            ipAddressBytes[i] = (byte)(indexBytes[i] & subnetMaskBytes[i]);
    //        }
    //        return new IPAddress(ipAddressBytes).ToString();
    //    }
    //}



    //class Program
    //{
    //    static void Main(string[] args)
    //    {
    //        // Get local IP address
    //        IPAddress localIP = GetLocalIPAddress();
    //        Console.WriteLine($"Scanning for port 80 on {localIP} and local network...");

    //        // Generate IP addresses to scan
    //        for (int i = 1; i <= 254; i++)
    //        {
    //            string ipAddress = $"{localIP.AddressFamily.ToString()}.{localIP.GetAddressBytes()[0]}.{localIP.GetAddressBytes()[1]}.{i}";
    //            ScanIPAddress(ipAddress, 80);
    //        }

    //        Console.WriteLine("Scan complete.");
    //        Console.ReadKey();
    //    }

    //    static IPAddress GetLocalIPAddress()
    //    {
    //        // Get host name
    //        string hostName = Dns.GetHostName();

    //        // Get IP addresses for host name
    //        IPAddress[] addresses = Dns.GetHostAddresses(hostName);

    //        // Find IPv4 address
    //        foreach (IPAddress address in addresses)
    //        {
    //            if (address.AddressFamily == AddressFamily.InterNetwork)
    //            {
    //                return address;
    //            }
    //        }

    //        // Return null if no IPv4 address is found
    //        return null;
    //    }

    //    static void ScanIPAddress(string ipAddress, int port)
    //    {
    //        try
    //        {
    //            // Create endpoint for IP address and port
    //            IPEndPoint endpoint = new IPEndPoint(IPAddress.Parse(ipAddress), port);

    //            // Create socket and connect to endpoint
    //            Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
    //            socket.Connect(endpoint);

    //            // Connection successful
    //            Console.WriteLine($"Port {port} open on {ipAddress}");

    //            // Close socket
    //            socket.Close();
    //        }
    //        catch (Exception)
    //        {
    //            // Connection failed
    //        }
    //    }
    //}




    //class Program
    //{
    //    static void Main(string[] args)
    //    {
    //        // Get the IP address of the local machine
    //        IPAddress[] localIPs = Dns.GetHostAddresses(Dns.GetHostName());
    //        Console.WriteLine("Local IP addresses:");
    //        foreach (IPAddress addr in localIPs)
    //        {
    //            if (addr.AddressFamily == AddressFamily.InterNetwork)
    //            {
    //                Console.WriteLine(addr.ToString());
    //            }
    //        }

    //        // Prompt the user for the target port number
    //        Console.Write("Enter port number to scan: ");
    //        int port = int.Parse(Console.ReadLine());

    //        // Loop through all possible IP addresses in the local network
    //        for (int i = 1; i < 255; i++)
    //        {
    //            string ip = string.Format("{0}.{1}.{2}.", localIPs[0].ToString().Split('.')[0], localIPs[0].ToString().Split('.')[1], localIPs[0].ToString().Split('.')[2]);
    //            string addr = ip + i.ToString();

    //            try
    //            {
    //                // Try to connect to the target port
    //                TcpClient client = new TcpClient();
    //                client.Connect(addr, port);
    //                Console.WriteLine("{0} is listening on port {1}", addr, port);
    //                client.Close();
    //            }
    //            catch (Exception)
    //            {
    //                // Failed to connect to the target port
    //                Console.WriteLine("{0} is not listening on port {1}", addr, port);
    //            }
    //        }

    //        Console.ReadLine();
    //    }
    //}




    //class Program
    //{
    //    static void Main(string[] args)
    //    {
    //        string hostName = Dns.GetHostName();
    //        IPHostEntry hostEntry = Dns.GetHostEntry(hostName);
    //        IPAddress[] addresses = hostEntry.AddressList;

    //        foreach (IPAddress address in addresses)
    //        {
    //            if (address.AddressFamily == AddressFamily.InterNetwork)
    //            {
    //                Console.WriteLine($"Scanning for open port on {address}");

    //                // choose a port to scan
    //                int port = 80;

    //                // create the socket
    //                using (Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp))
    //                {
    //                    // set the socket options
    //                    socket.ReceiveTimeout = 500;
    //                    socket.SendTimeout = 500;

    //                    // attempt to connect to the host
    //                    try
    //                    {
    //                        socket.Connect(new IPEndPoint(address, port));
    //                        Console.WriteLine($"Port {port} is open on {address}");
    //                    }
    //                    catch (SocketException)
    //                    {
    //                        Console.WriteLine($"Port {port} is closed on {address}");
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}
}

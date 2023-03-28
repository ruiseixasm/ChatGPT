using System;
using System.Collections.Generic;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;

namespace IPsScanner
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Starting Scanner!\n");

            int port = 80;

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
                                ScanLocalIPs(local_ip.Address.GetAddressBytes(), subnetMaskBytes, port);
                            }
                        }
                    }
                }
            }
        }

        public static void ScanLocalIPs(byte[] localAddressBytes, byte[] subnetMaskBytes, int port)
        {
            //UInt32 localAddress =
            //    (UInt32)localAddressBytes[0] << 3 * 8 | (UInt32)localAddressBytes[1] << 2 * 8 |
            //    (UInt32)localAddressBytes[2] << 1 * 8 | (UInt32)localAddressBytes[3] << 0 * 8;
            //UInt32 subnetMask =
            //    (UInt32)subnetMaskBytes[0] << 3 * 8 | (UInt32)subnetMaskBytes[1] << 2 * 8 |
            //    (UInt32)subnetMaskBytes[2] << 1 * 8 | (UInt32)subnetMaskBytes[3] << 0 * 8;
            
            Array.Reverse(localAddressBytes);
            Array.Reverse(subnetMaskBytes);
            UInt32 localAddress = BitConverter.ToUInt32(localAddressBytes, 0);
            UInt32 subnetMask = BitConverter.ToUInt32(subnetMaskBytes, 0);

            UInt32 first_address = localAddress & subnetMask;
            UInt32 last_address = localAddress | ~subnetMask;

            for (UInt32 scan_ip = first_address + 1; scan_ip < last_address; scan_ip++)
            {
                byte[] scannedAddressBytes = BitConverter.GetBytes(scan_ip);
                Array.Reverse(scannedAddressBytes);
                Console.WriteLine($"{scannedAddressBytes[0]}.{scannedAddressBytes[1]}.{scannedAddressBytes[2]}.{scannedAddressBytes[3]}:{port}");
            }

        }
    }
}

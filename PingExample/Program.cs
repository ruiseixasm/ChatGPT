using System;
using System.Text;
using System.Net.NetworkInformation;

namespace PingExample
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("Enter an IP address to ping: ");
            string ipAddress = Console.ReadLine();

            Ping pingSender = new Ping();
            PingOptions options = new PingOptions();
            options.DontFragment = true;
            string data = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
            byte[] buffer = Encoding.ASCII.GetBytes(data);
            int timeout = 120;
            PingReply reply = pingSender.Send(ipAddress, timeout, buffer, options);

            if (reply.Status == IPStatus.Success)
            {
                Console.WriteLine("Ping successful. Response time: {0} ms", reply.RoundtripTime);
            }
            else
            {
                Console.WriteLine("Ping failed. Error message: {0}", reply.Status.ToString());
            }

            Console.ReadKey();
        }
    }
}

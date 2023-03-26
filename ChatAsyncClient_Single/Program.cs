using System;
using System.Net.Sockets;
using System.Text;

namespace ChatAsyncClient_Single
{
    public class Client
    {
        public void Connect(string server, string message)
        {
            try
            {
                TcpClient client = new TcpClient(server, 1234);

                byte[] data = Encoding.ASCII.GetBytes(message);
                NetworkStream stream = client.GetStream();

                stream.Write(data, 0, data.Length);

                byte[] response = new byte[4096];
                int bytesRead = stream.Read(response, 0, 4096);
                Console.WriteLine(Encoding.ASCII.GetString(response, 0, bytesRead));

                client.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("Enter server IP address: ");
            string server = Console.ReadLine();

            while (true)
            {
                Console.Write("Enter message to send: ");
                string message = Console.ReadLine();

                if (!string.IsNullOrEmpty(message))
                {
                    Client client = new Client();
                    client.Connect(server, message);
                }
                else
                {
                    break;
                }
            }
        }
    }
}

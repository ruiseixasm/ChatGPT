using System;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace ChatAsyncClient_Multiple
{
    class Client
    {
        static TcpClient clientSocket = new TcpClient();
        static NetworkStream serverStream = default(NetworkStream);

        static void Main(string[] args)
        {
            Console.WriteLine("Enter your name:");
            string clientName = Console.ReadLine();

            clientSocket.Connect("127.0.0.1", 8888);
            serverStream = clientSocket.GetStream();

            byte[] outStream = Encoding.ASCII.GetBytes(clientName + "$");
            serverStream.Write(outStream, 0, outStream.Length);
            serverStream.Flush();

            Thread clientThread = new Thread(GetServerResponse);
            clientThread.Start();

            while (true)
            {
                Console.Write("Enter message: ");
                string message = Console.ReadLine();

                if (!string.IsNullOrEmpty(message))
                {
                    outStream = Encoding.ASCII.GetBytes(message + "$");
                    serverStream.Write(outStream, 0, outStream.Length);
                    serverStream.Flush();
                }
                else
                {
                    break;
                }
            }
            CloseConnection();
        }

        static void GetServerResponse()
        {
            while (true)
            {
                try
                {
                    byte[] inStream = new byte[clientSocket.ReceiveBufferSize];
                    serverStream.Read(inStream, 0, clientSocket.ReceiveBufferSize);
                    string response = Encoding.ASCII.GetString(inStream);
                    response = response.Substring(0, response.IndexOf("$"));

                    Console.WriteLine(response);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }
        }

        static void CloseConnection()
        {
            byte[] data = Encoding.Unicode.GetBytes("exit");
            serverStream.Write(data, 0, data.Length);
            serverStream.Close();
            serverStream.Close();
            Environment.Exit(0);
        }
    }
}

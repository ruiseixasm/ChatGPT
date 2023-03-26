using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace ChatAsyncServer_Multiple
{
    class Server
    {
        static List<TcpClient> clientsList = new List<TcpClient>();
        static void Main(string[] args)
        {
            TcpListener serverSocket = new TcpListener(IPAddress.Any, 8888);
            serverSocket.Start();
            Console.WriteLine("Server started");
            while (true)
            {
                TcpClient clientSocket = serverSocket.AcceptTcpClient();
                clientsList.Add(clientSocket);
                Console.WriteLine("Client connected");

                Thread clientThread = new Thread(() => HandleClient(clientSocket));
                clientThread.Start();
            }
        }

        static void HandleClient(TcpClient client)
        {
            while (true)
            {
                try
                {
                    NetworkStream networkStream = client.GetStream();
                    byte[] bytesFrom = new byte[client.ReceiveBufferSize];
                    networkStream.Read(bytesFrom, 0, client.ReceiveBufferSize);
                    string dataFromClient = Encoding.ASCII.GetString(bytesFrom);
                    dataFromClient = dataFromClient.Substring(0, dataFromClient.IndexOf("$"));
                    Console.WriteLine("Client message: " + dataFromClient);

                    Broadcast(dataFromClient, client);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }
        }

        static void Broadcast(string message, TcpClient excludeClient)
        {
            foreach (TcpClient client in clientsList)
            {
                if (client != excludeClient)
                {
                    try
                    {
                        NetworkStream networkStream = client.GetStream();
                        byte[] broadcastBytes = Encoding.ASCII.GetBytes(message + "$");
                        networkStream.Write(broadcastBytes, 0, broadcastBytes.Length);
                        networkStream.Flush();
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.ToString());
                    }
                }
            }
        }
    }
}

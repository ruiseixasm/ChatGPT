using System;
using System.IO.Ports;

namespace SerialCommunicationExample
{
    class Program
    {
        static void Main(string[] args)
        {
            // Set up the serial port with the appropriate settings
            SerialPort serialPort = new SerialPort("COM3", 9600, Parity.None, 8, StopBits.One);

            // Open the serial port
            serialPort.Open();

            // Write a string to the serial port
            serialPort.Write("Hello, world!");

            // Read a string from the serial port
            string response = serialPort.ReadLine();

            // Close the serial port
            serialPort.Close();
        }
    }
}

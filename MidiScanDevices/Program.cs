using System;
using NAudio.Midi;

namespace MidiScanInputDevices
{
    class Program
    {
        static void Main(string[] args)
        {
            // Get the number of MIDI input devices
            int inputCount = MidiIn.NumberOfDevices;

            if (inputCount > 0)
            {
                // Loop through each input device and print its name
                for (int i = 0; i < inputCount; i++)
                {
                    string inputName = MidiIn.DeviceInfo(i).ProductName;
                    Console.WriteLine($"Input Device {i}: {inputName}");
                }
            } 
            else
            {
                Console.WriteLine("No Input Devices were found!");
            }

            Console.ReadKey();
        }
    }
}

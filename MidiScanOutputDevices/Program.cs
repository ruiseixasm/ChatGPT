using System;
using NAudio.Midi;

namespace MidiScanOutputDevices
{
    class Program
    {
        static void Main(string[] args)
        {
            // Get the number of MIDI output devices
            int outputCount = MidiOut.NumberOfDevices;

            if (outputCount > 0)
            {
                // Loop through each input device and print its name
                for (int i = 0; i < outputCount; i++)
                {
                    string outputName = MidiOut.DeviceInfo(i).ProductName;
                    Console.WriteLine($"Output Device {i}: {outputName}");
                }
            }
            else
            {
                Console.WriteLine("No Output Devices were found!");
            }
            Console.ReadKey();
        }
    }
}

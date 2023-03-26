using System;
using NAudio.Midi;
using System.Collections.Generic;

namespace MidiScanOutputDevices2
{
    class Program
    {
        static void Main(string[] args)
        {
            // Create a list to store the output devices
            List<MidiOutPort> outputDevices = new List<MidiOutPort>();

            // Scan for all connected output devices
            for (int deviceId = 0; deviceId < MidiOut.NumberOfDevices; deviceId++)
            {
                try
                {
                    MidiOutPort outputDevice = new MidiOutPort(deviceId);
                    outputDevices.Add(outputDevice);
                    // do something with the device, e.g. display its name
                    string deviceName = outputDevice.DeviceInfo.ProductName;
                    Console.WriteLine($"Found output device: {deviceName}");
                }
                catch (MmException ex)
                {
                    // Failed to open the device, handle the exception
                    Console.WriteLine($"Failed to open MIDI output device {deviceId}: {ex.Message}");
                }
            }
        }
    }
}

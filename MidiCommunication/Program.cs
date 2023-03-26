using NAudio.Midi;
using System;
using System.Threading;

namespace MidiCommunication
{
    class Program
    {
        static void Main(string[] args)
        {
            int deviceNumber = 0; // change this to the MIDI device number you want to use
            MidiOut midiOut = new MidiOut(deviceNumber);
            //midiOut.Send(MidiMessage.Start); // send a MIDI start message, needs to be StartNote(60, 127, 0).RawData)
            midiOut.Send(MidiMessage.StartNote(60, 127, 1).RawData); // send a MIDI start message, needs to be StartNote(60, 127, 0).RawData)
            Thread.Sleep(5000); // Sleep for 5 seconds (5000 milliseconds)
            midiOut.Send(MidiMessage.StopNote(60, 0, 1).RawData); // send a MIDI start message, needs to be StartNote(60, 127, 0).RawData)
            midiOut.Close(); // close the MIDI output device
        }
    }
}

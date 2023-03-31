using System;
using System.Timers;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections.Generic;
using System.Threading;
using LibraryConsoleInterface;
using LibraryNetworkCommunication;

namespace LibraryCommandProcessor
{
    public class CommandProcessor
    {
        private static string localDevice;
        private static ConsoleInterface consoleInterface = new ConsoleInterface();
        private static NetworkCommunication localNetwork = new NetworkCommunication();
        private static Dictionary<string, deviceStateData> devicesControlData = new Dictionary<string, deviceStateData>();
        private static Thread clientThread = new Thread(ReadNetworkCommands);

        public CommandProcessor(string localDevice)
        {
            clientThread.Start();
            CommandProcessor.localDevice = localDevice;
        }

        public class deviceStateData
        {
            private int addedDeviceAt;
            private int addedLastCommandAt;
            private string lastCommand;
            private string lastChecksum;

            public string LastCommand { get { return lastCommand; } }
            public string LastChecksum { get { return lastChecksum; } }

            public deviceStateData()
            {
                addedDeviceAt = Environment.TickCount;
                lastCommand = null;
                foreach (KeyValuePair<string, deviceStateData> deviceData in devicesControlData)
                {
                    if (deviceData.Value.outdatedDevice())
                    {
                        localNetwork.DropRemoteDevice(deviceData.Key);
                        devicesControlData.Remove(deviceData.Key);
                    }
                }
            }

            public bool outdatedDevice()
            {
                return ((uint)(Environment.TickCount - addedDeviceAt) > 12*60*60*1000);
            }

            public bool validCommand(string checksum)
            {
                return (lastCommand != null && lastChecksum == checksum);
            }

            public void setLastCommand (string command)
            {
                lastCommand = command;
                lastChecksum = string.Format("{0:X}", TextCRC16(command));
                addedLastCommandAt = Environment.TickCount;
            }

            public void resetLastCommand()
            {
                lastCommand = null;
            }

            private static ushort TextCRC16(string textString)
            {
                char[] text = textString.ToCharArray();
                int length = textString.Length;
                const uint polynomial = 0x1021;
                uint crc = 0xFFFF;

                for (int i = 0; i < length; i++)
                {
                    crc ^= (uint)text[i] << 8;
                    for (byte j = 0; j < 8; j++)
                    {
                        crc = (crc & 0x8000) != 0 ? crc << 1 ^ polynomial : crc << 1;
                    }
                }

                return (ushort)crc;
            }

            public override string ToString() 
            {
                return lastCommand != null ? lastCommand : "";
            }
        }

        private static bool checkDeviceStateData(string device)
        {
            if (!devicesControlData.ContainsKey(device))
            {
                devicesControlData[device] = new deviceStateData();
                return false;
            }

            return true;
        }

        public static bool ReadEnteredCommands()
        {
            string command = consoleInterface.ReadCommand();
            if (command != null)
            {
                command = CleanCommand(command);
                string[] words = command.Split(' ');
                string remoteDevice = words[words.Length - 1];

                if (!IsACommand(remoteDevice))
                {
                    if (!devicesControlData.ContainsKey(remoteDevice))
                    {
                        remoteDevice = null; // TO BE REVIEWED
                    }
                }

                words[0] = words[0].ToLower();

                if (words[0] == "list")
                {
                    List<string> networkNames = NetworkCommunication.GetNetworkNames();
                    foreach (string name in networkNames)
                    {
                        consoleInterface.WriteInterface(name);
                    }
                    consoleInterface.allowInterfaceRead();
                }
                else if (words[0] == "exit")
                {
                    return false;
                }
                else if (IsACommand(words[0]))
                {
                    localNetwork.SendCommand(command, localDevice, remoteDevice);
                }
            }
            return true;
        }

        private static void ReadNetworkCommands()
        {
            NetworkCommunication.NetworkCommand commandData = localNetwork.ReceiveCommand();
            if (commandData != null)
            {
                string command = commandData.Command;
                string remoteDevice = commandData.RemoteDevice;
                string[] words = command.Split(' ');
                checkDeviceStateData(remoteDevice); // makes sure there is a data class

                if (words[0] != "checksum") // common command to be registed NOT triggered
                {
                    devicesControlData[remoteDevice].setLastCommand(command);
                }
                else if (words.Length == 2) // triggered commands by checksum command
                {
                    string checksum = devicesControlData[remoteDevice].LastChecksum;

                    if (devicesControlData[remoteDevice].validCommand(words[1]))
                    {
                        string[] lastWords = devicesControlData[remoteDevice].LastCommand.Split(' ');

                        if (lastWords[0] == "manifesto")
                        {
                            consoleInterface.WriteInterface($"{remoteDevice} Manifesto:");
                            for (uint i = 1; i < words.Length; i++)
                            {
                                consoleInterface.WriteInterface($" {words[i]}");
                            }
                            consoleInterface.WriteInterface("\n");
                            consoleInterface.allowInterfaceRead();
                        }
                        command = $"ok {checksum}";
                    }
                    else
                    {
                        command = $"fail {checksum}";
                    }
                    localNetwork.SendCommand(command, localDevice, remoteDevice); // send my checksum in response to remote checksum (pair)
                    devicesControlData[remoteDevice].resetLastCommand();
                }
            }
        }

        private static string CleanCommand(string command)
        {
            Regex.Replace(command, @"\s+", " ");
            return command;
        }

        private static bool IsACommand (string word)
        {
            if (word == "reveal" || word == "manifesto" || word == "get")
            {
                return true;
            }
            return false;
        }
    }
}

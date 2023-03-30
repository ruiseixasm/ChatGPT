using System;
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
        private static bool lockedInput = false;
        private static ConsoleInterface consoleInterface = new ConsoleInterface();
        private static NetworkCommunication localNetwork = new NetworkCommunication();
        private static Thread clientThread = new Thread(ReadNetworkCommands);
        private static Dictionary<string, string> lastCommands = new Dictionary<string, string>();
        private static Dictionary<string, string> commandsChecksum = new Dictionary<string, string>();

        public CommandProcessor()
        {
            clientThread.Start();
        }

        public static bool ReadEnteredCommands()
        {
            if (!lockedInput)
            {
                string command = CleanCommand(consoleInterface.ReadCommand());
                string[] words = command.Split(' ');

                words[0] = words[0].ToLower();

                if (words[0] == "list")
                {
                    List<string> networkNames = NetworkCommunication.GetNetworkNames();
                    foreach (string name in networkNames)
                    {
                        Console.WriteLine(name);
                    }
                }
                else if (words[0] == "recall")
                {
                    localNetwork.RecallAddresses();
                }
                else if (words[0] == "get")
                {
                    if (words.Length > 1 && words[1] == "manifesto")
                    {
                        lockedInput = true;

                    }
                }
                else if (words[0] == "exit")
                {
                    return false;
                }
            }
            return true;
        }

        private static void ReadNetworkCommands()
        {
            string[] command = localNetwork.ReceiveString();
            if (command != null && command.Length == 2)
            {
                command[0] = CleanCommand(command[0]);
                string[] words = command[0].Split(' ');
                if (words[0] != "checksum")
                {
                    lastCommands[command[1]] = command[0];
                    commandsChecksum[command[1]] = string.Format("{0:X}", TextCRC16(command[0]));
                    string[] sendString = { $"checksum {commandsChecksum[command[1]]}", command[1] };
                    localNetwork.SendString(sendString);
                }
                else if (!string.IsNullOrEmpty(lastCommands[command[1]]) && command[0] == "checksum" && lockedInput == true)
                {
                    if (commandsChecksum[command[1]] == words[1])
                    {
                        string[] last_words = lastCommands[command[1]].Split(' ');

                        if (words[0] == "manifesto")
                        {
                            Console.Write($"{command[1]} Manifesto:");
                            for (uint i = 1; i < words.Length; i++)
                            {
                                Console.Write($" {words[i]}");
                            }
                            Console.WriteLine("");
                        }
                    }
                    lockedInput = false;
                    lastCommands[command[1]] = null;
                }
            }
        }

        private static string CleanCommand(string command)
        {
            Regex.Replace(command, @"\s+", " ");
            return command;
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
    }
}

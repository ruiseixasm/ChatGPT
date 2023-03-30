using System;

namespace CRC16_StringChecksum
{
    public class Checksum
    {
        static UInt16 TextCRC16 (char[] text, byte length)
        {
            const UInt32 polynomial = 0x1021;
            UInt32 crc = 0xFFFF;

            for (byte i = 0; i < length; i++)
            {
                crc ^= (UInt32)text[i] << 8;
                for (byte j = 0; j < 8; j++)
                {
                    crc = (crc & 0x8000) != 0 ? (crc << 1) ^ polynomial : (crc << 1);
                }
            }

            return (UInt16)crc;
        }
    }
}
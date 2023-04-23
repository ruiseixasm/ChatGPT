#include <iostream>

int hexStrToInt_old(const char* hexString) {
    int result = 0;
    while (*hexString) {
        char digit = *hexString++;
        if (digit >= '0' && digit <= '9') {
            digit -= '0';
        }
        else if (digit >= 'a' && digit <= 'f') {
            digit -= 'a' - 10;
        }
        else if (digit >= 'A' && digit <= 'F') {
            digit -= 'A' - 10;
        }
        else {
            // invalid character
            return -1;
        }
        result = result * 16 + digit;
    }
    return result;
}

void intToHexStr_old(int value, char* hexString) {
    int i = 0;
    int total_nibbles = value / 16;
    while (*hexString) {
        int nibble = (value >> ((7 - i++) * 4)) & 0xf;
        if (nibble < 10) {
            *hexString = '0' + nibble;
        }
        else {
            *hexString = 'A' + nibble - 10;
        }
        hexString++;
    }

    //for (int i = 0; i < 8; i++) {
    //    int nibble = (value >> ((7 - i) * 4)) & 0xf;
    //    if (nibble < 10) {
    //        hexString[i] = '0' + nibble;
    //    }
    //    else {
    //        hexString[i] = 'A' + nibble - 10;
    //    }
    //}
    //hexString[8] = '\0';

}


int main(int argc, char* argv[]) {

    char hexString[] = "F4D";
    int intValue = hexStrToInt_old(hexString);
    std::cout << intValue << std::endl;

    intToHexStr_old(intValue, hexString);
    std::cout << hexString << std::endl;

	return 0;
}
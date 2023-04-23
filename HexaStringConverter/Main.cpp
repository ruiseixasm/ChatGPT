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

int IntegerFromHexString(const char* hexString, const int size) {
	int result = 0;
	for (int i = 0; i < size; i++) {
		char digit = hexString[i];
		if (digit >= '0' && digit <= '9') {
			digit -= '0';
		}
		else if (digit >= 'a' && digit <= 'f') {
			digit -= 'a' - 10;
		}
		else if (digit >= 'A' && digit <= 'F') {
			digit -= 'A' - 10;
		}
		else if (digit == '\0') {
			break;
		}
		else {
			// invalid character
			return -1;
		}
		result = result * 16 + digit;
	}
	return result;
}

void ConcatHexIntegerToString(int value, char* hexString, const int size) {

	int last_char;
	for (last_char = 0; hexString[last_char] != '\0'; last_char++) {
		if (last_char == size) {
			return;
		}
	}

	char hexa_char[4]; // 65536 = 2^16
	int first_not_null = -1;
	for (int i = 0; i < 4; i++) {
		int nibble = (value >> ((3 - i) * 4)) & 0xf;
		if (first_not_null == -1 && nibble > 0) {
			first_not_null = i;
		}
		if (nibble < 10) {
			hexa_char[i] = '0' + nibble;
		}
		else {
			hexa_char[i] = 'A' + nibble - 10;
		}
	}
	first_not_null = first_not_null < 0 ? 3 : first_not_null;

	if (size - 1 - last_char < 4 - first_not_null) {
		return;
	}

	for (int i = first_not_null; i < 4; i++) {
		hexString[last_char] = hexa_char[i];
		last_char++;
	}
	hexString[last_char] = '\0';

}


int main(int argc, char* argv[]) {

	char hexString[] = "F4D";
	int intValue = IntegerFromHexString(hexString, sizeof hexString);
	std::cout << intValue << std::endl;

	hexString[0] = '\0';
	ConcatHexIntegerToString(intValue, hexString, sizeof hexString);
	std::cout << hexString << std::endl;

	return 0;
}
#include <iostream>

class CommandWords {
private:
	char* device_name;
	int name_size;
	char** command_words;
	int* command_sizes;
	int word_count;
	uint16_t checksum;
public:
	CommandWords(char* full_command, int commad_size) { // For netwoek usage
		checksum = TextCRC16(full_command, commad_size);
		word_count = CountWords(full_command, commad_size);
		command_words = new char* [word_count - 1];
		command_sizes = new int[word_count - 1];

		bool isWord = false;
		int actualWord = 0;
		int totalChars = 0;
		// ascii as valid word char from 33 to 126 and 32 for [SPACE]
		for (int i = 0; i < commad_size; i++) {
			if (full_command[i] > 32 && full_command[i] < 127) {
				totalChars++;
				isWord = true;
			}
			else {
				if (isWord) {
					char* new_word = new char[totalChars];
					for (int j = 0; j < totalChars; j++) {
						int global_char = i - totalChars + j;
						new_word[j] = full_command[global_char];
					}
					if (actualWord == 0) {
						device_name = new_word;
						name_size = totalChars;
					}
					else {
						command_words[actualWord - 1] = new_word;
						command_sizes[actualWord - 1] = totalChars;
					}
					actualWord++;
					totalChars = 0;
				}
				isWord = false;
			}
		}
	}
	CommandWords(char* full_command, int commad_size, char* device, int size) { // for console usage


	}
	~CommandWords() {
		delete[] device_name;
		for (int i = 0; i < word_count - 1; i++) {
			delete[] command_words[i];
		}
		delete[] command_words;
		delete[] command_sizes;
	}

private:

	static int CountWords(char* big_string, int string_size) {
		bool isWord = false;
		int totalWords = 0;

		// ascii as valid word char from 33 to 126 and 32 for [SPACE]
		for (int i = 0; i < string_size; i++) {
			if (big_string[i] > 32 && big_string[i] < 127) {
				if (!isWord) {
					totalWords++;
				}
				isWord = true;
			}
			else {
				isWord = false;
			}
		}
		return totalWords;
	}

	static uint16_t TextCRC16(const char text[], const int &length)
	{
		const uint16_t polynomial = 0x1021;
		uint16_t crc = 0xFFFF;

		for (int i = 0; i < length; i++)
		{
			crc ^= (uint16_t)text[i] << 8;
			for (int j = 0; j < 8; j++)
			{
				crc = (crc & 0x8000) != 0 ? crc << 1 ^ polynomial : crc << 1;
			}
		}
		return crc;
	}

};

class StringUtils {




};


int main(int argc, char* argv[]) {
	std::cout << "Hello World!" << std::endl;
	return 0;
}
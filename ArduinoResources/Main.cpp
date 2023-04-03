#include <iostream>

// Full Command Structure
// 0                    1                2               3                  4                 5
// (SOURCE_NAME_DEVICE) COMMMAND_KEYWORD FIRST_PARAMETER [SECOND_PARAMETER] [THIRD_PARAMETER] [DESTINATION_NAME_DEVICE]

class CommandWords {
private:
	char** command_words;
	int* word_sizes;
	int word_count = 0;
	uint16_t command_checksum;
	bool validated_command = false;
public:
	CommandWords(const char* full_command, const int commad_size) { // For netwoek usage
		command_words = nullptr;
		command_checksum = TextCRC16(full_command, commad_size);
		GenerateWords(full_command, commad_size);

		std::cout << "NEW command: ";
		std::cout << word_count;
		std::cout << " words." << std::endl;
	}
	~CommandWords() {
		std::cout << "OLD command: ";
		std::cout << word_count;
		std::cout << " words." << std::endl;

		for (int i = 0; i < word_count - 1; i++) {
			delete[] command_words[i];
		}
		delete[] command_words;
		delete[] word_sizes;
	}

	int getWordCount() const {
		return word_count;
	}

	char* getCommandWord(const int word) const {
		if (word < word_count) {
			return command_words[word];
		}
		return nullptr;
	}

	int getCommandWordSize(const int word) const {
		if (word < word_count) {
			return word_sizes[word];
		}
		return 0;
	}

	bool validateCommand(const uint16_t checksum) {
		validated_command = (checksum == command_checksum);
		return validated_command;
	}

	bool ValidCommand() {
		return validated_command;
	}

	int FillBuffer(char* buffer, const int size) {
		int buffer_char = 0;
		for (int i = 0; i < word_count; i++) {
			for (int j = 0; j < word_sizes[i]; j++) {
				if (buffer_char < size - 1) {
					buffer[buffer_char] = command_words[i][j];
					buffer_char++;
				}
			}
			if (i < word_count - 1 && buffer_char < size - 1) {
				buffer[buffer_char] = ' ';
				buffer_char++;
			}
		}
		buffer[buffer_char] = '\0';
		return buffer_char + 1;
	}

private:

	void GenerateWords(const char* full_command, const int& commad_size) {

		do {

			if (word_count > 0) {
				command_words = new char* [word_count];
				word_sizes = new int[word_count];
			}

			bool isWord = false;
			bool isInnerWord = false;
			int actualWord = 0;
			int totalChars = 0;
			int charOffset = 0;
			// ascii as valid word char from 33 to 126
			// 32 for [SPACE]
			// 34 for "
			// 39 for '
			// 96 for `
			for (int i = 0; i < commad_size; i++) {
				if (isInnerWord || full_command[i] > 32 && full_command[i] < 127) {
					if (full_command[i] == 34 || full_command[i] == 39 || full_command[i] == 96) {
						if (isInnerWord) {
							isInnerWord = false;
						}
						else if (i > 0 && charOffset == 0) {
							isInnerWord = true;
						}
						else {
							totalChars--;
							charOffset = -1;
						}
					}
					totalChars++;
					isWord = true;
				}
				else {
					if (isWord && totalChars > 0) {
						if (word_count > 0) {
							char* new_word = new char[totalChars];
							for (int j = 0; j < totalChars; j++) {
								int global_char = i - totalChars + j + charOffset;
								new_word[j] = full_command[global_char];
							}
							command_words[actualWord] = new_word;
							word_sizes[actualWord] = totalChars;
						}
						actualWord++;
						totalChars = 0;
					}
					isWord = false;
				}
			}
			word_count = actualWord;

		} while (command_words == nullptr);
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
	std::cout << "Starting..." << std::endl << std::endl;

	char big_command[] = "This is a Big command!";
	CommandWords firstCommand(big_command, sizeof big_command);

	char another_command[] = "This is a command os another \"inner command\" to execute";
	CommandWords secondCommand(another_command, sizeof another_command);

	// stack auto deleted objects!

	return 0;
}
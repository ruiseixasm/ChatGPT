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
				else {
					buffer[buffer_char] = '\0';
					return buffer_char + 1;
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

	bool isQuotationMark(const char& command_char) {
		// ascii as valid word char from 33 to 126
		// 32 for [SPACE]
		// 34 for "
		// 39 for '
		// 96 for `
		return (command_char == 34 || command_char == 39 || command_char == 96);
	}

	bool isCommandChar(const char& command_char) {
		// ascii as valid word char from 33 to 126
		// 32 for [SPACE]
		// 34 for "
		// 39 for '
		// 96 for `
		return (command_char > 32 && command_char < 127);
	}

	void GenerateWords(const char* full_command, const int& commad_size) {

		int firstChar = 0;
		int commandSize = commad_size;
		if (isQuotationMark(full_command[0])) {
			if (isQuotationMark(full_command[commad_size - 1])) {

				firstChar++;
				commandSize--;
			}
			else if (isQuotationMark(full_command[commad_size - 2])) {

				firstChar++;
				commandSize = commad_size - 2;
			}
		}

		int wordCount = 0;

		do {

			if (word_count > 0) {
				command_words = new char* [word_count];
				word_sizes = new int[word_count];
			}

			bool isInnerWord = false;
			int totalChars = 0;
			int totalWords = 0;

			for (int i = firstChar; i < commandSize; i++) {
				// ascii as valid word char from 33 to 126
				// 32 for [SPACE]
				// 34 for "
				// 39 for '
				// 96 for `
				if (isQuotationMark(full_command[i])) {
					if (isInnerWord) {
						isInnerWord = false; // for last quotation mark
					}
					else {
						isInnerWord = true; // for first quotation mark
					}
				}
				if (isInnerWord || isCommandChar(full_command[i])) {
					totalChars++;
					if (!isInnerWord && (i == commandSize - 1 || !isCommandChar(full_command[i + 1]))) {
						totalWords++;
						if (word_count > 0) {
							char* new_word = new char[totalChars];
							for (int j = 0; j < totalChars; j++) {
								int global_char = i - (totalChars - 1) + j;
								new_word[j] = full_command[global_char];
							}
							command_words[totalWords - 1] = new_word;
							word_sizes[totalWords - 1] = totalChars;
						}
						totalChars = 0;
					}
				}
			}
			word_count = totalWords;

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

	char big_command[] = "This   is a Big   command!";
	CommandWords firstCommand(big_command, sizeof big_command);

	//                        0    1  2     3       4  5        6                     7  8
	char another_command[] = "This is a     command os another \"inner   command\"    to execute";
	CommandWords secondCommand(another_command, sizeof another_command);

	CommandWords innerCommand(secondCommand.getCommandWord(6), secondCommand.getCommandWordSize(6));

	// Fill up a buffer
	char buffer[1024];

	int total_chars = firstCommand.FillBuffer(buffer, sizeof buffer);
	std::cout << "Assembled first phrase: ";
	std::cout << buffer;
	std::cout << " with ";
	std::cout << total_chars;
	std::cout << " char." << std::endl;

	total_chars = secondCommand.FillBuffer(buffer, sizeof buffer);
	std::cout << "Assembled second phrase: ";
	std::cout << buffer;
	std::cout << " with ";
	std::cout << total_chars;
	std::cout << " char." << std::endl;

	char* innerWord = secondCommand.getCommandWord(6);
	total_chars = secondCommand.getCommandWordSize(6); // 15
	std::cout << "Existent inner phrase: ";
	std::cout << innerWord;
	std::cout << " with ";
	std::cout << total_chars;
	std::cout << " char." << std::endl;

	total_chars = innerCommand.FillBuffer(buffer, sizeof buffer);
	std::cout << "Assembled inner phrase: ";
	std::cout << buffer;
	std::cout << " with ";
	std::cout << total_chars;
	std::cout << " char." << std::endl;

	// stack auto deleted objects!

	return 0;
}
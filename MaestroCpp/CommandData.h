#ifndef COMMAND_DATA_H
#define COMMAND_DATA_H

#include <iostream>
#include <chrono>

// Full Command Structure
// 0                    1                2               3                  4                 5
// (SOURCE_NAME_DEVICE) COMMMAND_KEYWORD FIRST_PARAMETER [SECOND_PARAMETER] [THIRD_PARAMETER] [DESTINATION_NAME_DEVICE]

class CommandData {
private:
	char** command_words;
	int* word_sizes;
	int word_count = 0;
	uint16_t command_checksum;
	bool validated_command = false;
public:
	CommandData(const char* full_command, const int commad_size); // For netwoek usage
	~CommandData();

	int getWordCount() const;
	char* getCommandWord(const int word) const;
	int getCommandWordSize(const int word) const;
	bool validateCommand(const uint16_t checksum);
	bool isValidCommand() const;
	int FillBuffer(char* buffer, const int size) const;

private:
	static bool isQuotationMark(const char& command_char);
	static bool isCommandChar(const char& command_char);
	void GenerateWords(const char* full_command, const int& commad_size);
	static uint16_t TextCRC16(const char text[], const int& length);
};


#endif /* COMMAND_DATA_H */
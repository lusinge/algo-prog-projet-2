#include "text.h"

char* cut_str(char* str)
{
	char* result = (char*) malloc(sizeof(char) * MAX_WORD_LENGTH);
	int len = strlen(str);
	int i = 0;

	while(i < len && str[i] != ' ' && i < MAX_WORD_LENGTH - 1) {
		result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return result;
}

bool has_prefix(char* word, char* prefix)
{
	return strncmp(word, prefix, strlen(prefix)) == 0;
}

/*
 * This code is in the major part taken from
 * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/search-bar.c
 * I added a button to make the whole thing more intuitive.
 */

#include "gui.h"

int main(int argc, char *argv[])
{
	AppData data;
	const char* dictionaryFileName;
	char** suggestions;
	unsigned int n;
	unsigned int i;

	dictionaryFileName = "mots_courants.txt";

	data.hashTab = (HashTable*) malloc(sizeof(HashTable));
	initializeHashTable(data.hashTab);
	loadDictionaryFromFile(data.hashTab, dictionaryFileName);

	search_window(argc, argv, &data);

	printf("Final search text: %s\n", data.search_text);

	free(data.hashTab);

	return 0;
}

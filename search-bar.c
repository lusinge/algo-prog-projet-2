/* This code is in the major part taken and inspired from
 * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/
 * I modified the search bar example, and learned how to use GTK in full C from it.
 */
#include "gui.h"

int main(int argc, char *argv[])
{
	AppData* data;
	const char* dictionaryFileName;
	char** suggestions;
	unsigned int n;
	unsigned int i;

	dictionaryFileName = "mots_courants.txt";

	data = (AppData*) malloc(sizeof(AppData));
	data->hashTab = (HashTable*) malloc(sizeof(HashTable));
	printf("hashTab: %p, size: %lu, nbOccupiedEntries: %u, nbElements: %u\n",
       (void *)data->hashTab, data->hashTab->size, data->hashTab->nbOccupiedEntries, data->hashTab->nbElements);
	initializeHashTable(data->hashTab);
	loadDictionaryFromFile(data->hashTab, dictionaryFileName);

	dictionaryFileName = "francais.txt";
	loadDictionaryFromFile(data->hashTab, dictionaryFileName);

	printf("hashTab: %p, size: %lu, nbOccupiedEntries: %u, nbElements: %u\n",
       (void *)data->hashTab, data->hashTab->size, data->hashTab->nbOccupiedEntries, data->hashTab->nbElements);

	search_window(argc, argv, data);

	insertElementToHashTable(data->hashTab, "WHY");
	updateLocalDictionnary("WHY", "mots_courants.txt");

	printf("Final search text: %s\n", data->search_text);

	free(data->hashTab);
	free(data);

	return 0;
}

#include "hash.h"

void initializeHashTable(HashTable* hashTab)
{
	hashTab->size = HASH_SIZE;
	hashTab->nbOccupiedEntries = 0;
	hashTab->nbElements = 0;
	hashTab->Elements = (Element**)malloc(hashTab->size * sizeof(Element*));

	for (unsigned int i = 0; i < hashTab->size; i++)
		hashTab->Elements[i] = NULL;
}



void loadDictionaryFromFile(HashTable* hashTab, const char* dictionaryFileName)
{
	FILE* fp;
	fp = fopen(dictionaryFileName, "r");

	if (fp != NULL)
	{
		char word[MAX_WORD_LENGHT];
		while (fscanf(fp, "%s", word) == 1)
			insertElementToHashTable(hashTab, word);
		fclose(fp);
	}
	else
		printf("File not found.");
}

unsigned long getHashValue(char* string)
{
	unsigned long	hashValue = 0;
	int				i = 0;

	while ((*string) != '\0')
	{
		hashValue += hashValue % HASH_SIZE + ((*string) * (int)pow(BASE, i)) % HASH_SIZE;
		i++;
		string++;
	}
	return hashValue % HASH_SIZE;
}

void insertElementToHashTable(HashTable* hashTab, char* word)
{
	hashTab->nbElements++;
	unsigned long	i = getHashValue(word);
	Element* elem = (Element*)malloc(sizeof(Element));
	strcpy(elem->word, word);

	if (hashTab->Elements[i] == NULL)
		hashTab->nbOccupiedEntries++;

	elem->next = hashTab->Elements[i];
	hashTab->Elements[i] = elem;
}

bool checkExistenceWordInDictionary(HashTable* hashTab, char* word)
{
	unsigned long	hashValue = getHashValue(word);
	Element* elem = hashTab->Elements[hashValue];
	bool		found = 0;

	while (found == 0 && elem != NULL)
	{
		found = (strcmp(word, elem->word) == 0);
		elem = elem->next;
	}
	return found;
}


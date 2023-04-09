#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_WORD_LENGTH	28		/* Maximum word length */
#define HASH_SIZE 37987	/* Prime number */
#define BASE 128



typedef struct _element
{
	char word[MAX_WORD_LENGTH];
	unsigned int frq;
	struct _element* next;

} Element;

typedef struct _hashTable
{

	unsigned int size;
	unsigned int nbOccupiedEntries;
	unsigned int nbElements;
	Element** Elements;

} HashTable;

void loadDictionaryFromFile(HashTable* hashTab, const char* dictionaryFileName);
void insertElementToHashTable(HashTable* hashTab, char* word);
void initializeHashTable(HashTable* hashTab);
bool checkExistenceWordInDictionary(HashTable* hashTab, char* word);

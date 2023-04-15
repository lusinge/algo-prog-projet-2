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

typedef struct {
	char word[MAX_WORD_LENGTH];
	unsigned int frq;
} TopWord;

void loadDictionaryFromFile(HashTable* hashTab, const char* dictionaryFileName);
void insertElementToHashTable(HashTable* hashTab, char* word);
void initializeHashTable(HashTable* hashTab);
bool checkExistenceWordInDictionary(HashTable* hashTab, char* word);
void findTopThreeWordsWithPrefix(HashTable* hashTab, const char* prefix, TopWord topThreeWords[3]);
void updateLocalDictionnary(char* word, const char* dictionaryFileName);
int removeElementFromHashTable(HashTable *hashTab, char *word);
bool editElementFromHashTable(HashTable *hashTab, char *word, char *new_word);
int removeWord(HashTable* hashTab, char* word, char* file_name);
void editWord(HashTable* hashTab, char* old_word, char* new_word, char* file_name);

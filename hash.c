#include "hash.h"

void initializeHashTable(HashTable* hashTab)
{
	hashTab->size = HASH_SIZE;
	hashTab->nbOccupiedEntries = 0;
	hashTab->nbElements = 0;
	hashTab->Elements = (Element**)malloc(hashTab->size * sizeof(Element*));

	for (unsigned int i = 0; i < hashTab->size; i++)
		hashTab->Elements[i] = NULL;

	printf("hashTab: %p, size: %lu, nbOccupiedEntries: %u, nbElements: %u\n",
	   (void *)hashTab, hashTab->size, hashTab->nbOccupiedEntries, hashTab->nbElements);
}



void loadDictionaryFromFile(HashTable* hashTab, const char* dictionaryFileName)
{
	FILE* fp;
	fp = fopen(dictionaryFileName, "r");

	if (fp != NULL)
	{
		char word[MAX_WORD_LENGTH];
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
		hashValue += hashValue % HASH_SIZE + ((*string)
			* (int)pow(BASE, i)) % HASH_SIZE;
		i++;
		string++;
	}

	return hashValue % HASH_SIZE;
}

void insertElementToHashTable(HashTable* hashTab, char* word)
{
	unsigned long i = getHashValue(word);
	Element* elem = hashTab->Elements[i];
	Element* prev = NULL;
	bool found = false;

	// Check if the word is already in the hash table
	while (!found && elem != NULL)
	{
		found = (strcmp(word, elem->word) == 0);
		if (!found)
		{
			prev = elem;
			elem = elem->next;
		}
	}

	if (found)
	{
		// Increment the frequency if the word is found
		elem->frq++;
	}
	else
	{
		// If not found, create a new element and add it to the hash table
		hashTab->nbElements++;
		Element* newElem = (Element*)malloc(sizeof(Element));
		strcpy(newElem->word, word);
		newElem->frq = 1;
		newElem->next = NULL;

		if (prev == NULL)
		{
			// If this is the first element in the linked list
			hashTab->Elements[i] = newElem;
			hashTab->nbOccupiedEntries++;
		}
		else
		{
			// Add the new element at the end of the linked list
			prev->next = newElem;
		}
	}
}

bool checkExistenceWordInDictionary(HashTable* hashTab, char* word)
{
	unsigned long hashValue = getHashValue(word);
	Element* elem = hashTab->Elements[hashValue];
	bool found = false;

	while (!found && elem != NULL)
	{
		found = (strcmp(word, elem->word) == 0);
		elem = elem->next;
	}

	return found;
}

void findTopThreeWordsWithPrefix(HashTable* hashTab, const char* prefix, TopWord topThreeWords[3])
{
	size_t prefixLength = strlen(prefix);

	// Initialize topThreeWords array
	for (int i = 0; i < 3; ++i) {
		topThreeWords[i].word[0] = '\0';
		topThreeWords[i].frq = 0;
	}
	printf("hashTab: %p, size: %lu, nbOccupiedEntries: %u, nbElements: %u\n",
       (void *)hashTab, hashTab->size, hashTab->nbOccupiedEntries, hashTab->nbElements);
	for (unsigned int i = 0; i < hashTab->size; ++i) {
		Element* currentElement = hashTab->Elements[i];

		while (currentElement != NULL) {
			if (strncmp(currentElement->word, prefix, prefixLength) == 0) {
				int j = 0;
				while (j < 3 && currentElement->frq <= topThreeWords[j].frq) {
					j++;
				}

				if (j < 3) {
					// Shift lower frequency words to the right
					for (int k = 2; k > j; --k) {
						topThreeWords[k] = topThreeWords[k - 1];
					}

					// Insert the new word
					strncpy(topThreeWords[j].word, currentElement->word, MAX_WORD_LENGTH);
					topThreeWords[j].frq = currentElement->frq;
				}
			}

			currentElement = currentElement->next;

		}
	}
}

void updateLocalDictionnary(char* word, const char* dictionaryFileName)
{
	FILE* fp;
	fp = fopen(dictionaryFileName, "a");

	if (fp != NULL)
	{
		fprintf(fp,"%s\n", word);
		fclose(fp);
	}
	else
		printf("File not found.");
}

bool removeElementFromHashTable(HashTable *hashTab, char *word)
{
	unsigned long i = getHashValue(word);
	// Check if the index is within bounds
	printf("hashTab: %p, size: %lu, nbOccupiedEntries: %u, nbElements: %u\n",
	   (void *)hashTab, hashTab->size, hashTab->nbOccupiedEntries, hashTab->nbElements);
	if (i >= hashTab->size) {
		// If the index is out of bounds, return
		return false;
	}
	Element *elem = hashTab->Elements[i];
	Element *prev = NULL;
	bool found = false;

	/* Find the element with the given word */
	while (!found && elem != NULL) {
		found = (strcmp(word, elem->word) == 0);
		if (!found) {
			prev = elem;
			elem = elem->next;
		}
	}

	if (!found) {
		/* Word not found, return false */
		return false;
	}

	/* If found, remove the element from the hash table */
	if (prev == NULL) {
		/* If this is the first element in the linked list */
		hashTab->Elements[i] = elem->next;
	} else {
		/* Remove the element by updating the previous element's next pointer */
		prev->next = elem->next;
	}

	/* Deallocate memory for the element */
	free(elem);

	/* Decrement the number of elements in the hash table */
	hashTab->nbElements--;

	/* If the linked list is empty after removing the element, decrement the number of occupied entries */
	if (hashTab->Elements[i] == NULL) {
		hashTab->nbOccupiedEntries--;
	}

	return true;
}

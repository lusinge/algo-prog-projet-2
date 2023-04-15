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

int removeElementFromHashTable(HashTable *hashTab, char *word)
{
	unsigned long i = getHashValue(word);
	unsigned int frq;

	// Check if the index is within bounds
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
		return 0;
	}

	/* If found, remove the element from the hash table and return its frequency*/
	frq = elem->frq;
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

	return frq;
}

int removeWord(HashTable* hashTab, char* word, char* file_name)
{
	FILE* fd;
	FILE* ftmp;
	char tmp_word[MAX_WORD_LENGTH];
	unsigned int frq;

	fd = fopen(file_name, "r");

	if (!fd) {
		printf("\nFichier dictionnaire de prédiction non trouvé.\n");
		return 0;
	}
	else {
		//copie du dico de prédiction sans le mot qui va être supprimé.
		ftmp = fopen("tmp", "w");
		while( fscanf(fd, "%s", tmp_word) != EOF ) {
			if (strcmp(tmp_word, word) != 0)
				fprintf(ftmp,"%s\n", tmp_word);
		}
		fclose(fd);
		fclose(ftmp);
		//on réécrit le dico sans le mot voulu.
		fd = fopen(file_name, "w");
		ftmp = fopen("tmp", "r");
		strcpy(tmp_word, "");
		while( fscanf(ftmp, "%s", tmp_word) != EOF )
			fprintf(fd,"%s\n", tmp_word);
		frq = removeElementFromHashTable(hashTab, word);
		if (frq == 0)
			printf("\nErreur : Mot non trouvé dans la table de hachage.\n");
		fclose(ftmp);
	}
	fclose(fd);
	return frq;
}

void editWord(HashTable* hashTab, char* old_word, char* new_word, char* file_name)
{
	int frq = removeWord(hashTab, old_word, file_name);
	if (frq != 0) {
		for (int i = 0; i < frq; i++) {
			insertElementToHashTable(hashTab, new_word);
			updateLocalDictionnary(new_word, file_name);
			printf("\n%s has been replaced by %s\n", old_word, new_word);
		}
	}
	else
		printf("\n%s non trouvé\n", old_word);
}

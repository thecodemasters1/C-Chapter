/****************************************************************
 * Summary: This program recieves a text file and replaces      *
 *          words according to a given dictionary.              *
 *                                                              *
 * Example: limited_vocab dictionary.txt essay.txt              *
 ****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

#define WRONG_ARGUMENTS			(-1)
#define CANNOT_BUILD_DICTIONARY	(-2)
#define NOT_ENOUGH_MEMORY		(-3)
#define CANNOT_OPEN_VOCAB_FILE	(-4)
#define CANNOT_OPEN_INPUT_FILE	(-5)
#define CANNOT_CLOSE_VOCAB_FILE (-6)
#define CANNOT_CLOSE_INPUT_FILE (-7)

#define MAX_WORD (30)

static int rewrite(FILE *fp_text, list_node_t *dictionary);

static char * read_all(FILE * fp);

int main(int argc, char *argv[])
{
    int rc = 0;
	
	FILE *fp_vocab = NULL;
	FILE *fp_text = NULL;

	list_node_t *dictionary = NULL;

	/* Check argument count */
	if (3 != argc) {
		printf("Usage: %s vocabulary_file text_file\n", argv[0]);
		return WRONG_ARGUMENTS;
	}

	/* Open files */
	fp_vocab = fopen(argv[1], "r");
	if (NULL == fp_vocab) {
		printf("Could not open %s.\n", argv[1]);
		return CANNOT_OPEN_VOCAB_FILE;
	}

	fp_text = fopen(argv[2], "r+");
	if (NULL == fp_text) {
		printf("Could not open %s.\n", argv[2]);
		return CANNOT_OPEN_INPUT_FILE;
	}

	/* Build dictionary */
	dictionary = create_dictionary(fp_vocab);
	if(NULL == dictionary) {
		printf("Not enough memory or dictionary file is in incorrect format.\n"
				"Format is:\n\tword synonym1 synonym2...\n\tword synonym1 synonym2...\n");
		return CANNOT_BUILD_DICTIONARY;
	}

	/* Rewrite text file */
	if(0 != rewrite(fp_text, dictionary)) {
		printf("Not enough memory.\n");
		return NOT_ENOUGH_MEMORY;
	}

	/* Free dictionary */
	list_node_kill(dictionary);

	/* Close files */
	rc = 0;
	
	if (0 != fclose(fp_vocab)) {
		printf("Could not close %s.\n", argv[1]);
		rc = CANNOT_CLOSE_VOCAB_FILE;
	}
	
	if (0 != fclose(fp_text)) {
		printf("Could not close %s.\n", argv[2]);
		rc =  CANNOT_CLOSE_INPUT_FILE;
	}

	return rc;
}

/****************************************************************
 * Summary: Rewrites the given text file, replacing word with   *
 *          synonyms from dictionary.                           *
 *                                                              *
 * Parameters: fp_text - Represents the text file to rewrite.   *
 *             dictionary - A pointer to the first node in the  *
 *                          dictionary with the synonyms.       *
 *                                                              *
 * Returns: 0 if successful, can return NOT_ENOUGH_MEMORY.      *
 ****************************************************************/
static int rewrite(FILE *fp_text, list_node_t *dictionary)
{
	int i = 0, j = 0;
	char *buffer = NULL;
	char temp[MAX_WORD] = { 0 };
	
	/* Save text to buffer */
	buffer = read_all(fp_text);
	if (NULL == buffer) {
		return NOT_ENOUGH_MEMORY;
	}

	/* Reset write index */
	fseek(fp_text, 0, SEEK_SET);

	/* Seperate to words */
	j = 0;
	i = 0;
	while ('\0' != buffer[i]) {
		if (0 == isalpha(buffer[i])) {
			temp[j] = '\0';
			j = 0;
			if (temp[0] != '\0') fprintf(fp_text, "%s", find_synonym(temp, dictionary));
			memset(temp, 0, sizeof(char) * MAX_WORD);
			fputc(buffer[i], fp_text);
		} else {
			temp[j] = buffer[i];
			++j;
		}
		++i;
	}
	if (temp[0] != '\0') fprintf(fp_text, "%s", find_synonym(temp, dictionary));
	
	/* Free memory */
	free(buffer);

	return 0;
}

/****************************************************************
 * Summary: Reads a text file to a buffer.                      *
 *                                                              *
 * Parameters: fp - Represents the text file to read.           *
 *                                                              *
 * Returns: A buffer with the text if successful, otherwise     *
 *          NULL.                                               *
 ****************************************************************/
static char * read_all(FILE * fp)
{
	int i = 0;
	int c = 0;
	int fsize = 0;
	char *buffer = NULL;

	/* Get file size */
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp) + 1;
	fseek(fp, 0, SEEK_SET);

	/* Allocate memory */
	buffer = (char *)malloc(sizeof(char) * fsize);
	if (NULL == buffer) {
		return NULL;
	}

	/* Copy to buffer */
	i = 0;
	while (EOF != (c = fgetc(fp))) {
		buffer[i] = (char)c;
		++i;
	}
	buffer[i] = '\0';

	return buffer;
}
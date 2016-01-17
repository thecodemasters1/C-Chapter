#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

static list_node_t * interpret_line(char *line, list_node_t *last);

/****************************************************************
 * Summary: Generates a dictionary from given dictionary file.  *
 *                                                              *
 * Parameters: fp_dictionary - represents the dictionary file.  *
 *                                                              *
 * Returns: The first node in the dictionary if successful,     *
 *          otherwise NULL.                                     *
 ****************************************************************/
list_node_t * create_dictionary(FILE *fp_dictionary)
{
	char *temp_word = NULL;
	char *temp_line = NULL;
	
	list_node_t *first = NULL;
	list_node_t *last = NULL;
	
	/* Allocate memory */
	temp_line = (char *)malloc(sizeof(char) * DICTIONARY_MAX_LINE);
	if (NULL == temp_line) {
		return NULL;
	}

	/* Build dictionary */
	while (NULL != fgets(temp_line, DICTIONARY_MAX_LINE, fp_dictionary)) {
		last = interpret_line(temp_line, last);

		/* If interpret_line failed, free memory and return NULL */
		if(NULL == last) {
			if (NULL != first) {
				list_node_kill(first);
			}
			free(temp_line);
			return NULL;
		}

		if (NULL == first) {
			first = last;
		}
	}
	
	free(temp_line); /* Free memory */

	return first;
}

/****************************************************************
 * Summary: Converts a line from the dictionary file to a       *
 *          list_node_t for the dictionary.                     *
 *                                                              *
 * Parameters: line - The line to interpret.                    *
 *             last - A pointer to the last node in the         *
 *                    dictionary, the new node is added after   *
 *                    this node.                                *
 *                                                              *
 * Returns: The new node if successful, otherwise NULL.         *
 ****************************************************************/
static list_node_t * interpret_line(char *line, list_node_t *last)
{
	char *temp_word = NULL;
	list_node_t *new_node = NULL;

	/* Get the original word and create a node */
	temp_word = strtok(line, DICTIONARY_SEPERATOR);
	if (NULL != temp_word) {
		new_node = list_node_add(last, temp_word);
		if (NULL == new_node) {
			return NULL;
		}
	} else {
		return NULL;
	}

	/* Add synonyms */
	temp_word = strtok(NULL, DICTIONARY_SEPERATOR);
	while (NULL != temp_word) {
		if (NULL == (*new_node).synonyms) {
			(*new_node).synonyms = circle_node_create(temp_word);
		} else {
			circle_node_add_after((*new_node).synonyms, temp_word);
		}
		temp_word = strtok(NULL, DICTIONARY_SEPERATOR);
	}

	return new_node;
}

/****************************************************************
 * Summary: Searches for a synonym to word within given         *
 *          dictionary.                                         *
 *                                                              *
 * Parameters: word - The original word.                        *
 *             dictionary - A pointer to the first node in the  *
 *                          dictionary with the synonyms.       *
 *                                                              *
 * Returns: A synonym if found, otherwise the original word.    *
 ****************************************************************/
char * find_synonym(char *word, list_node_t *dictionary)
{
	/* Find word */
	while (NULL != dictionary) {
		if (0 == strcmp(strlwr(word), (*dictionary).word)) {/* found */
			return list_node_get_synonym(dictionary);
		} else {
			dictionary = (*dictionary).next;
		}
	}
	/* word is not in dictionary */
	return word;
}
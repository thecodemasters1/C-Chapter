#include <stdlib.h>
#include <string.h>
#include "list.h"

/****************************************************************
 * Summary: Adds a word to the list after pos.                  *
 *                                                              *
 * Parameters: pos - The position to add the new node,          *
 *                   use NULL for the first node.               *
 *             word - The word to add.                          *
 *                                                              *
 * Returns: The new node if successful, otherwise NULL.         *
 ****************************************************************/
list_node_t * list_node_add(list_node_t *pos, char *word)
{
	list_node_t *new_node;

	/* Allocate memory */
	new_node = (list_node_t *)malloc(sizeof(list_node_t));

	if (NULL != new_node) {
		/* Allocate more memory */
		(*new_node).word = (char *)malloc(sizeof(char) * (strlen(word) + 1));
		if (NULL == (*new_node).word) {
			free(new_node);
			return NULL;
		}
		/* Initialize values */
		strcpy((*new_node).word, strlwr(word));
		(*new_node).word[strlen(word)] = '\0';

		(*new_node).synonyms = NULL;
		(*new_node).next = NULL;
		/* Insert at given position */
		if (NULL != pos) {
			(*new_node).next = (*pos).next;
			(*pos).next = new_node;
		}
	}

	return new_node;
}

/****************************************************************
 * Summary: Frees the list.                                     *
 *                                                              *
 * Parameters: node - The first node in the list.               *
 *                                                              *
 * Returns: void.                                               *
 ****************************************************************/
void list_node_kill(list_node_t *node)
{
	/* Kill next */
	if (NULL != (*node).next) {
		list_node_kill((*node).next);
	}
	/* Kill synonyms */
	if (NULL != (*node).synonyms) {
		circle_node_kill((*node).synonyms);
	}
	/* Free memory */
	free((*node).word);
	free(node);
}

/****************************************************************
 * Summary: Adds a synonym to the synonyms in the node.         *
 *                                                              *
 * Parameters: node - The node to add the synonym to.           *
 *             synonym - The synonym to add.                    *
 *                                                              *
 * Returns: 0 if successful, can return CN_NOT_ENOUGH_MEMORY.   *
 ****************************************************************/
int list_node_add_synonym(list_node_t *node, char *synonym)
{
	if (NULL == (*node).synonyms) { /* first synonym */
		(*node).synonyms = circle_node_create(synonym);
		return 0;
	} else {
		return circle_node_add_after((*node).synonyms, synonym);
	}
}

/****************************************************************
 * Summary: Gets a synonym for the word value of the node.      *
 *                                                              *
 * Parameters: node - The node.                                 *
 *                                                              *
 * Returns: The synonym if successful, otherwise NULL.          *
 ****************************************************************/
char * list_node_get_synonym(list_node_t *node)
{
	char *syn = NULL;

	/* Get synonym and move to next. */
	if(NULL != (*node).synonyms) {
		syn = (*(*node).synonyms).word;
		(*node).synonyms = (*(*node).synonyms).next;
	}

	return syn;
}
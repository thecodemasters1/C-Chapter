#include <stdlib.h>
#include <string.h>
#include "cnode.h"

static void _kill_circle(circle_node_t *current, circle_node_t *start);

/****************************************************************
 * Summary: Creates a circle_node_t.                            *
 *                                                              *
 * Parameters: word - The word to place in the new node.        *
 *                                                              *
 * Returns: The new node if successful, otherwise NULL.         *
 ****************************************************************/
circle_node_t * circle_node_create(char *word)
{
	/* Allocate memory */
	circle_node_t *new_node = (circle_node_t *)malloc(sizeof(circle_node_t));

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
		(*new_node).next = new_node;
	}
	
	return new_node;
}

/****************************************************************
 * Summary: Frees a circle_node_t and its entire circle.        *
 *                                                              *
 * Parameters: segment - The node to start at.                  *
 *                                                              *
 * Returns: void.                                               *
 ****************************************************************/
void circle_node_kill(circle_node_t *segment)
{
	/* Kill the circle using segment as its head */
	_kill_circle(segment, segment);	
}

/****************************************************************
 * Summary: Frees a circle_node_t and its entire circle.        *
 *                                                              *
 * Parameters: current - The current node to kill.              *
 *             start - The first node in the recursive call.    *
 *                                                              *
 * Returns: void.                                               *
 ****************************************************************/
static void _kill_circle(circle_node_t *current, circle_node_t *start)
{
	/* Kill next */
	if (NULL != (*current).next && start != (*current).next) {
		_kill_circle((*current).next, start);
	}
	/* Free memory */
	free((*current).word);
	free(current);
}

/****************************************************************
 * Summary: Adds a word to the list after pos.                  *
 *                                                              *
 * Parameters: pos - The position to add the new node.          *
 *             word - The word to add.                          *
 *                                                              *
 * Returns: 0 if successful, can return CN_NOT_ENOUGH_MEMORY.   *
 ****************************************************************/
int circle_node_add_after(circle_node_t *pos, char *word)
{
	circle_node_t *new_node = NULL;
	
	/* Create a new node */
	new_node = circle_node_create(word);
	
	/* If creation falied */
	if (NULL == new_node) {
		return CN_NOT_ENOUGH_MEMORY;
	}
	
	/* Add to after pos */
	(*new_node).next = (*pos).next;
	(*pos).next = new_node;
	
	return 0;
}


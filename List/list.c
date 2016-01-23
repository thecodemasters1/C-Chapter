/****************************************************************
 * Summary: This library implements a bi-directional            *
 *          linked list.                                        *
 ****************************************************************/

#include <stdlib.h>
#include "list.h"

/****************************************************************
 * Summary: Creates an empty list and returns a pointer         *
 *          to the list.                                        *
 *                                                              *
 * Parameters: None.                                            *
 *                                                              *
 * Returns: A pointer to list_t or NULL if failed.              *
 ****************************************************************/
list_t * list_create()
{
	/* Allocate memory. */
	list_t * new_list = (list_t *)malloc(sizeof(list_t));

	/* Check if malloc succeeded. */
	if (new_list == NULL) {
		return NULL;
	}

	/* Initialize members. */
	new_list->count = 0;
	new_list->avg = 0;
	new_list->head = NULL;
	new_list->tail = NULL;

	return new_list;
}

/****************************************************************
 * Summary: Destroys a list, freeing memory.                    *
 *                                                              *
 * Parameters: list - A pointer to the list_t to destroy.       *
 *                                                              *
 * Returns: void.                                               *
 ****************************************************************/
void list_destroy(list_t * list)
{
	/* Free all nodes' memory. */
	while (list->head != NULL) { // T: validate list_t list is not null
		list_remove_node(list, list->head);
	}
	/* Free list memory. */
	free(list);
}

/****************************************************************
 * Summary: Adds value to the end of the list.                  *
 *                                                              *
 * Parameters: list - A pointer to the list_t.                  *
 *             value - The value to add.                        *
 *                                                              *
 * Returns: A pointer to the added list_node_t or NULL          *
 *          if failed.                                          *
 ****************************************************************/
list_node_t * list_add_node(list_t * list, int value)
{
	double sum;
	list_node_t * new_node;
	
	/* Allocate memory. */
	new_node = (list_node_t *)malloc(sizeof(list_node_t));
	
	/* Check if malloc succeeded. */
	if (new_node == NULL) {
		return NULL;
	}

	/* Initialize and add to list. */
	new_node->val = value;
	new_node->prev = list->tail;
	new_node->next = NULL;
	if (list->tail != NULL) { /* if not adding to an empty list */ // T: validate list_t list is not null
		list->tail->next = new_node;
	}
	list->tail = new_node;
	if (list->head == NULL) { /* if adding to an empty list */
		list->head = new_node;
	}

	/* Update list's count and avg. */
	sum = (list->avg) * (list->count); // T: would be easier if you saved the sum instead of the avarage
	sum += value;
	++(list->count);
	list->avg = sum / (list->count);

	return new_node;
}

/****************************************************************
 * Summary: Removes a node from a list.                         *
 *                                                              *
 * Parameters: list - A pointer to the list_t.                  *
 *             node - A pointer to the list_node_t to remove.   *
 *                                                              *
 * Returns: void.                                               *
 ****************************************************************/
void list_remove_node(list_t * list, list_node_t * node)
{
	double sum;

	/* Update list's count and avg. */
	sum = (list->avg) * (list->count); // T: validate list_t list is not null
	sum -= (node->val);
	--(list->count);
	list->avg = sum / (list->count);

	/* Remove from list. */
	if (node->next == NULL) { /* if removing tail */
		list->tail = node->prev;
	} else {
		node->next->prev = node->prev;
	}
	if (node->prev == NULL) { /* if removing head */
		list->head = node->next;
	} else { /* if removing a node from the middle */
		node->prev->next = node->next;
	}
	
	/* Free memory. */
	free(node);
}

/****************************************************************
 * Summary: Gets the amount of values in a list.                *
 *                                                              *
 * Parameters: list - A pointer to the list_t.                  *
 *                                                              *
 * Returns: The amount of values in the list.                   *
 ****************************************************************/
int list_get_count(list_t * list)
{
	return list->count; // T: validate list_t list is not null
}

/****************************************************************
 * Summary: Gets the average of the values in a list.           *
 *                                                              *
 * Parameters: list - A pointer to the list_t.                  *
 *                                                              *
 * Returns: The average of the values in the list.              *
 ****************************************************************/
double list_get_average(list_t * list)
{
	return list->avg; // T: validate list_t list is not null
}

/****************************************************************
 * Summary: Gets the first node in a list.                      *
 *                                                              *
 * Parameters: list - A pointer to the list_t.                  *
 *                                                              *
 * Returns: A pointer to the first list_node_t.                 *
 ****************************************************************/
list_node_t * list_get_head(list_t * list)
{
	return list->head; // T: validate list_t list is not null
}

/****************************************************************
 * Summary: Gets the last node in a list.                       *
 *                                                              *
 * Parameters: list - A pointer to the list_t.                  *
 *                                                              *
 * Returns: A pointer to the last list_node_t.                  *
 ****************************************************************/
list_node_t * list_get_tail(list_t * list)
{
	return list->tail; // T: validate list_t list is not null
}

/****************************************************************
 * Summary: Gets the next node.                                 *
 *                                                              *
 * Parameters: node - A pointer to the list_node_t.             *
 *                                                              *
 * Returns: A pointer to the next list_node_t.                  *
 ****************************************************************/
list_node_t * list_get_next_node(list_node_t * node)
{
	return node->next; // T: validate list_node_t node is not null
}

/****************************************************************
 * Summary: Gets the previous node.                             *
 *                                                              *
 * Parameters: node - A pointer to the list_node_t.             *
 *                                                              *
 * Returns: A pointer to the previous list_node_t.              *
 ****************************************************************/
list_node_t * list_get_prev_node(list_node_t * node)
{
	return node->prev; // T: validate list_node_t node is not null
}

/****************************************************************
 * Summary: Gets the value of a node.                           *
 *                                                              *
 * Parameters: node - A pointer to the list_node_t.             *
 *                                                              *
 * Returns: The value of the node.                              *
 ****************************************************************/
int list_get_node_value(list_node_t * node)
{
	return node->val; // T: validate list_node_t node is not null
}

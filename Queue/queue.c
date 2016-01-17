/****************************************************************
 * Summary: This library implements a queue.					*
 ****************************************************************/

#include <stdlib.h>
#include "queue.h"

/****************************************************************
 * Summary: Creates an empty queue and returns a pointer		*
 *          to the queue.										*
 *																*
 * Parameters: None.											*
 *																*
 * Returns: A pointer to queue_t or NULL if failed.				*
 ****************************************************************/
queue_t * queue_create()
{
	/* Allocate memory. */
	queue_t * new_queue = (queue_t *)malloc(sizeof(queue_t));

	if (new_queue == NULL) {
		return NULL;
	}
	
	/* Initialize members. */
	new_queue->count = 0;
	new_queue->head = NULL;
	new_queue->tail = NULL;

	return new_queue;
}

/****************************************************************
 * Summary: Destroys a queue, freeing memory.					*
 *																*
 * Parameters: queue - A pointer to the queue_t to destroy.		*
 *																*
 * Returns: void.												*
 ****************************************************************/
void queue_destroy(queue_t * queue)
{
	/* Remove all values. */
	while (queue->count > 0) {
		queue_pop(queue, NULL);
	}

	/* Free memory. */
	free(queue);
}

/****************************************************************
 * Summary: Adds value to the end of the queue.					*
 *																*
 * Parameters: queue - A pointer to the queue_t.				*
 *             value - The value to add.						*
 *																*
 * Returns: 0 if completed successfully, -1 if failed.			*
 ****************************************************************/
int queue_push(queue_t * queue, int value)
{
	/* Allocate memory. */
	queue_node_t * temp = (queue_node_t *)malloc(sizeof(queue_node_t));

	if (temp == NULL) {
		return -1;
	}
	
	/* Initialize and add to queue. */
	temp->val = value;
	temp->next = NULL;
	if (queue->tail != NULL) {/* prevent crashing when adding to an empty queue */
		queue->tail->next = temp;
	}
	queue->tail = temp;

	if (queue->head == NULL) {/* upon adding to an empty queue. */
		queue->head = temp;
	}

	/* Update count. */
	++(queue->count);

	/* Completed successfully. */
	return 0;
}

/****************************************************************
 * Summary: Pops a value from a queue.							*
 *																*
 * Parameters: queue - A pointer to the queue_t.				*
 *             out - A pointer to an int to hold the popped		*
 *                   value.										*
 *																*
 * Returns: 0 if completed successfully, -1 if failed.			*
 ****************************************************************/
int queue_pop(queue_t * queue, int *out)
{
	int val;
	queue_node_t * temp;

	if (queue->head == NULL) {/* if popping from an empty queue */
		return -1;
	}

	/* Save removed node and promote node. */
	temp = queue->head;
	queue->head = queue->head->next;

	/* Save value and free memory. */
	val = temp->val;
	free(temp);

	/* Update count. */
	--(queue->count);

	/* Completed successfully. */
	if (out != NULL) {
		*out = val;
	}
	return 0;
}

/****************************************************************
 * Summary: Gets the amount of values in a queue.				*
 *																*
 * Parameters: queue - A pointer to the queue_t.				*
 *																*
 * Returns: The amount of values in the queue.					*
 ****************************************************************/
int queue_get_count(queue_t * queue)
{
	return queue->count;
}

/****************************************************************
 * Summary: Gets the value at the top of the queue without		*
 *          removing it.										*
 *																*
 * Parameters: queue - A pointer to the queue_t.				*
 *             out - A pointer to an int to hold the value.		*
 *																*
 * Returns: 0 if completed successfully, -1 if failed.			*
 ****************************************************************/
int queue_peek(queue_t * queue, int *out)
{
	if (queue->head != NULL) { /* prevent peeking into an empty queue */
		if (out != NULL) {
			*out = queue->head->val;
		}
		return 0;
	} else {
		return -1;
	}
}
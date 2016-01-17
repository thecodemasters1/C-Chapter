#if !defined(_LIST_H_)
#define _LIST_H_

#include "cnode.h"

typedef struct list_node_rec {
	char *word;
	circle_node_t *synonyms;
	struct list_node_rec *next;
} list_node_t;

list_node_t * list_node_add(list_node_t *pos, char *word);

void list_node_kill(list_node_t *first);

int list_node_add_synonym(list_node_t *node);

char * list_node_get_synonym(list_node_t *node);

#endif
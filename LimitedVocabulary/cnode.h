#if !defined(_CNODE_H_)
#define _CNODE_H_

#define CN_NOT_ENOUGH_MEMORY (-1)

typedef struct circle_node_rec {
	char *word;
	struct circle_node_rec *next;
} circle_node_t;

circle_node_t * circle_node_create(char *word);

void circle_node_kill(circle_node_t *segment);

int circle_node_add_after(circle_node_t *pos, char *word);

#endif
typedef struct list_node_rec {
	int val;
	struct list_node_rec * next;
	struct list_node_rec * prev;
} list_node_t;

typedef struct list_rec {
	int count;
	double avg;
	struct list_node_rec * head;
	struct list_node_rec * tail;
} list_t;

list_t * list_create();

void list_destroy(list_t * list);

list_node_t * list_add_node(list_t * list, int value);

void list_remove_node(list_t * list, list_node_t * node);

int list_get_count(list_t * list);

double list_get_average(list_t * list);

list_node_t * list_get_head(list_t * list);

list_node_t * list_get_tail(list_t * list);

list_node_t * list_get_next_node(list_node_t * node);

list_node_t * list_get_prev_node(list_node_t * node);

int list_get_node_value(list_node_t * node);
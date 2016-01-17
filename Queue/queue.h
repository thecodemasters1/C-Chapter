typedef struct queue_node_rec {
	int val;
	struct queue_node_rec * next;
} queue_node_t;

typedef struct queue_rec {
	int count;
	struct queue_node_rec * head;
	struct queue_node_rec * tail;
} queue_t;

queue_t * queue_create();

void queue_destroy(queue_t * queue);

int queue_push(queue_t * queue, int value);

int queue_pop(queue_t * queue, int * out);

int queue_get_count(queue_t * queue);

int queue_peek(queue_t * queue, int * out);
#include <stdio.h>
#include <stdlib.h>
#include "iterator.h"
// mimics the linked list that c++ provides, 
// though without the back pointer. 
struct l_node {
	void *item;
	struct l_node *next;
	struct l_node *prev;
};

// hacky function names, but it fits for now.
int l_node_init
(struct l_node **ln_dp, void *item);

int l_node_set_next
(struct l_node *ln_p, struct l_node *next);

int l_node_set_prev
(struct l_node *ln_p, struct l_node *prev);

struct list;
typedef struct list *List;
	
struct list {

	int size;	
	struct l_node *front;
	struct l_node *back;

	// the end iterator for 
	// checking in while loops
	Iterator end;
};

List 
list_ctor_empty(void);

List 
list_ctor_copy(List);

int list_push_back
(List, void *item);

int list_push_front
(List, void *item);


Iterator 
list_insert(List, Iterator, void *);
	

// for debugging: 
// the function argument is one that is user supplied - 
// since the data is void pointers, and list does not 
// know how to print it.
int list_print
(List lst, void (*user_print)(void *));

void list_destroy(List);
//
// TODO Do we need this? 
void list_destroy_all(List);

// FOR ITERATORS
Iterator list_begin(List);
Iterator list_end(List);
	

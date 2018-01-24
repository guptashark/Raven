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
	
struct linked_list {

	int size;	
	struct l_node *front;
	struct l_node *back;

	// the end iterator for 
	// checking in while loops
	Iterator end;
};

struct linked_list *
ll_ctor_empty(void);

struct linked_list *
ll_ctor_copy(struct linked_list *);

int ll_push_back
(struct linked_list *ll_p, void *item);

int ll_push_front
(struct linked_list *ll_p, void *item);

// for debugging: 
// the function argument is one that is user supplied - 
// since the data is void pointers, and ll does not 
// know how to print it.
int ll_print
(struct linked_list *ll_p, void (*user_print)(void *));

void ll_destroy(struct linked_list *);
void ll_destroy_all(struct linked_list *);

// FOR ITERATORS
Iterator
ll_begin(struct linked_list *ll_p);

Iterator 
ll_end(struct linked_list *ll_p);
	

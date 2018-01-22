#include <stdio.h>
#include <stdlib.h>
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
};

int ll_init
(struct linked_list **ll_dp);

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


	

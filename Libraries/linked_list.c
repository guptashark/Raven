#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
// mimics the linked list that c++ provides, 
// though without the back pointer. 
// hacky function names, but it fits for now.
int l_node_init
(struct l_node **ln_dp, void *item) {
	struct l_node *ret = NULL;
	ret = malloc(sizeof(struct l_node));
	ret->item = item;
	ret->next = NULL;
	ret->prev = NULL;
	
	*ln_dp = ret;
	return 0;
}

int l_node_set_next
(struct l_node *ln_p, struct l_node *next) {
	ln_p->next = next;
	return 0;
}

int l_node_set_prev
(struct l_node *ln_p, struct l_node *prev) {
	ln_p->prev = prev;
	return 0;
}

int ll_init
(struct linked_list **ll_dp) {
	struct linked_list *ret = NULL;
	ret = malloc(sizeof(struct linked_list));
	
	struct l_node *front = NULL;
	struct l_node *back = NULL;
	l_node_init(&front, NULL);
	l_node_init(&back, NULL);
	
	ret->front = front;
	ret->back = back;
	
	l_node_set_next(front, back);
	l_node_set_prev(back, front);		

	ret->size = 0;
	*ll_dp = ret;
	return 0;
}

int ll_push_back
(struct linked_list *ll_p, void *item) {

	struct l_node *to_add = NULL;
	l_node_init(&to_add, item);

	l_node_set_next(to_add, ll_p->back);
	l_node_set_next(ll_p->back->prev, to_add);

	l_node_set_prev(to_add, ll_p->back->prev);
	l_node_set_prev(ll_p->back, to_add);

	ll_p->size++;

	return 0;
}

int ll_push_front
(struct linked_list *ll_p, void *item) {

	struct l_node *to_add = NULL;
	l_node_init(&to_add, item);
	
	l_node_set_prev(to_add, ll_p->front);
	l_node_set_prev(ll_p->front->next, to_add);

	l_node_set_next(to_add, ll_p->front->next);
	l_node_set_next(ll_p->front, to_add);

	ll_p->size++;
	return 0;
}

// for debugging: 
// the function argument is one that is user supplied - 
// since the data is void pointers, and ll does not 
// know how to print it.
int ll_print
(struct linked_list *ll_p, void (*user_print)(void *)) {

	struct l_node *current = ll_p->front->next;
	for(int i = 0; i < ll_p->size; i++) {
		user_print(current->item);
		current = current->next;
		printf("\t");
	}
	printf("\n");
	return 0;
}

	

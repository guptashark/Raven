#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"



// *******************************
// ITERATORS SECTION 
// *******************************

// functions to increment, decrement, deref, cmp
// might as well define these here so that the list
// init function can use them. 

int ll_iterator_increment(Iterator bi_p) {
	
	struct l_node *current = (struct l_node *)bi_p->data;
	bi_p->data = current->next;
	return 0;
}

int ll_iterator_decrement(Iterator bi_p) {
	
	struct l_node *current = (struct l_node *)bi_p->data;
	bi_p->data = current->prev;
	return 0;
}

void *
ll_iterator_deref
(Iterator bi_p) { 

	struct l_node *current = (struct l_node *)bi_p->data;
	return current->item;
}

int ll_iterator_cmp
(Iterator self, Iterator other) {
	
	// main idea is to do a raw comparison - 
	// is the node we're at the node in other? 
	struct l_node *current = (struct l_node *)self->data;
	struct l_node *other_current = (struct l_node *)other->data;
	if(current == other_current) {
		return 1;
	} else {
		return 0;
	}
}


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

struct linked_list *
ll_ctor_empty(void) {
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


	// set up the end iterator	
	ret->end = malloc(sizeof(struct iterator));
	ret->end->container = ret;
	ret->end->data = ret->back;
	ret->end->increment = ll_iterator_increment;
	ret->end->decrement = ll_iterator_decrement;
	ret->end->deref = ll_iterator_deref;
	ret->end->cmp = ll_iterator_cmp;

	return ret;
}

struct linked_list *
ll_ctor_copy(struct linked_list *source) {
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
//
	//now, copy over all the elements from source. 
	Iterator i;
	for(
		i = ll_begin(source);
		!iter_cmp(i, ll_end(source));
		iter_increment(i)
	) {
		ll_push_back(ret, iter_deref(i));
	}


	// set up the end iterator	
	ret->end = malloc(sizeof(struct iterator));
	ret->end->container = ret;
	ret->end->data = ret->back;
	ret->end->increment = ll_iterator_increment;
	ret->end->decrement = ll_iterator_decrement;
	ret->end->deref = ll_iterator_deref;
	ret->end->cmp = ll_iterator_cmp;

	return ret;

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

void ll_destroy
(struct linked_list *lst) {
	struct l_node *current = lst->front;
	struct l_node *next = current->next;

	while(next != NULL) {	
		free(current);
		current = next;
		next = current->next;
	} 

	free(lst->back);

	iter_destroy(lst->end);
	free(lst);
}

void ll_destroy_all
(struct linked_list *lst) {
	struct l_node *current = lst->front;
	struct l_node *next = current->next;
	
	while(next != NULL) {
		free(current->item);
		free(current);
		current = next;
		next = current->next;
	}

	free(lst->end);
	free(lst);
}

Iterator
ll_begin(struct linked_list *ll_p) {
	// need to malloc... 
	Iterator ret = NULL;
	ret = malloc(sizeof(struct iterator));
	ret->container = ll_p;
	// fill in everything. 
	
	// set data to point at the first valid node. 
	ret->data = ll_p->front->next;
	ret->increment = ll_iterator_increment;
	ret->decrement = ll_iterator_decrement;
	ret->deref = ll_iterator_deref;
	ret->cmp = ll_iterator_cmp;

	return ret;
}

// This iterator is made at initialization, 
// since we never change the end iterator 
// except when doing random things like reverse, 
// and whatnot. 
Iterator
ll_end(struct linked_list *ll_p) {
	/* 
	// install more things
	// need to malloc... 
	Iterator ret = NULL;
	ret = malloc(sizeof(struct iterator));
	ret->container = ll_p;
	// fill in everything. 
	
	// set data to point at the first valid node. 
	ret->data = ll_p->back;
	ret->increment = ll_iterator_increment;
	ret->decrement = ll_iterator_decrement;
	ret->deref = ll_iterator_deref;
	ret->cmp = ll_iterator_cmp;
	*/

	return ll_p->end;
}



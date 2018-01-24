#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"



// *******************************
// ITERATORS SECTION 
// *******************************

// functions to increment, decrement, deref, cmp
// might as well define these here so that the list
// init function can use them. 

int list_iterator_increment(Iterator i) {
	
	struct l_node *current = (struct l_node *)i->data;
	i->data = current->next;
	return 0;
}

int list_iterator_decrement(Iterator i) {
	
	struct l_node *current = (struct l_node *)i->data;
	i->data = current->prev;
	return 0;
}

void *
list_iterator_deref
(Iterator i) { 

	struct l_node *current = (struct l_node *)i->data;
	return current->item;
}

int list_iterator_cmp
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

Iterator 
list_end_init(List lst) {

	struct iterator *ret = NULL;
	ret = malloc(sizeof(struct iterator));
	ret->container = lst;
	ret->data = lst->back;
	ret->increment = list_iterator_increment;
	ret->decrement = list_iterator_decrement;
	ret->deref = list_iterator_deref;
	ret->cmp = list_iterator_cmp;

	return ret;

}

List 
list_ctor_empty(void) {
	List ret = NULL;
	ret = malloc(sizeof(struct list));
	
	struct l_node *front = NULL;
	struct l_node *back = NULL;
	l_node_init(&front, NULL);
	l_node_init(&back, NULL);
	
	ret->front = front;
	ret->back = back;
	
	l_node_set_next(front, back);
	l_node_set_prev(back, front);

	ret->size = 0;

	Iterator end = list_end_init(ret);
	ret->end = end;

	return ret;
}

List 
list_ctor_copy(List source) {
	List ret = NULL;
	ret = malloc(sizeof(struct list));
	
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
		i = list_begin(source);
		!iter_cmp(i, list_end(source));
		iter_increment(i)
	) {
		list_push_back(ret, iter_deref(i));
	}
	Iterator end = list_end_init(ret);
	ret->end = end;
	return ret;
}

int list_push_back
(List lst, void *item) {

	struct l_node *to_add = NULL;
	l_node_init(&to_add, item);

	l_node_set_next(to_add, lst->back);
	l_node_set_next(lst->back->prev, to_add);

	l_node_set_prev(to_add, lst->back->prev);
	l_node_set_prev(lst->back, to_add);

	lst->size++;

	return 0;
}

int list_push_front
(List lst, void *item) {

	struct l_node *to_add = NULL;
	l_node_init(&to_add, item);
	
	l_node_set_prev(to_add, lst->front);
	l_node_set_prev(lst->front->next, to_add);

	l_node_set_next(to_add, lst->front->next);
	l_node_set_next(lst->front, to_add);

	lst->size++;
	return 0;
}

// for debugging: 
// the function argument is one that is user supplied - 
// since the data is void pointers, and ll does not 
// know how to print it.
int list_print
(List lst, void (*user_print)(void *)) {

	struct l_node *current = lst->front->next;
	for(int i = 0; i < lst->size; i++) {
		user_print(current->item);
		current = current->next;
		printf("\t");
	}
	printf("\n");
	return 0;
}

void list_destroy
(List lst) {
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

void list_destroy_all
(List lst) {
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
list_begin(List lst) {
	// need to malloc... 
	Iterator ret = NULL;
	ret = malloc(sizeof(struct iterator));
	ret->container = lst;
	// fill in everything. 
	
	// set data to point at the first valid node. 
	ret->data = lst->front->next;
	ret->increment = list_iterator_increment;
	ret->decrement = list_iterator_decrement;
	ret->deref = list_iterator_deref;
	ret->cmp = list_iterator_cmp;

	return ret;
}

// This iterator is made at initialization, 
// since we never change the end iterator 
// except when doing random things like reverse, 
// and whatnot. 
Iterator
list_end(List lst) {
	/* 
	// install more things
	// need to malloc... 
	Iterator ret = NULL;
	ret = malloc(sizeof(struct iterator));
	ret->container = lst;
	// fill in everything. 
	
	// set data to point at the first valid node. 
	ret->data = lst->back;
	ret->increment = list_iterator_increment;
	ret->decrement = list_iterator_decrement;
	ret->deref = list_iterator_deref;
	ret->cmp = list_iterator_cmp;
	*/

	return lst->end;
}



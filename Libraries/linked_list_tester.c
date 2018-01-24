#include <stdio.h>
#include "linked_list.h"

void int_printer(void *p) {
	// cast to an int pointer... 
	int *q = (int *)p;

	printf("%d", *q);
}

void ptr_printer(void *p) {
	printf("%p", p);
}

int main(void) {
	printf("ll tester\n");

	List ll = list_ctor_empty();

	int a[4] = {3, 4, 2, 1};
	for(int i = 0; i < 4; i ++) {
		list_push_front(ll, &a[i]);
	}
	
	list_print(ll, int_printer);
	printf("\n Now with iterators\n");
/*
	Iterator i = list_begin(ll);

	while(!(i->cmp(i, list_end(ll)))) {
		int *val = iter_deref(i);
		printf("%d\t", *val);
		iter_increment(i);
	}
*/
	Iterator i;	

	for(
		i = list_begin(ll);
		iter_neq(i, list_end(ll));
		iter_increment(i)
	) {
		printf("%d\t", *(int *)iter_deref(i));
	}	

	iter_destroy(i);
	i = list_begin(ll);
	iter_increment(i);
	iter_increment(i);

	printf("\n");
	printf("got here 1\n");
	int x = 13;
	(void)x;
	list_insert(ll, i, &x);

	printf("got here\n");	
	iter_destroy(i);

	for(
		i = list_begin(ll);
		iter_neq(i, list_end(ll));
		iter_increment(i)
	) {
		printf("%d\t", *(int *)iter_deref(i));
	}

	list_destroy(ll);

	printf("\n");

	return 0;
}

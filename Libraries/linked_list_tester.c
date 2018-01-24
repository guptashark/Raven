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

	struct linked_list *ll = ll_ctor_empty();

	int a[4] = {3, 4, 2, 1};
	for(int i = 0; i < 4; i ++) {
		ll_push_front(ll, &a[i]);
	}
	
	ll_print(ll, int_printer);
	printf("\n Now with iterators\n");
/*
	Iterator i = ll_begin(ll);

	while(!(i->cmp(i, ll_end(ll)))) {
		int *val = iter_deref(i);
		printf("%d\t", *val);
		iter_increment(i);
	}
*/
	Iterator i;	

	for(
		i = ll_begin(ll);
		!iter_cmp(i, ll_end(ll));
		iter_increment(i)
	) {
		printf("%d\t", *(int *)iter_deref(i));
	}	
	
	iter_destroy(i);

	struct linked_list *l2 = ll_ctor_copy(ll);
	ll_print(l2, int_printer);

	ll_destroy(ll);

	printf("\n");

	return 0;
}

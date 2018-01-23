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

	struct linked_list *ll = ll_init();

	int a[4] = {3, 4, 2, 1};
	for(int i = 0; i < 4; i ++) {
		ll_push_front(ll, &a[i]);
	}
	
	ll_print(ll, int_printer);
	printf("\n Now with iterators\n");

	Iterator i = ll_begin(ll);
	Iterator i_end = ll_end(ll);

	while(!(i->cmp(i, i_end))) {
		int *val = iter_deref(i);
		printf("%d\t", *val);
		iter_increment(i);
	}
	printf("\n");

	return 0;
}

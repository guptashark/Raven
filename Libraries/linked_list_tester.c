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

	struct linked_list *ll = NULL;
	ll_init(&ll);

	int a[4] = {3, 4, 2, 1};
	for(int i = 0; i < 4; i ++) {
		ll_push_front(ll, &a[i]);
	}
	ll_print(ll, int_printer);

	return 0;
}

#include "iterator.h"
#include <stdlib.h>

void iter_increment(Iterator i) {
	i->increment(i);
}

void iter_decrement(Iterator i) {
	i->decrement(i);
}

void *iter_deref(Iterator i) {
	return i->deref(i);
}

int iter_cmp(Iterator i, Iterator j) {
	return i->cmp(i, j);
}

void iter_destroy(Iterator i) {
	free(i);
}

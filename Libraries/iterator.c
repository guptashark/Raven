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

int iter_eq(Iterator i, Iterator j) {
	return (i->cmp(i, j) == 0);
}

int iter_neq(Iterator i, Iterator j) {
	return (i->cmp(i, j) != 0);
}

void iter_destroy(Iterator i) {
	free(i);
}

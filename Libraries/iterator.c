#include "iterator.h"

void iter_increment(Iterator i) {
	i->increment(i);
}

void iter_decrement(Iterator i) {
	i->decrement(i);
}

void *iter_deref(Iterator i) {
	return i->deref(i);
}

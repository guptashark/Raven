#include <stdbool.h>


struct iterator;
typedef struct iterator *Iterator;
// bidirectional iterator
// 
struct iterator {

	// the container we're iterating along. 
	void *container;

	// the data we need to hold to allow 
	// dereferencing. This also helps when 
	// doing checks for if we're at the 
	// iterator end. 
	void *data;

	// to make iterator move along container. 	
	int (*increment)(Iterator self);

	// to make iterator move backwards. 
	int (*decrement)(Iterator self);

	// to get the item... 
	// put the item in the first arg slot. 
	// (our "return" value) 
	void *(*deref)(Iterator);

	// same here, put result of cmp in bool * slot. 
	int (*cmp)(Iterator self, Iterator);

};

// Notes: 

// wether an iterator is bidirectional, forward, etc
// seem to be defined elsewhere - not in the type. 
// otherwise, we'd be stuck with a lot of types... 
// it's better to instead create iter functions
// that act on iterators themselves, which 
// end up calling that internal function to increment, 
// decrement, etc. Doesn't look fully like c++, 
// but it is a lot nicer than something like: 

// Iterator *i = ll_begin(my_list);
// i->increment(i); 

// when instead we can have: 
// iter_increment(i);

void iter_increment(Iterator);

void iter_decrement(Iterator);

void *iter_deref(Iterator);

int iter_cmp(Iterator, Iterator);

int iter_neq(Iterator, Iterator);

int iter_eq(Iterator, Iterator);
void iter_destroy(Iterator);



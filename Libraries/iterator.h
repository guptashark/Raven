#include <stdbool.h>

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
	int (*increment)(struct bidir_iterator *self);

	// to make iterator move backwards. 
	int (*decrement)(struct bidir_iterator *self);

	// to get the item... 
	// put the item in the first arg slot. 
	// (our "return" value) 
	void *(*deref)(struct bidir_iterator *);

	// same here, put result of cmp in bool * slot. 
	int (*cmp)(struct bidir_iterator *self, struct bidir_iterator *);

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

// struct iterator *i = ll_begin(my_list);
// i->increment(i); 

// when instead we can have: 
// iter_increment(i);

int iter_increment(struct iterator *);

int iter_decrement(struct iterator *);

void *iter_deref(struct iterator *);



#include <stdbool.h>

// bidirectional iterator
// 
struct bidir_iterator {

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

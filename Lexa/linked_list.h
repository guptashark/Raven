

// Needs a custom type. We do void pointers. 
// Does not need a custom allocator. 
// can implement later, if need be, when we implement 
// our own memory management... 

struct ll_node {
	
	void *item;
	struct ll_node *next;

};

struct linked_list {
	int size;
	struct ll_node *front;

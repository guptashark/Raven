#include <stdio.h>
#include <stdlib.h>

// We need several components: 
/*
	* A structure to hold the data to be trained on. 
	* A way to generate the data. (our function f) 
	
	Unsure for next.... 
	*  clearly need to implement Linear Programming... 
	* 
*/

/* Lets start with the structure to hold the data. 
 * It's going to be a sequence, (our training set). 
 * The sequence can be a linked list. 
 * In each spot in the array, we have a pair: 
 	* One value from X, the other value from Y, (label). 
	* The value from X realistically can be an array type
	* There should be some way of knowing the dimension, 
	* but that can be addressed later, this is only 
	* a prototype. 
	* For now, we can assume that X is R^2, so all
	* x's can either be arrays with 2 values, or 
	* structs with an x_1 and x_2 member. I like 
	* the second. 
*/


struct domain_point {
	// may not want to leave them as ints... 
	// we'll see if it's compatible with floats. 
	// ... later
	int x_1;
	int x_2;
};

int 
domain_point_init
(struct domain_point **dp_dp, int x_1, int x_2) {
	struct domain_point *ret = malloc(sizeof(struct domain_point));
	ret->x_1 = x_1;
	ret->x_2 = x_2;

	*dp_dp = ret;

	// might do error checking whenever. 
	return 0;
}



int domain_point_print
(struct domain_point *dp_p) {
	printf("(%d, %d)", dp_p->x_1, dp_p->x_2);
	return 0;
}

struct labeled_point {
	// the x value
	struct domain_point *dp;

	// the y value
	int label;
};

int 
labeled_point_init
(struct labeled_point **lp_dp, 
struct domain_point *dp_p, int label) {
	struct labeled_point *ret = NULL;
	ret = malloc(sizeof(struct labeled_point));
	ret->dp = dp_p;
	ret->label = label;

	*lp_dp = ret;
	return 0;
}

int labeled_point_print
(struct labeled_point *lp_p) {
	
	printf("(");	
	domain_point_print(lp_p->dp);
	printf(", %d)\n", lp_p->label);
	return 0;
}

struct training_set {
	int num_points;

	// an array of labeled point ptrs. 
	struct labeled_point **first;
};


int main(void) {

	// initialize random state for generator
	srand(0);


	struct domain_point *d1 = NULL;
	domain_point_init(&d1, 4, 5);
	struct labeled_point *l1 = NULL;
	labeled_point_init(&l1, d1, 1);
	printf("Done.\n");

	labeled_point_print(l1);

}

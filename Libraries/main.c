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

/* data generator
	randomly generates domain point
	determines label for it (using f)
	produces the labeled point.
*/
struct data_gen {
	// labeling function
	// f takes in a domain point, outputs a label. 
	int (*label_fn)(struct domain_point *);
};

int data_gen_init
(struct data_gen *dg_p, int (*label_fn)(struct domain_point *)) {
	dg_p->label_fn = label_fn;
	return 0;
}
	 
int data_gen_generate
(struct data_gen *dg_p, struct labeled_point **lp_dp) {
	int x_1 = (rand() % 20) - 10;
	int x_2 = (rand() % 20) - 10;
	struct domain_point *dp = NULL;
	domain_point_init(&dp, x_1, x_2);
	int label = dg_p->label_fn(dp);
	
	labeled_point_init(lp_dp, dp, label);
	return 0;
}

int label_fn_01(struct domain_point *dp) {
	int val = dp->x_1 * 3 + dp->x_2 * 5 - 7;
	if(val > 0) {
		return 1;
	} else {
		return -1;
	}
}

// this is where it all comes together - 
// we set the number of samples we want, 
// and generate them, since the label
// function is already set in the data_gen. 
struct training_set {
	// number of pairs of labeled points. 
	// also maybe understood as length of sequence.
	int num_pairs;

	// an array of labeled point ptrs. 
	struct labeled_point **sequence;
};

int training_set_init
(struct training_set *ts, struct data_gen *dg_p, int num_pairs) {


	ts->num_pairs = num_pairs;

	struct labeled_point **arr = NULL;
	arr = malloc(sizeof(struct labeled_point *) * num_pairs);

	for(int i = 0; i < num_pairs; i++) {
		data_gen_generate(dg_p, &(arr[i]));
	}

	ts->sequence = arr;
	return 0;
}

int training_set_print
(struct training_set *ts) {
	printf("Num Pairs: %d\n", ts->num_pairs);
	for(int i = 0; i < ts->num_pairs; i++) {
		labeled_point_print(ts->sequence[i]);
	}

	return 0;
}



int main(void) {

	// initialize random state for generator
	srand(0);

	// initialize data generator
	struct data_gen dg;
	struct data_gen *dg_p = &dg;
	data_gen_init(dg_p, label_fn_01);

	struct training_set ts;
	struct training_set *ts_p = &ts;
	training_set_init(ts_p, dg_p, 10);

	training_set_print(ts_p);
		
	
	return 0;
		
}

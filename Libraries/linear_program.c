#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

struct lp_constraint {

	struct linked_list *coeffs;
	char relation[8];
	float rhs;
};


int
lp_constraint_init(
	struct lp_constraint **lpc_p, 
	int coeffs_len, 
	float *coeffs,
	char *relation,
	float rhs
) {
	struct lp_constraint *ret = NULL;
	ret = malloc(sizeof(struct lp_constraint));

	ret->coeffs = ll_ctor_empty();
	// not the best soln... 
	// but lets malloc each float... 
	// and push the pointer to it in the linked_list

	for(int i = 0; i < coeffs_len; i++) {
		float *to_add = NULL;
		to_add = malloc(sizeof(float));
		*to_add = coeffs[i];
		ll_push_back(ret->coeffs, to_add);
	}
	
	strcpy(ret->relation, relation);
	ret->rhs = rhs;

	*lpc_p = ret;
	return 0;
}

int
lp_constraint_print(struct lp_constraint *lpc_p) {
	Iterator i;
	Iterator i_end;

	i = ll_begin(lpc_p->coeffs);
	i_end = ll_end(lpc_p->coeffs);

	while(!(i->cmp(i, i_end))) {
		float *f = i->deref(i);
		printf("%.2f\t", *f);

		i->increment(i);
	}

	// now print the relation operator
	printf("%s\t", lpc_p->relation);
	// now print the rhs
	printf("%.2f", lpc_p->rhs);

	return 0;
}

struct linear_program {
	// all components of the objective fn. 
	struct linked_list *c;
	float obj_bias;
	char optimize_for[8];

	// the constraints
	struct linked_list *constraints;

	// variable constraints (bigger than 0, less, etc); 
	// disclaimer: 
	// The main way this works is that the variable
	// constraint is a very simple one: free, >=, <= .
	// otherwise, we've got a problem. If you want to 
	// codify something more complicated, you have
	// to add it as a general constraint. 
	// rule - which is not hard. 
	
	struct linked_list *var_constraints;	
};

// initialize a linear program: 
// 		need to hold constraints... 
//			equation lhs
//			relation
//			rhs
// 		objective function. 
//			c vector
//			bias
//			max or min. 
// 		list of variables and relation to zero... 
//			automatically have names for the variables as: 
//			x1, x2, x3, etc. 
//			which turn into x1+, x1-, etc. as needed. 
// 			(because this means we can solve the sef_lp, 
//			and get a result in the LP itself.

// init the linear program, that's it. 
int lp_init
(struct linear_program **lp_dp) {
	
	struct linear_program *ret = NULL;
	ret = malloc(sizeof(struct linear_program));
	ret->c = ll_ctor_empty();
	ret->constraints = ll_ctor_empty();
	ret->var_constraints = ll_ctor_empty();
	*lp_dp = ret;
	return 0;
}

// add in one equation: 
// requires lhs, relation and rhs. 
// equations are stored in the order they are put in. 
// (0, 0, 3, 4) <=, 5, means: 3*x3 + 4*x4 <= 5 is the constraint. 
int lp_add_constraint(	
	struct linear_program *lp_p, 
	int arr_len, 
	float *coeffs, 
	char *relation,
	float rhs) {


	struct lp_constraint *to_add= NULL;
	lp_constraint_init(&to_add, arr_len, coeffs, relation, rhs);

	ll_push_back(lp_p->constraints, to_add);
	return 0;
}

// add in the objuctive fonctoin
// requires the lhs
// comparison
// rhs
// wether we're maxing or minimizing.
int lp_set_obj_fn
(struct linear_program *lp_p, 
int w_len,
float *c,
float bias,
char *optimize_for) {
	
	// sigh... need to malloc all these floast
	// grr	

	for(int i = 0; i < w_len; i++) {
		float *to_add;
		to_add = malloc(sizeof(float));
		*to_add = c[i];
		ll_push_back(lp_p->c, to_add);
	}

	// strcpy the max or min
	strcpy(lp_p->optimize_for, optimize_for);

	// copy the bias
	lp_p->obj_bias = bias;
	// and we're done. Phew. 
	return 0;
}

int lp_invert_obj_fn
(struct linear_program *lp_p) {

	Iterator i = ll_begin(lp_p->c);
	Iterator i_end = ll_end(lp_p->c);
	printf("%p, %p \n", i->data, i_end->data);

	printf("A\n");
	while(!(iter_cmp(i, i_end))) {
		printf("IN LOOP\n");
		float *f = iter_deref(i); 
		*f = *f * -1;
		printf("%f yeaa ", *f);

		iter_increment(i);
	}

	printf("B\n");	
	int result = strcmp(lp_p->optimize_for, "max");
	if(result) {
		strcpy(lp_p->optimize_for, "max");
	} else {
		strcpy(lp_p->optimize_for, "min");
	}

	return 0;

}

// TODO STILL need to add in
// the part of the linear prog that
// records which vars have non neg constraints, et.c 

// long function name, but pretty descriptive, no? 
// (rel_to_zero means relation to zero, free, >= or <=.
int lp_add_variable_constraint
(struct linear_program *lp_p, char *rel_to_zero ) {
	// malloc an 8 byte string, 
	// add it onto the list.
	char *to_add = NULL;
	to_add = malloc(sizeof(char) * 8);
	
	strcpy(to_add, rel_to_zero);
	ll_push_back(lp_p->var_constraints, to_add);
	return 0;
}


int lp_print
(struct linear_program *lp_p) {

	// print the objective fn
	Iterator i = ll_begin(lp_p->c);
	Iterator i_end = ll_end(lp_p->c);

	printf("%s (\t", lp_p->optimize_for);
	while(!(i->cmp(i, i_end))) {
		float *val = iter_deref(i);
		printf("%.2f\t ", *val);
		iter_increment(i);
	}

	printf(") \t+\t%.2f\n\n", lp_p->obj_bias);

	// now to print the constraints;
	// to do this, we need to destroy the iterators. 	

	// TODO
	// is this okay?? To just call free?? 
	free(i);
	// The culprit - can't free this lol

	i = ll_begin(lp_p->constraints);
	i_end = ll_end(lp_p->constraints);

	while(!(i->cmp(i, i_end))) {
		struct lp_constraint *r = iter_deref(i);
		lp_constraint_print(r);
		printf("\n");
		iter_increment(i);
	}

	// Now add in the variable constraints. 

	free(i);
	
	i = ll_begin(lp_p->var_constraints);
	i_end = ll_end(lp_p->var_constraints);
	
	int j = 1;

	while(!(i->cmp(i, i_end))) { 
		char *s = iter_deref(i);
		// just print right now, 
		// later on we can pretty print. 
		int result = strcmp(s, "free");
		if(result) {

			printf("x_%d %s 0, ", j, s);
		} else {
			printf("x_%d free, ", j);
		}
		
		iter_increment(i);
		j++;

	}

	printf("\n");
	return 0;
}



// standard equality form linear program
// realistically, only linear program
// should be making this. 
struct sef_lp {
	
};


int main(void) {

	// we can do this with compund literals... 
	// but lets just do it with named arrays	
	struct linear_program *lp;
	lp_init(&lp);

	float c[3] = {1, 2, 1};
	float coeff_1[2] = {3, 4};
	float coeff_2[2] = {6, 7};
	float coeff_3[2] = {2, 1};

	lp_set_obj_fn(lp, 3, c, 9, "max");
	lp_add_constraint(lp, 2, coeff_1, "<=", 5);
	lp_add_constraint(lp, 2, coeff_2, ">=", 9);
	lp_add_constraint(lp, 2, coeff_3, "==", 3);

	lp_add_variable_constraint(lp, ">=");
	lp_add_variable_constraint(lp, "<=");
	lp_add_variable_constraint(lp, "free");
//	lp_add_variable_constraint(lp, ">=");
	

	lp_print(lp);
	printf("\n");

	lp_invert_obj_fn(lp);
	lp_print(lp);
	return 0;
	
}

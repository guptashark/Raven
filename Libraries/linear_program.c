#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

struct lp_constraint {

	List coeffs;
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

	ret->coeffs = list_ctor_empty();
	// not the best soln... 
	// but lets malloc each float... 
	// and push the pointer to it in the linked_list

	for(int i = 0; i < coeffs_len; i++) {
		float *to_add = NULL;
		to_add = malloc(sizeof(float));
		*to_add = coeffs[i];
		list_push_back(ret->coeffs, to_add);
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

	i = list_begin(lpc_p->coeffs);
	i_end = list_end(lpc_p->coeffs);

	while(iter_neq(i, i_end)) {
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


// Main problem: 
// We need to record which variables are where, 
// in terms of variable names. How do we keep 
// track? Especially with sorting and such, 
// moving columns around, etc. 

// 2 solutions: 
//	* add the variable name to var_constraints
//	* make a new list for the var constraints. 
// 
// Second way feels best because then when we
// pass it on to the standard equality form 
// it won't have the extra useless data from 
// var_constraints. Also, when sef meses up the 
// columns, we can fix them because we'll know 
// of the original order. 

// It's getting common enough that we should
// implement an ascii string library, 
// similar to the c++ string library. 
struct linear_program {
	// all components of the objective fn. 
	List c;
	float obj_bias;
	char optimize_for[8];

	// the constraints
	List constraints;

	// variable constraints (bigger than 0, less, etc); 
	// disclaimer: 
	// The main way this works is that the variable
	// constraint is a very simple one: free, >=, <= .
	// otherwise, we've got a problem. If you want to 
	// codify something more complicated, you have
	// to add it as a general constraint 
	// - which is not hard. 
	
	List var_constraints;
	List var_names;
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
	ret->c = list_ctor_empty();
	ret->constraints = list_ctor_empty();
	ret->var_constraints = list_ctor_empty();
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

	list_push_back(lp_p->constraints, to_add);
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
		list_push_back(lp_p->c, to_add);
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

	Iterator i = list_begin(lp_p->c);
	Iterator i_end = list_end(lp_p->c);

	while(iter_neq(i, i_end)) {
		float *f = iter_deref(i); 
		*f = *f * -1;

		iter_increment(i);
	}

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
	list_push_back(lp_p->var_constraints, to_add);
	return 0;
}


// Need to be able to convert a constraint
// that is an inequality into an equality
// by adding in the corresponding slack 
// variable. 

int lp_add_slack_vars(struct linear_program *lp) {

	Iterator i;
	for(
		i = list_begin(lp->constraints);
		iter_neq(i, list_end(lp->constraints));
		iter_increment(i)
	) {
		struct lp_constraint *curr= iter_deref(i);
		if(strcmp(curr->relation, "==") != 0) {
			float to_set = 0;
			if(strcmp(curr->relation, ">=") == 0) {
				to_set = -1;
			} else {
				to_set = 1;
			}

			// if the constraint is >= 0... 
			// append a zero to all the constraints - 
			// except this one. 
			// also, push a zero in the objective fn. 
			Iterator j;
			for(
				j = list_begin(lp->constraints);
				iter_neq(j, list_end(lp->constraints));
				iter_increment(j)
			) {
				struct lp_constraint *to_extend = iter_deref(j);
				float *to_add = malloc(sizeof(float));

				// set correct val to push back
				if(iter_eq(i, j)) {
					*to_add = to_set;
				} else {
					*to_add = 0;
				}

				list_push_back(to_extend->coeffs, to_add);
			}
			
			// set the new relation
			strcpy(curr->relation, "==");
			
			// push back on the obj fn to make room 
			// for the new value. 
			float *obj_to_add = malloc(sizeof(float));
			*obj_to_add = 0;
			list_push_back(lp->c, obj_to_add);
		}
	}

	return 0;
}

// fix problems like: x2 <= 0 
// and things like: x5 is free

int lp_regularize_vars(struct linear_program *lp) {

	// first make the list of iterators. one for each 
	// constraint, so we can iterate them all at the 
	// same time. 

	List listof_iters = list_ctor_empty();
	// put in the iterator from the obj function: 
	Iterator obj_iter = list_begin(lp->c);
	list_push_back(listof_iters, obj_iter);

	Iterator i;

	for(
		i = list_begin(lp->constraints);
		iter_neq(i, list_end(lp->constraints));
		iter_increment(i)
	) {

		struct lp_constraint *current = iter_deref(i);

		Iterator j = list_begin(current->coeffs);
		list_push_back(listof_iters, j);

	}
	iter_destroy(i);

	// now that we have the list of iters of constraints
	// for every single constraint... lol 

	// iterator along the variables
	Iterator k;
	
	for(k = list_begin(lp->var_constraints); 
		iter_neq(k, list_end(lp->var_constraints)); 
		iter_increment(k))
	{
		// we don't yet have the insert function... 
		char *s = iter_deref(k);
		bool less_than_zero = (strcmp(s, "<=") == 0);
		bool is_free = (strcmp(s, "free") == 0);
		(void)is_free;	
			
		for(
			i = list_begin(listof_iters);
			iter_neq(i, list_end(listof_iters));
			iter_increment(i)
		) {

			Iterator ptr = iter_deref(i);	

			if(less_than_zero) {

				float *val = iter_deref(ptr);
				*val = *val * -1;
			} else if(is_free) {
				float *val = iter_deref(ptr);
				float *to_insert = malloc(sizeof(float));
				*to_insert = *val;
				*val = *val * -1;
				list_insert(ptr->container, ptr, to_insert);
			}
				
			iter_increment(ptr);
		}

		strcpy(s, ">=");

	}

		
				
	return 0;
}

int lp_print
(struct linear_program *lp_p) {

	// print the objective fn
	Iterator i = list_begin(lp_p->c);
	Iterator i_end = list_end(lp_p->c);

	printf("%s (\t", lp_p->optimize_for);
	while(iter_neq(i, i_end)) {
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

	i = list_begin(lp_p->constraints);
	i_end = list_end(lp_p->constraints);

	while(iter_neq(i, i_end)) {
		struct lp_constraint *r = iter_deref(i);
		lp_constraint_print(r);
		printf("\n");
		iter_increment(i);
	}

	// Now add in the variable constraints. 

	free(i);
	
	i = list_begin(lp_p->var_constraints);
	i_end = list_end(lp_p->var_constraints);
	
	int j = 1;

	while(iter_neq(i, i_end)) { 
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


int main(void) {

	// Need a bit of a procedure for initializing
	// linear programs: Must always write the 
	// objective function first. 
	// then all the constraints. 
	// then convert to a SEF linearprog. 
	
		

	// we can do this with compund literals... 
	// but lets just do it with named arrays	
	struct linear_program *lp;
	lp_init(&lp);

	float c[3] = {-1, 2, -4};
	float coeff_1[3] = {1, 5, 3};
	float coeff_2[3] = {2, -1, 2};
	float coeff_3[3] = {1, 2, -1};

	lp_set_obj_fn(lp, 3, c, 9, "min");
	lp_add_constraint(lp, 3, coeff_1, ">=", 5);
	lp_add_constraint(lp, 3, coeff_2, "<=", 4);
	lp_add_constraint(lp, 3, coeff_3, "==", 2);

	lp_add_variable_constraint(lp, ">=");
	lp_add_variable_constraint(lp, ">=");
	lp_add_variable_constraint(lp, "free");
//	lp_add_variable_constraint(lp, ">=");
	

	lp_print(lp);
	printf("\n");

	lp_invert_obj_fn(lp);
	lp_regularize_vars(lp);
	lp_add_slack_vars(lp);
	lp_print(lp);
	return 0;
	
}

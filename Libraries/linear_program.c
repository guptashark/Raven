#include <stdio.h>
#include <stdlib.h>

// 
struct linear_program {
	struct linked_list *c;
	// dont' know what really goes in here yet. 
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
	//stuff 
}

// add in one equation: 
// requires lhs, relation and rhs. 
// equations are stored in the order they are put in. 
// (0, 0, 3, 4) <=, 5, means: 3*x3 + 4*x4 <= 5 is the constraint. 
int lp_add_constraint
(struct linear_program *lp_p, 
int arr_len, 
float *coeffs, 
char *relation,
float *rhs) {
		

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

	// stuff
}



// standard equality form linear program
// realistically, only linear program
// should be making this. 
struct sef_lp {
	
};


int main(void) {

	printf("Linear Program\n");
}

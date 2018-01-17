#include <stdio.h>
#include <stdlib.h>

// several steps for a linear prog... 
// the linear prog itself, in any sort of form (all acceptable) 
// the linear prog in standard form, for computing with simplex. 

// May need matrices... and row operations... 

// might as well restrict the matrix for use in this file by namespacing it. c:w

struct LP_matrix { 

	int rows; 
	int cols;
	float **elements;
};

int matrix_init 

struct affine_fn {

	// an element of R^n
	int dimension;

	// the actual vector itself. 
	float *w;

	// the bias. 
	float b;

	////////////////////////////
};

int affine_fn_init
(struct affine_fn **fn_dp, int dimension, float *w, float bias) {

	struct affine_fn *fn_p = malloc(sizeof(struct affine_fn));


	return 0;

}



struct linear_program {
// We need an affine function 
// 

};

// init fn for linear program
int linear_program_init(struct linear_program **lp_dp);

// create this from a linear program. 
struct standard_LP {

};


// init function for standard LP. 
int standard_LP_init(struct standard_LP **slp_dp);

int main(void) {

	return 0;
}

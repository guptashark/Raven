#include <stdio.h>
#include <stdlib.h>

// several steps for a linear prog... 
// the linear prog itself, in any sort of form (all acceptable) 
// the linear prog in standard form, for computing with simplex. 

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


int main(void) {

	return 0;
}

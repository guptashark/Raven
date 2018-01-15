#include <stdio.h>

// We're going to start with the form in which 
// the simplex algo can work - standard form. 
// we'll have a class for linear programs in general, 
// and then when that linear program is in standard form, 
// it can generate a new LP called StandardLP that gets
// rid of the fluff and focuses on the algo itself. 
// We'll start by assuming we're in Standard form. 

struct standard_LP {

	// the objective function
	// an array of floats, with length. 
	int c_len;
	float *c;

	// the matrix A
	int A_num_rows;
	int A_num_cols;
	float **A;

	// the b vector. 
	int b_len;
	float *b;
};

int main(void) {

	printf("Linear Programming\n");
	return 0;
}

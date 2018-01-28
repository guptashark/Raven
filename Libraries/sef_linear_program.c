

struct sef_linear_program {
	// obj fn, row or col vector?? 

	// matrix A

	// vector b, // row or col vector?? 

};

int sef_lp_simplex(struct sef_linear_prog) {

}
// find the first value of the obj function that is > 0 so that 
// we can use it. 

// get the corresponding index i, and raise x_i to the highest possible
// value to match it. 

// how big can we make x_i? 
// if all of the values are negative, we say that the linear program 
// is then unbounded. 

// we need canonical form... 

// for this point of the iteration, assume that a linear prog is 
// in canonical form. That is: 
// A_B = I
// c_B = 0. 

// we should keep a list of ints that are the indicies to our basis. 
// or maybe some other data structure. Maybe an array, because at 
// any given time, we know the dimensions of the matrix, and hence, 
// the size of the basis. 



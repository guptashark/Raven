

struct r_class {
	/* 
	 * Member variables
	 * 	type, name, visibility (public, private, protected, etc)
	 * 	-store as dict with name of var as key (because it's unique) and typeinfo as val. 
	 *
	 * Member Functions
	 * 	Since C++ can differentiate based on types... 
	 * 	Generated function signatures, etc.
	 * 	-store as dict key == fn_signature, val == function pointer, data, etc.  
	 */
};

/* instantiate the class. The object should have some way to access its data fields... methods... etc. */

struct class_instantiation {

	/* What my type is... pointer to the class this object is a member of. */ 

	/* Dict of member variables and types, etc. */
	/* probably have a dictionary that can be filled with data... about the object itself. 
	 * We'll have to make a copy of the existing data... 
	 */

};	





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

/* Required functions: 
 *
 * create_class(class_name)	
 * 	registers the class in a global set of class defs. 
 * 
 * add_member_var(class_name, attributes, type, etc)	
 * 	Adds the specified variabel to an existing class. 
 *
 * TODO: Since these are methods... something special has to happen here
 * add_member_fn(class_name, fn_name, fn_attributes, etc)
 * 	Adds the specified function to an existing class. ((HOWW??))
 *
 * finalizes_class(class_name)
 * 	Indicates that the class definition is finished, and should now not be modified. 
 *
 * instantiate(fresh_ptr, class_name)
 * 	// don't see a real way to create member functions with all the stuff we want. 	
 *	// unless we decide to implement them as constructs in the new language
 * 	
 */

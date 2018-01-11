#ifndef __EPSILON_NFA_H__
#define __EPSILON_NFA_H__

/* We need epsilon NFA's
for ease of conversion from 
regex to automaton. From an eNFA, 
we can either simulate it, or construct
an NFA, or DFA. Either way, we need to
implement this as a base liason between
our final automata and the higher level
regex specifications. */

struct epsilon_nfa_state {
	// States we can transfer to on an epsilon
	// transition. 
	// Best case scenario is a linked list. 
	// insertion is super easy, and we need
	// to traverse the list anyways. 

	// States we can get to from each 
	// possible character. 
	// implement as an array of linked list ptrs. 

	// That way, from the above, we get all the 
	// possible transitions. 

	// We need to implement a decent linked list. 
	
};

struct language {
	// start state
	// final states
	// transitions encoded in nfa states

};

// The start of a language itself, basing off of a single char. 
int language_base
(struct language *product, char c);

// The union of two languages
// should output a new language: 
int language_union
(struct language *product, struct language *a, struct language *b);

// The concatenation of two langs
int language_concatenation
(struct language *product, struct language *a, struct language *b);

// The kleen star/closure of a lang
int language_kleen
(struct language *product, struct language *a);

#endif

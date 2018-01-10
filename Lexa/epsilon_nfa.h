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

};

struct language {

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

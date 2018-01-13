#include <set>
#include <iostream> 
#include <list> 
#include <string>

using namespace std;

class eNFA_state {

	private: 
		// all the epsilon transitions
		list<eNFA_state *> epsilon_transits;

		// all the character transitions
		list<eNFA_state *> symbol_transits[256];

	public: 
		// constructor is default. 

		// add a character transition
		int add_transit(unsigned char c, eNFA_state *destination) {
			symbol_transits[c].push_back(destination);
			// can potentially return something else in case of errs. 
			return 0;
		};

		int add_e_transit(eNFA_state *destination) {
			epsilon_transits.push_back(destination);
			return 0;
		};

		// "step" is to step on the char c in the 
		// automaton, for simulation. 
		list<eNFA_state *> *step(unsigned char c) {
			return &(symbol_transits[c]);
		};

		list<eNFA_state *> *eps_step() {
			return &epsilon_transits;
		};

		set<eNFA_state *> compute_eps_closure() {
			set<eNFA_state *> ret;

			// avoid excess computation. Also good base case. 
			if(epsilon_transits.size() == 0) {
				return ret;
			}

			set<eNFA_state *>::iterator i;
			list<eNFA_state *>::iterator j;

			for(j = epsilon_transits.begin(); j!= epsilon_transits.end(); j++) {
				set<eNFA_state *> s = (*j)->compute_eps_closure();
				for(i = s.begin(); i != s.end(); i++) {
					ret.insert(*i);
				}
			}

			return ret;
		}

		void single_print() {
			cout << "num eps transitions: " << epsilon_transits.size() << endl;
			
			for(int c = 0; c < 256; c++) {

				if(symbol_transits[c].size() > 0) {
					cout << (char)c << symbol_transits[c].size() << endl;
				}
			}
		}

		void pretty_print() {
			cout << "num eps_transits: " << epsilon_transits.size() << endl;
			list<eNFA_state *>::iterator i;
			for(i = epsilon_transits.begin(); i != epsilon_transits.end(); i++) {
				(*i)->pretty_print();
			}

			for(int c = 0; c < 256; c++) {

				if(symbol_transits[c].size() > 0) {
					cout << (char)c << symbol_transits[c].size() << endl;
				}
			}

			for(int c = 0; c < 256; c++) {
				if(symbol_transits[c].size() > 0) {
					for(i = symbol_transits[c].begin(); i != symbol_transits[c].end(); i++) {
						(*i)->pretty_print();
					}
				}
			}	
		};
};


class eNFA_Language {
	
	private: 
		// start state
		eNFA_state *start;

		// set of acceptor states
		set<eNFA_state *> accept;

		// since we seem to be making set unions so often, 
		// this is helpful: 
		void make_union(set<eNFA_state *> *s, set<eNFA_state *> *t) {
			set<eNFA_state *>::iterator i; 
			for(i = t->begin(); i != t->end(); i++) {
				s->insert(*i);
			}
		}

		// since we seem to have lots of places where we add 
		// everything in a list to a set. 
		void transfer_all(set<eNFA_state *> *s, list<eNFA_state *> *l) {
			list<eNFA_state *>::iterator i;
			for(i = l->begin(); i != l->end(); i++) {
				s->insert(*i);
			}
		}
	
	// functional style creation of Langs. 
	// Always produce a new eNFA_langauge obj, 
	// leave the two previous ones as is. Later
	// implementations may delete "consumed" langs. 
	// 	- create from single char. 
	//	- create from concat of two langs
	// 	- create from Union of two langs
	// 	- create from kleene of another lang. 


	public: 
		eNFA_Language(char c) {
			start = new eNFA_state();
			eNFA_state *acceptor = new eNFA_state();
			start->add_transit(c, acceptor);
			accept.insert(acceptor);
		};

		eNFA_Language(eNFA_Language *a, eNFA_Language *b, string operation) {
			if(operation == "concatenate") {
				// take start state of "a", set as our start state. 
				start = a->start;

				// link together all finals of "a" to the start state 
				// of "b". 

				set<eNFA_state *>::iterator i;
				for(i = a->accept.begin(); i != a->accept.end(); i++) {
					(*i)->add_e_transit(b->start);
				}
				// copy over the b acceptors. 
				accept = b->accept;
			} else if(operation == "union") {
				
				// create a new state, add eps transitions
				// to each of the start states in the other langs. 
				start = new eNFA_state();
				start->add_e_transit(a->start);
				start->add_e_transit(b->start);
				
				// append accepter state lists, this is our list
				// of final states. 
				set<eNFA_state *>::iterator i;
				for(i = a->accept.begin(); i != a->accept.end(); i++) {
					accept.insert(*i);
				}

				for(i = b->accept.begin(); i != b->accept.end(); i++) {
					accept.insert(*i);
				}

			} else {
				cout << "Unrecognized operation." << endl;
			}
		};

		// right now, this can only ever be the kleene operator
		eNFA_Language(eNFA_Language *a) {

			start = new eNFA_state();
			start->add_e_transit(a->start);
		
			eNFA_state *new_acceptor = new eNFA_state();	
			set<eNFA_state *>::iterator i;

			for(i = a->accept.begin(); i != a->accept.end(); i++) {
				(*i)->add_e_transit(new_acceptor);
			}
		
			start->add_e_transit(new_acceptor);
			new_acceptor->add_e_transit(start);
			accept.insert(new_acceptor);
		};
			
		void pretty_print(void) {
			start->pretty_print();
			cout << endl;
		};


		// test to see if a string is in this language. 

			// consider that we have a set of states we are currently in. 
			// Before we make a step on a character, we need the eps closure. 
			// Construct a new set for all the states we could be in: 
			
			// computationally hard... 
			// easy solution!! 
			// 1) get the eps_transits for just those in the current state, creating a union of those sets. 
			// 2) add each element in the union to current. If an element from the union doesn't exist in current, set a flag. 
			// 3) go back to step 1 if the flag is set. If it isn't then continue! 

			// 4) Now we're in all the states we could possibly be in. 
			// 5) add in all the transitions for the next char c. 
	
		bool contains_string(string s) {
			(void)s;
			// follow every possible path. 
			// don't care about efficiency in copy, 
			// just make it work. 

			// set of current states: 
			set<eNFA_state *> current;
			current.insert(start);

			string::iterator s_i;
			for(s_i = s.begin(); s_i != s.end(); s_i++) {
				
				unsigned char c = *s_i;

				bool incomplete = true;
				while(incomplete) {
					set<eNFA_state *> eps_close;
					set<eNFA_state *>::iterator i;
					for(i = current.begin(); i != current.end(); i++) {
						list<eNFA_state *> *close = (*i)->eps_step();
						
						transfer_all(&eps_close, close);
					}
					incomplete = false;
					for(i = eps_close.begin(); i != eps_close.end(); i++) {
						if(current.find(*i) == current.end()) {
							current.insert(*i);
							incomplete = true;
						}
					}
				}

			// now, current is good. Now we can take a step on a char. 
			/*
			set<eNFA_state *>::iterator i;
			for(i = current.begin(); i != current.end(); i++) {
				(*i)->single_print();
			}
			return true;
			*/

		// now try to take the first char step. 
				set<eNFA_state *> next_states;
				set<eNFA_state *>::iterator i;
				for(i = current.begin(); i != current.end(); i++) {
					list<eNFA_state *> *next = (*i)->step(c);
					transfer_all(&next_states, next);
				}

				if(next_states.size() == 0) {
					
					cout << "Fell off automaton" << endl;
					return false;
				} else {
					cout << "processed \"" << c << "\""<< endl;
					cout << next_states.size() << " is the number of immediate next states." << endl;
				}
				// set "current" to next_states. 
				current = next_states;
			}

			// if we're here, then we've exhausted the string. 
			// check to see if we are in an accepting state. 
			// but before that, we need to add in eps closure
			// of the states in current into current. 
			// time for some copypasta. 

			
			bool incomplete = true;
			while(incomplete) {
				set<eNFA_state *> eps_close;
				set<eNFA_state *>::iterator i;
				for(i = current.begin(); i != current.end(); i++) {
					list<eNFA_state *> *close = (*i)->eps_step();
					
					transfer_all(&eps_close, close);
				}
				incomplete = false;
				for(i = eps_close.begin(); i != eps_close.end(); i++) {
					if(current.find(*i) == current.end()) {
						current.insert(*i);
						incomplete = true;
					}
				}
			}

			set<eNFA_state *>::iterator set_it;
			for(set_it = current.begin(); set_it != current.end(); set_it++) {
				if(accept.find(*set_it) != accept.end()) {
					cout << "Accepting." << endl;
					return true; 
				}
			}
			
			// we're not in any accept states. 
			cout << "Rejecting." << endl;
			return false;
		}
};		


class LexaRegex {
	private: 
		eNFA_Language *automaton;

	// Lets start with super easy regex definitions 
	// for langs where the alphabet is made of 2 letters. 

	// regex with only one letter: 
	// "a", "b". 
	// we create the langauge with the eNFA_Language(char) ctor. 

	// regex with word of length 2. 
	// "aa", "ab", "ba", "bb"
	// we get the first letter, turn into a language with Lang(char). 
	// notice that the next character is a letter, turn that into a 
	// language with Lang(char). Join the two with concatenation. 
	
	// regex of lang with only one word of finite length. 
	// "abbaa", "aaaba", "bbbbbbbbbbbba", etc. 
	// follow as above, create and stitch together languages as you go. 
	// you need some way to attach the lang you're holding, and the 
	// way to do it is to find what the next lang is. 

	// regex of union of two langs. 
	// "aab|abb" ... we need brackets... right? 
	// "(aab)|(abb)" ... brackets help signify that this lang needs to be held. 

	// we can create a tree structure to build the regex. 
	// use brackets - if there is a bracket, you know you're going to be 
	// getting arguments to that language. 

	public: 
		// for now, assume that s is only one character long. 
		// adn that the character is either 'a' or 'b'
		LexaRegex(string s) {

			if(s.size() == 1) {
				automaton = new eNFA_Language(s[0]);
			} else {

				// assume size is bigger than one, but lang still
				// has just one word. 
				string::iterator i = s.begin();
				eNFA_Language *tmp = new eNFA_Language(*i);
				i++;
				// might want an empty state
				for(;i != s.end(); i++) {
					eNFA_Language *next = new eNFA_Language(*i);
					eNFA_Language *my_tmp = new eNFA_Language(tmp, next, "concatenate");
					tmp = my_tmp;
				}

				automaton = tmp;
			}

		}

		bool match(string s) {
			return automaton->contains_string(s);
		}




	
};



int main(void) {
/*	
	{
		eNFA_Language lang_a = eNFA_Language('a');
		eNFA_Language lang_b = eNFA_Language('b');

		eNFA_Language lang_ab = eNFA_Language(&lang_a, &lang_b, "concatenate");
		//lang_ab.pretty_print();
		lang_ab.contains_string("ab");
		//lang_ab.contains_string("b");
	}

	{
		eNFA_Language lang_a = eNFA_Language('a');
		eNFA_Language lang_b = eNFA_Language('b');
			
		eNFA_Language lang_ab = eNFA_Language(&lang_a, &lang_b, "union");
		//lang_ab.pretty_print();
		//lang_ab.contains_string("a");
		//lang_ab.contains_string("b");
	}

	{
		eNFA_Language lang_a = eNFA_Language('a');
		eNFA_Language lang_aa = eNFA_Language(&lang_a);
		//lang_aa.pretty_print();
		//lang_aa.contains_string("aaaa");
	}
*/
	LexaRegex l_01("a");
	l_01.match("a");
	l_01.match("b");

	LexaRegex l_02("abb");
	l_02.match("abb");
	l_02.match("bba");
	return 0;
}

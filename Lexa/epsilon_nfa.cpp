#include <set>
#include <iostream> 
#include <list> 
#include <string>

using namespace std;

class LexaRegex {

	
};

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
		list<eNFA_state *> accept;


	
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
			accept.push_back(acceptor);
		};

		eNFA_Language(eNFA_Language *a, eNFA_Language *b, string operation) {
			if(operation == "concatenate") {
				// take start state of "a", set as our start state. 
				start = a->start;

				// link together all finals of "a" to the start state 
				// of "b". 
				list<eNFA_state *>::iterator i;
				for(i = a->accept.begin(); i != a->accept.end(); i++) {
					(*i)->add_e_transit(b->start);
				}
			} else if(operation == "union") {
				
				// create a new state, add eps transitions
				// to each of the start states in the other langs. 
				start = new eNFA_state();
				start->add_e_transit(a->start);
				start->add_e_transit(b->start);
				
				// append accepter state lists, this is our list
				// of final states. 
				list<eNFA_state *>::iterator i;
				for(i = a->accept.begin(); i != a->accept.end(); i++) {
					accept.push_back(*i);
				}

				for(i = b->accept.begin(); i != b->accept.end(); i++) {
					accept.push_back(*i);
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
			list<eNFA_state *>::iterator i;

			for(i = a->accept.begin(); i != a->accept.end(); i++) {
				(*i)->add_e_transit(new_acceptor);
			}
		
			start->add_e_transit(new_acceptor);
			new_acceptor->add_e_transit(start);
			accept.push_back(new_acceptor);
		};
			
		void pretty_print(void) {
			start->pretty_print();
			cout << endl;
		};

		// test to see if a string is in this language. 
		bool contains_string(string s) {
			(void)s;
			// follow every possible path. 
			// don't care about efficiency in copy, 
			// just make it work. 

			// set of current states: 
			set<eNFA_state *> current;
			current.insert(start);

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
			bool incomplete = true;
			while(incomplete) {
				cout << "A";
				set<eNFA_state *> eps_close;
				set<eNFA_state *>::iterator i;
				for(i = current.begin(); i != current.end(); i++) {
					cout <<"B";
					list<eNFA_state *> *close = (*i)->eps_step();
					list<eNFA_state *>::iterator j;
					cout << "C";
					for(j = close->begin(); j != close->end(); j++) {
						eps_close.insert(*j);
					}
					cout <<"D";
				}
				incomplete = false;
				cout <<"E";
				for(i = eps_close.begin(); i != eps_close.end(); i++) {
					if(current.find(*i) == current.end()) {
						current.insert(*i);
						incomplete = true;
					}
				}
				cout<<"F";
			}

			// now, current is good. Now we can take a step on a char. 
			set<eNFA_state *>::iterator i;
			for(i = current.begin(); i != current.end(); i++) {
				(*i)->single_print();
			}
			return true;
		}
	
};	

int main(void) {
	
	{
		eNFA_Language lang_a = eNFA_Language('a');
		eNFA_Language lang_b = eNFA_Language('b');

		eNFA_Language lang_ab = eNFA_Language(&lang_a, &lang_b, "concatenate");
		lang_ab.pretty_print();
	}

	{
		eNFA_Language lang_a = eNFA_Language('a');
		eNFA_Language lang_b = eNFA_Language('b');
			
		eNFA_Language lang_ab = eNFA_Language(&lang_a, &lang_b, "union");
		lang_ab.pretty_print();
	}

	{
		eNFA_Language lang_a = eNFA_Language('a');
		eNFA_Language lang_aa = eNFA_Language(&lang_a);
	//	lang_aa.pretty_print();
		lang_aa.contains_string("string");
	}

	return 0;
}

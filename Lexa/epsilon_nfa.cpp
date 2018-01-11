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
		int add_transit(char c, eNFA_state *destination) {
			symbol_transits[c].push_back(destination);
		};

		int add_e_transit(eNFA_state *destination) {
			epsilon_transits.push_back(destination);
		};

		// "step" is to step on the char c in the 
		// automaton, for simulation. 
		list<eNFA_state *> *step(char c) {
			return &(symbol_transits[c]);
		};

		list<eNFA_state *> *eps_step() {
			return &epsilon_transits;
		};

		void pretty_print() {
			cout << "num eps_transits: " << epsilon_transits.size();

			for(char c = 0; c < 256; c++) {
				if(symbol_transits[c].size() > 0) {
					cout << c << symbol_transits[c].size() << endl;
				}
			}

			for(char c = 0; c < 256; c++) {
				if(symbol_transits[c].size() > 0) {
					list<eNFA_state *>::iterator i;
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
		}	

		// test to see if a string is in this language. 
		bool contains_string(string s);
	
		

};
int main(void) {

	return 0;
}

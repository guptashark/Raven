#include <iostream> 
#include <list> 

using namespace std;

class LexaRegex {

	
};

class eNFA_Language {

	// start state
	// set of final states

	// test to see if a string is in this language. 

	// functional style creation of Langs. 
	// Always produce a new eNFA_langauge obj, 
	// leave the two previous ones as is. Later
	// implementations may delete "consumed" langs. 
	// 	- create from single char. 
	//	- create from concat of two langs
	// 	- create from Union of two langs
	// 	- create from kleene of another lang. 

};

class eNFA_state {

	private: 
		// the char this state holds, ie to get here, 
		// you need to read this char in the input. 
		char c;

		// all the epsilon transitions
		list<eNFA_state *> epsilon_transits;

		// all the character transitions
		list<eNFA_state *> symbol_transits[256];

	public: 
		// constructor
		eNFA_state(char c): c(c) {};

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
};

int main(void) {

	return 0;
}

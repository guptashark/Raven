#include <iostream> 
#include <list> 

using namespace std;

class LexaRegex {

	
};

class eNFA_state {

	// the char this state holds, ie to get here, 
	// you need to read this char in the input. 
	char c;

	// all the epsilon transitions
	list<eNFA_state *> epsilon_transits;

	list<eNFA_state *> symbol_transits[256];

};

int main(void) {



	return 0;
}

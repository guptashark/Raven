#include <iostream>
#include <list> 
#include <string> 
#include <set> 

using namespace std;

// General scheme... 
// We need to determine first and follow sets... 
// We thus need an efficient way to store the rules and access them. 
// Once the first and follow sets are made, We can do actual parsing... 
// Just implement everything as you go. 

class Terminal;


class CfgSymbol {

	protected: 
		string name;

		// first set
		set<Terminal *> first_set;

		// follow set... maybe? 

		CfgSymbol(string name): name(name) {};

	public:
		string getName(void) {return name;};

};

typedef list<CfgSymbol *> *production;

class NonTerminal: public CfgSymbol {
	protected: 
		// inherit the following from CfgSymbol
		// string name
		// first_set
		
		// all productions in which this non-term is the head. 
		// the main cfg will add rules as it processes rules. 
	private: 
		list<production> all_productions;
	
	public:
	// Ctor
		NonTerminal(string name): CfgSymbol(name) {};


		void addProduction(production p) {
			all_productions.push_back(p);	
		}

		friend ostream &operator<<(ostream &os, NonTerminal &nt) {
			os << nt.name << ":" << endl;
			for(auto i = nt.all_productions.begin(); i != nt.all_productions.end(); i++) {
				os << "\t";
				for(auto j = (*i)->begin(); j != (*i)->end(); j++) {
					os << (*j)->getName() << " ";
				}
				os << endl;
			}
			os << endl;	// to have the extra space
			return os;
		}
};

class Terminal: public CfgSymbol {
		// inherit name, first set

		// don't have any productions... 
	public: 
		Terminal(string name): CfgSymbol(name) {};


};


int main(void) {

	cout << "Parser" << endl;
	NonTerminal A("A");
	NonTerminal B("B");
	NonTerminal C("C");
	
	Terminal a("a");
	Terminal b("b");
	Terminal c("c");

	list<CfgSymbol *> l = {&A, &B, &a, &b};


	//list<CfgSymbol *> p2 = {&B, &B, &A};
	
	C.addProduction(&l);

	cout << C;

	return 0;

}

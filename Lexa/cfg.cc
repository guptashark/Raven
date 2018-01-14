#include <iostream>
#include <list>
#include <set>
#include <string>


using namespace std;

class Production {

	private:
		string head;

		list<string> body;

	public: 
		Production(string head, list<string> body): head(head), body(body) {}

		void prettyPrint() {
			cout << head << " -> ";
			for(auto i = body.begin(); i != body.end(); i++) 
				cout << *i << " ";
			cout << endl;
		}
};


class ContextFreeGrammar {

	private: 
		set<string> non_terminals;
		set<string> terminals;

		list<Production> rules;

		// the next things are generated for each nonterminal 
		// first sets
		// follow sets. 

	public: 
		ContextFreeGrammar(set<string> non_terminals, set<string> terminals): 
		non_terminals(non_terminals), terminals(terminals) {};

		// is this efficient? 
		void addRule(string head, list<string> body) {
			rules.push_back(Production(head, body));
		}

		void prettyPrint() {
			for(auto i = rules.begin(); i != rules.end(); i++) {
				(*i).prettyPrint();
			}
		}
};

int main(void) {
	
	
	
	Production p("A", {"b", "*", "c"});
	p.prettyPrint();
}

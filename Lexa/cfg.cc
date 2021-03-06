#include <iostream>
#include <list>
#include <set>
#include <string>


using namespace std;

/* General Possible ideas and resources for implementing regex... 

	- CFG to implment the regex engine. 
	- 	prefixed regex. 
	-	ecma script regex. 
	- 	grep and egrep docs
	- 	
	- 	postfix regex
	- 

	General layout of Lexer: 
	-User defined matching on regex
	-Regex implemented using CFG parsing. 
	-Turn regex into eNFA
	-Turn eNFA into DFA
	-send lexer outputs to a "real" parser. 
*/

/* General parsing requirements to know: 

	-LL(k) parsing, and how it works
	-Transforming grammars to not be left recursive
	-LR parsing, and how it works
	-LALR parsing
	-SLR parsing
	-Speed and efficiency testing, average cases, etc.

	-Full CFG parsing
	-General parse tree transformations
	-Attachment to symbol tables. 
*/


/* First Sets: 
	
	* Need to calculate a "First" set, 
	which is effectively the set of terminals that
	can signal the start of a production of a particular
	non terminal. Ie, if we see a terminal, we'll be able
	to use the first set to determine which non-term we
	are deriving and such. 

	* If there is a production where terminal is first, 
	then add in that terminal to the set. 
	* If there is a non-terminal, then add the first set
	of that non-terminal to the set of this nonTerminal. 
	* If there is a non-terminal that has an epsilon production, 
	then add in the following symbol in the production. If it
	is a terminal, then we're done, if it is a non-terminal, 
	then proceed to add its first set. 



*/
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

class CFGNode {

	// both terminals and non terminals have names. 	
	string name;
};



class NonTerminalNode: CFGNode {
	private: 
		// inherit string name member var from CFGNode

		// the list of children
		list<CFGNode *> children;
	public: 
		// nothing yet. 
};


class TerminalNode: CFGNode {
	private: 
		// inherit string name member var from CFGNode
		Token *token;	// could be null, 
						// if this is LPAREN or something. 
	public: 
		// nothing yet... need a way to assign that string .
};


int main(void) {
	
	
	
	Production p("A", {"b", "*", "c"});
	p.prettyPrint();
}

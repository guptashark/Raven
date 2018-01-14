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


int main(void) {

	Production p("A", {"b", "*", "c"});
	p.prettyPrint();
}

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "lexa.h"
/* Requirements for this lexical analysis tool. 
 *
 * 1) User specification of tokens is important. (ie, regex config file).
 * 2) Can be used in Interpreter mode - output tokens on the go. 
 * 3) Can be used in compiler mode - output a file or internal buffer of tokens. 
 * 4) Specific to compilers only, 
 * 	not going to be in use for text editors or anything like that. 
 * 5) Can be set to start with several options in terms of token info.   
 */

/* 1) User spec of tokens. 
 * 	- it makes sense to have the Lex method: 
 * 		regex on left, output on match on the right. 
 * 	- keyword matching. 
 * 	- no arbitrary code on right, just token names. 
 * 	$ extension: store dfa compactly instead of generating from regex on startup each time. 
 */

/* 2) Interpreter Mode
 * 	- Split up the work: 
 * 		- some module in charge of reading user input or the file. 
 * 		- Lexa TODO need to figure out more here. 
 *
 */

/* 4) Compiler specific features
 * 	- need to be able to interact with a symbol table. We'll need another module for this. 
 */

/* 5) Implementable options: 
 * 	- pass line number with token
 * 	- auto remove comments
 * 	- auto remove whitespace
 */

/* Definitions:
 * 	Token: A pair consisting of a name and optional attribute value. 
 * 	Token name: symbol representing lexical unit. This is passed to parser.  
 * 	Pattern: The description of what lexemes can match a token name
 * 	Lexeme: The matching string in the source code 
 *
 */

/* Token Attributes. 
 * When more than one lexeme can match the token name, it's important to know the exact string matched, 
 * since it could mean different things. If NUMBER is matched, we *really* need to know what that number is. 
 * Otherwise, how are we going to do anything with data? This is where token attributes come in. 
 *
 * The attribute isn't generally useful to the parser, but rather used after the parse in the compiler backend. 
 *
 * Tokens in the dragon implementation have at most one attribute... but that attribute may be a pointer to a struct 
 * that has a lot of info. 
 *
 * Also, the most important one is when the token_name is ID because there is a lot of info that can exist wrt to it. 
 * Which brings us to the question... exactly how important is the symbol table? How does the lexer really use it? 
 * Or, is it that the lexer doesn't really use it, but rather updates it? 
 * 
 * When the lexer sees an ID, it can try to find the ID in the symbol table. If the symtab is already populated with 
 * keywords, then the lexer returns the keyword itself. (Ex, "for" "FUNCTION", "int", etc.)
 * otherwise, the lexer installs the newfound lexeme for the id in the table, creates attributes for it and returns the 
 * token to the Parser. 
 *
 * But does the lexer create symbol table entries? Like... if it saw "i" in one function, then another, would it not start
 * to overwrite things about "i"? ANd besides, the lexer has no idea about scope... Would it not make more sense to just not
 * create *any* symbol table entries and leave it all up to the parser? After all, it makes more sense for the lexer to say: 
 *
 * I found this ID and the lexeme is "hello" etc. 
 * But then the lexer would not be able to look up line numbers for identifiers since it wouldn't create a symbol table entry! 
 *
 * Possible solution: The lexer is set up with a pointer to various symbol tables (one for reserved words, etc) and one for its 
 * current scope. The compiler starts generating parse trees and learnign where the lexer is, and correspondingly changing the 
 * table that the lexer points to. Seems a little weird... 
 */

/* TODO:
 * Compiler feature: How would macro expansion work? Would it be implemented here, or somewhere else???
 * How specific do we go with tokens? Can we make a specification that the parser will never consider the lexeme? 
 * 	So you *must* only be interfacing with the symbol table and such? So we can't have a token called "comparison" ?? 
 * 	Thoughts: In the parser, it can just look at it and say this as a rule: CMP -> LT or GT or SLT or SGT or EQ or NEQ
 */

/* TODO:
 * Decide if we want to split lexer into 2: A scanner and lexer, (multiple input passes) or just one lexer.
 */


/* Implementation detail: 
 * When using the buffer pair method, how are we going to deal with things like strings that are longer than a single buffer length?
 *
 * Technically, it's a valid program construct (really long string), but it's going to ruin our lexer. What do we do? 
 *
 * Potential solutions: 
 * 	- Ignore and run the error that the lexeme to match is too long. Perhaps a language specification. 
 * 		- Clean solution, prevents most problems. (What string is longer than 4096 characters anyways!? 
 * 		- literally is longer than a whole page of text. We're not writing a text processor. So. 
 * 		- Disallow matching a lexeme longer than the buffer length. 
 * 	- Adaptive scanning - store the string as an indefinitely long object. 
 *
 */


/* We can safely split the process into two parts: Collecting input and running the dfa to match it. 
 * Lets implement the portion that reads the input. 
 */



/* Scanner specifications: 
 *
 * Buffer lengths of 4096. 
 * Be able to respond to calls for: 
 * 	getchar
 * 	putback
 * 	accept_lexeme
 *
 * Be implemented by sentinels. 
 * Recognize the end of the file. 
 * Recognize when the length of a lexeme is > bufflen. 
 *
 * Be configured to be able to read from stdin or a specified file. 
 * To do this... we need two particularly different implementations. 
 * If we're reading from a file, we already have all of our input. 
 *
 * We don't need to care about reading from stdin. That is a problem
 * for another time. Python does it well in that it puts the indents in for you. 
 * Not a good idea with what we're doing now. We just want tokenization to work. 
 * We can design a second front end later, so that our program can interface with a user. 
 * 
 * But right now, we just want interfacing with a file. 
 *
 */

struct scanner {

	char buff_A[SCANNER_BUFFSIZE + 1];
	char buff_B[SCANNER_BUFFSIZE + 1];

	char *lex_begin;
	char *lex_end;

	FILE *source;

	bool using_buff_A;
};

/* Leave the scanner on stack. It's size is constant. */  
/* may add other params as necessary. */
int 
scanner_ctor
(struct scanner *sp, 
 char *filename) {

	assert(sp != NULL);

	/* Zeroed buffers are nice, but they'll be overwritten anyways. */
	/* more importantly, set our sentinels to eof. */
	sp->buff_A[SCANNER_BUFFSIZE] = EOF;
	sp->buff_B[SCANNER_BUFFSIZE] = EOF;

	/* Is this the right way to set a pointer here... */
	/* TODO is this right... */
	sp->lex_begin = sp->buff_A;
	sp->lex_end = sp->buff_A;


	/* open up the file, with the checks. */
	FILE *source = fopen(filename, "r");
	sp->source = source;

	/* TODO address the following */
	/* Check that source isn't null. */
	/* Also check something with errno? */
	if(NULL == sp->source) {
		/* TODO address the floowing. */
		/* Need to send some kind of signal, 
		 * or set the scanner in a bad state. 
		 * But like, if sp->source is Null, it's 
		 * already unusable anyways. 
		 *
		 * For now, print the file does not exist.
		 */

		printf("The file doesn't exist!\n");
		return 1;
	}

	/* Load the data into buff_A */
	size_t num_chars_read = fread(	sp->buff_A, 
					sizeof(char), 
					SCANNER_BUFFSIZE, 
					sp->source);

	sp->using_buff_A = true;

	/* and we're done! (TODO fix return values */
	return 0;
}


int main(void) {

	printf("Lexical analysis software for the Raven compiler.\n");
	
	struct scanner s;
	int ret_val = scanner_ctor(&s, "lexa_test_01.txt");
	if(ret_val) {
		printf("OH NOOO!??\n");
	}

	return 0;
	

}

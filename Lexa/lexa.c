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

	char buff_A[SCANNER_BUFFSIZE];
	char buff_B[SCANNER_BUFFSIZE];

	int lex_end_index;
	int current_buffer_limit;
	char *lex_begin;
	char *lex_end;

	FILE *source;

	bool using_buff_A;
	bool on_final_read;
	bool scanning_terminated;

	/* A function pointer seems perfect */
	int (*scanner_getc)(struct scanner *, int *);
};


/* reload the scanner */
/* We make it impossible for a scanner to load an eof at the 
 * same spot as the sentinels. Since we're only loading 
 * SCANNER_BUFFSIZE bytes, there's just no way to fill that 
 * last spot with an eof 
 *
 * ALSO
 * We can check how many spots we've loaded. If it is any less 
 * than buffsize, then we know that we've reached the end of input. 
 * In which case, the next call to scanner_reload should do nothing, 
 * and a call to scanner_getc should return EOF. Alternatively, we can 
 * cause a system crash if someone asks for more characters after getting EOF. 
 */


/* Need to forward declare this. */
int scanner_getc(struct scanner *sp, int *c);

/* Need to forward declare this too. */
int scanner_getc_EOF(struct scanner *sp, int *c);

/* Leave the scanner on stack. It's size is constant. */  
/* may add other params as necessary. */


int 
scanner_ctor
(struct scanner *sp, 
 char *filename) {

	assert(sp != NULL);

	/* Zeroed buffers are nice, but they'll be overwritten anyways. */
	/* more importantly, set our sentinels to eof. */
	sp->buff_A[SCANNER_BUFFSIZE];
	sp->buff_B[SCANNER_BUFFSIZE];

	/* Is this the right way to set a pointer here... */
	/* TODO is this right... */
	sp->lex_end_index = 0;
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

	size_t chars_read = fread(
			sp->buff_A, 
			sizeof(char), 
			SCANNER_BUFFSIZE, 
			sp->source);

	printf("initial number of chars read: %d\n", chars_read);
	if(chars_read < SCANNER_BUFFSIZE && chars_read > 0) {
		sp->on_final_read = true;
		sp->scanning_terminated = false;
	} else if(chars_read == 0) {
		sp->on_final_read = true;
		sp->scanning_terminated = true;
	} else {
		sp->on_final_read = false;
		sp->scanning_terminated = false;
	}

	sp->current_buffer_limit = chars_read;
	/* proper settings for reload fn */
	sp->using_buff_A = true;
	sp->scanner_getc = scanner_getc;

	return 0;
}

int 
scanner_reload
(struct scanner *sp) {

	/* Select the new buffer */
	char *fresh_buff;
	if(sp->using_buff_A) {
		fresh_buff = sp->buff_B;
	} else { 
		fresh_buff = sp->buff_A;
	}

	sp->using_buff_A = !(sp->using_buff_A);

	size_t chars_read = fread(	fresh_buff,
					sizeof(char), 
					SCANNER_BUFFSIZE, 
					sp->source);

	/* TODO */
	/* We should probably check that we even loaded something in... */
	
	/* set the lex_end pointer to the start of the new buffer. */
	/* Update the lex_end_index to 0.*/ 

	//printf("The num_chars read are: %d\n", chars_read);
	if(chars_read < SCANNER_BUFFSIZE) { 
		sp->current_buffer_limit = chars_read;
		sp->on_final_read = true;
	}

	if(chars_read == 0) {
		printf("Read next buff, input turned out to be multiple of buff size.\n");
		sp->scanning_terminated = true;
		sp->scanner_getc = scanner_getc_EOF;
		// sort of a tricky situation - need to set another flag! 
		// can potentially swap functions so that on call, it rets
		// nothing but eof. 
	}

	sp->lex_end = fresh_buff;
	sp->lex_end_index = 0;

	return 0;
}
int
scanner_getc_EOF
(struct scanner *sp,
 int *c) {

	*c = EOF;
	return 0;
}

int
scanner_getc
(struct scanner *sp,
 int *c) {


/* Since we can't really check against eof... 
 * since our buffer is only characters... 
 * we'll have to do the check if we're at the buff end. 
 * The reloader sets the upper bound, and we count. 
 * Once we reach the upper bound, auto reload. If the 
 * bound indicates were' at the end of the file, then 
 * send out eofs. 
 */

	/* potentially do the type conversion to make it work 
	 * with a int *c instead of char *c so that we can send a 
	 * compliant EOF 
	 */

/* Small little if, just check if scanning is terminated. 
 * If it is, return after sending out an EOF. 
 * Otherwise, continue on. 
 */
	/* execution stops if we get here. */
	/*
	if(sp->scanning_terminated) {
		*c = EOF;
		return 0;
	}
	*/	
	char val = *(sp->lex_end);
	int c_val = val;
	*c = c_val;

	sp->lex_end++;
	sp->lex_end_index++;

	if(sp->lex_end_index == sp->current_buffer_limit) { 
		if(sp->on_final_read) {
			// do something for final/close procedurse. 
			// probably just send eof. 
			sp->scanning_terminated = true;
			sp->scanner_getc = scanner_getc_EOF;
		} else {
			// buffer reload. 
			scanner_reload(sp);
		}
	} 

	return 0;
/*
	if(*(sp->lex_end) == EOF) {
		if(sp->lex_end_index == SCANNER_BUFFSIZE) {
			printf("SAYS END OF BUFFER\n");
			// is this proper? TODO 
			scanner_reload(sp);
			scanner_getc(sp, c);
		} else {
			// we're at the end of the input. 
			// TODO set scanner flags 
			printf("WE're AT THE END OF THE INPPUTT! \n");
			printf("We're at the end of input! \n");
			*c = EOF;
		}

		return 0;

	} else {
		*c = *(sp->lex_end);
		sp->lex_end++;
		sp->lex_end_index++;
		printf("JUST READ: %02x = %c\n", *c, *c);
		printf("CURRENT INDEX VALUE: %d\n", sp->lex_end_index);
		return 0;
	}

*/
}

int main(void) {

	printf("Lexical analysis software for the Raven compiler.\n");
	
	struct scanner s;
	int ret_val = scanner_ctor(&s, "lexa_test_03.txt");

	// the "char" we're reading into. 
	
	int c = 1;

	while(c != EOF) {
		s.scanner_getc(&s, &c);

//		printf("%x\n", c);
	}

	return 0;
}

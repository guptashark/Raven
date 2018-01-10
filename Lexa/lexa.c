#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "lexa.h"

/*
 * TODO
 *
 * Proper feof and ferror checking in case fread
 * reads less than (var count) bytes. 
 *
 * Close the file on input termination. 
 *
 * implement ungetc. 
 */
struct scanner {

	char buff_A[SCANNER_BUFFSIZE];
	char buff_B[SCANNER_BUFFSIZE];

	int lex_end_index;
	int lex_begin_index;
	int current_buffer_limit;
	char *lex_begin;
	char *lex_end;

	FILE *source;

	bool using_buff_A;
	bool lex_begin_on_A;
	bool on_final_read;

	bool next_buff_already_loaded;
	bool was_on_final_read;

	/* A function pointer seems perfect */
	int (*scanner_getc)(struct scanner *, int *);
};

/* Need to forward declare this. */
int scanner_getc(struct scanner *sp, int *c);

/* Need to forward declare this too. */
int scanner_getc_EOF(struct scanner *sp, int *c);

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
	sp->lex_begin_index = 0;

	sp->lex_begin_on_A = true;

	sp->lex_begin = sp->buff_A;
	sp->lex_end = sp->buff_A;

	sp->scanner_getc = scanner_getc;
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

	if(chars_read < SCANNER_BUFFSIZE && chars_read > 0) {
		sp->on_final_read = true;
	} else if(chars_read == 0) {
		sp->on_final_read = true;
		sp->scanner_getc = scanner_getc_EOF;
		fclose(sp->source);
	} else {
		sp->on_final_read = false;
	}

	sp->current_buffer_limit = chars_read;
	/* proper settings for reload fn */
	sp->using_buff_A = true;
	sp->next_buff_already_loaded = false;
	sp->was_on_final_read = false;

	return 0;
}

int scanner_ungetc(struct scanner *sp) {

	/* if lex_begin == lex_end then nothing to do. */
	if(sp->lex_begin == sp->lex_end) {
		// TODO Return something other than 0... 
		// and set an errno or something. 
		return 0;
	} 
	
	if(sp->lex_end_index > 0) {
		sp->lex_end_index--;
		sp->lex_end--;
		return 0;
	}

	if(sp->lex_end_index == 0) {
		sp->lex_end_index = SCANNER_BUFFSIZE - 1;
		// which buffer are we on? 
		if(sp->using_buff_A) {
			sp->lex_end = &(sp->buff_B[SCANNER_BUFFSIZE - 1]);
		} else {
			sp->lex_end = &(sp->buff_A[SCANNER_BUFFSIZE - 1]);
		}

		sp->using_buff_A = !(sp->using_buff_A);
		sp->next_buff_already_loaded = true;

		if(sp->on_final_read) {
			sp->was_on_final_read = true;
			sp->on_final_read = false;
		}

		return 0;
	}


	// we should never really get here... 
	// TODO shouold this be here? 
	return 0;
}

int scanner_reload(struct scanner *sp) {

	/* Select the new buffer */
	char *fresh_buff;
	if(sp->using_buff_A) {
		fresh_buff = sp->buff_B;
	} else { 
		fresh_buff = sp->buff_A;
	}

	sp->using_buff_A = !(sp->using_buff_A);

	if(!(sp->next_buff_already_loaded)) {
	
		size_t chars_read = fread(	
					fresh_buff,
					sizeof(char), 
					SCANNER_BUFFSIZE, 
					sp->source);

		/* TODO */
		/* We should probably check that we even loaded something in... */
	
		/* set the lex_end pointer to the start of the new buffer. */
		/* Update the lex_end_index to 0.*/ 

		if(chars_read < SCANNER_BUFFSIZE) { 
			sp->current_buffer_limit = chars_read;
			sp->on_final_read = true;
		}

		if(chars_read == 0) {
			sp->scanner_getc = scanner_getc_EOF;
			fclose(sp->source);
		}
	}

	// reset it for the next time we call reload. 
	if(sp->next_buff_already_loaded) {
		sp->next_buff_already_loaded = false;
	}

	sp->lex_end = fresh_buff;
	sp->lex_end_index = 0;

	return 0;
}

int scanner_getc_EOF(struct scanner *sp, int *c) {
	*c = EOF;
	return 0;
}

int scanner_getc(struct scanner *sp, int *c) {
	
	char val = *(sp->lex_end);
	int c_val = val;
	*c = c_val;

	sp->lex_end++;
	sp->lex_end_index++;

	if(sp->lex_end_index == sp->current_buffer_limit) { 
		if(sp->on_final_read) {
			sp->scanner_getc = scanner_getc_EOF;
			fclose(sp->source);
		} else {
			scanner_reload(sp);
		}
	} 

	return 0;
}

int scanner_pretty_print(struct scanner *sp) {

	// Print buffers
	// Print location of pointers. 
	// print flags. 
	
	int mod_lex_end_index = sp->lex_end_index * 2 + 1;
	int mod_lex_begin_index = sp->lex_begin_index * 2 + 1; 

	// print buffer A: 
	
	// print where the B pointer is. 
	if(sp->lex_begin_on_A) {
		for(int i = 0; i < mod_lex_begin_index; i++) {
			printf(" ");
		}

		printf("B");
	} 

	printf("\n");

	// print where the E pointer is. 
	if(sp->using_buff_A) {
		for(int i = 0; i < mod_lex_end_index; i++) {
			printf(" ");
		}

		printf("E");
	} 

	printf("\n");

	// Print the topline of the buffer: -----------
	for(int i = 0; i < SCANNER_BUFFSIZE * 2 + 1; i++) {
		printf("-");

	}
	printf("\n");

	// print the values of the buffer: |a|b|c|d|
	for(int i = 0 ; i < SCANNER_BUFFSIZE; i++) {
		printf("|%c", sp->buff_A[i]);
	}
	printf("|\n");

	// print the bottom line of the buffer: --------
	for(int i = 0; i < SCANNER_BUFFSIZE * 2 + 1; i++) {
		printf("-");

	}
	// extra newline for buffer B. 
	printf("\n\n");

	// Now do everything all over again to print buffer B. 

	// print where the B pointer is. 
	if(!(sp->lex_begin_on_A)) {
		for(int i = 0; i < mod_lex_begin_index; i++) {
			printf(" ");
		}

		printf("B");
	} 

	printf("\n");

	// print where the E pointer is. 
	if(!(sp->using_buff_A)) {
		for(int i = 0; i < mod_lex_end_index; i++) {
			printf(" ");
		}

		printf("E");
	} 

	printf("\n");

	// Print the topline of the buffer: -----------
	for(int i = 0; i < SCANNER_BUFFSIZE * 2 + 1; i++) {
		printf("-");

	}
	printf("\n");

	// print the values of the buffer: |a|b|c|d|
	for(int i = 0 ; i < SCANNER_BUFFSIZE; i++) {
		printf("|%c", sp->buff_B[i]);
	}
	printf("|\n");

	// print the bottom line of the buffer: --------
	for(int i = 0; i < SCANNER_BUFFSIZE * 2 + 1; i++) {
		printf("-");

	}
	printf("\n");
	return 0;
}

int main(void) {

	printf("Lexical analysis software for the Raven compiler.\n");
	struct scanner s;
	int ret_val = scanner_ctor(&s, "lexa_test_02.txt");

	char c = 1;
	int i = 0;

	int c2 = 1; 
	scanner_pretty_print(&s);
	while(true) {

		scanf("%c", &c);
		if(c == 'g') {
			s.scanner_getc(&s, &c2);
			scanner_pretty_print(&s);

		} else if(c == 'u') {
			scanner_ungetc(&s);
			scanner_pretty_print(&s);
		}
	}

	printf("Done: %d\n", i);

	return 0;
}

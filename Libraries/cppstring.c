#include <string.h>

struct string;

typedef struct string *String;

struct string {
	int size;
	int capacity;
	char *data;

};

String
string_ctor_empty(void) {

	String ret = malloc(sizeof(struct string));

	ret->size = 0;
	ret->capacity = 16;
	ret->data = malloc(sizeof(char) * ret->capacity);

	return ret;
}

String
string_ctor_copy(String *str) {

	String ret = malloc(sizeof(struct string));

	ret->size = str->size;
	ret->capacity = str->capacity;
	
	ret->data = malloc(sizeof(char) * ret->capacity);

	for(int i = 0; i < ret->size; i++) {
		ret->data[i] = str->data[i];
	}

	return ret;
}

// TODO Proper error handling... 
// right now, we assume that len 
// is the length of the substrng, 
// whereas we may be getting -1, 
// ie, to the end of the array. 
String
string_ctor_substr(String str, int n_pos, int len) {
	String ret = malloc(sizeof(struct string));

	ret->size = len;
	ret->capacity = len + 16;
	ret->data = malloc(sizeof(char) * ret->capacity);

	for(int i = 0; i < len; i++) {
		ret->data[i] = str->data[i + n_pos];
	}

	return ret;
}
// TODO
// How to make this nicer? In the way of 
// ensuring it is a good allocation? 
String
string_ctor_literal(char *s) {

	int len = strlen(s);
	String ret = malloc(sizeof(struct string));

	ret->size = len;
	ret->capacity = len + 16;

	for(int i = 0; i < ret->size; i++) {
		ret->data[i] = s[i];
	}

	return ret;
}



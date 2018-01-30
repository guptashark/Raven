
struct string;
typedef struct string *String;

String 
string_ctor_empty(void);

String
string_ctor_copy(String)

String
string_ctor_substr(String, int n_pos, int len);

String
string_ctor_literal(char *s);

String
string_ctor_buff(char *b, int n);

String
string_ctor_fill(int n, char c);


// Assignment operations
void
string_assign_literal(String dest, char *s);

void
string_assign_str(String dest, String source);

void
string_assign_char(String dest, char c);

// element access
char
string_at(String s, int pos);

// capacity options
int 
string_size(String);

int
string_length(String);

int
string_capacity(String);

// String operations
int
string_cmp_str(String s1, String s2);

// String relational operators;
bool 
string_eq(String s1, String s2);

// TODO Implement
bool 
string_neq(String s1, String s2);

// TODO Implement
bool
string_lt(String s1, String s2);

// TODO implement
bool
string_leq(String s1, String s2);

// TODO Implement
bool
string_gt(String s1, String s2);

// TODO implement
bool
string_geq(String s1, String s2);




typedef enum {
	IF, THEN, ELSE, ADD, SUB, NUM, ID, MUL, DIV, BEGIN, END, DO, WHILE, SEMICOLON, RPAR, LPAR
} TOKEN_CLASS;

typedef struct TOKEN{
	TOKEN_CLASS type;
	union{
		char *name;
		int val;
	}attribute;
} TOKEN;

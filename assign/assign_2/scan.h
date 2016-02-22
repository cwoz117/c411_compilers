


typedef enum{
	ENDFILE, ERROR,
	ADD, SUB, MUL, DIV, 
	LPAR, RPAR, SEMICOLON, 
	NUM, VAR, ASSIGN,
	BEGIN, END, 
	WHILE, DO, 
	IF, THEN, ELSE,
	PRINT, WRITE
}TOKEN_CLASS;

typedef struct TOKEN{
	TOKEN_CLASS type;
	union{
		char name[128];
		int val;
	}attribute;
} TOKEN;
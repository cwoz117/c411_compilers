#ifndef SCAN_HEAD
#define SCAN_HEAD


typedef enum{
	ENDFILE, ERROR,
	ADD, SUB, MUL, DIV, 
	LPAR, RPAR, SEMICOLON, 
	NUM, VAR, ASSIGN,
	BEGIN, END, 
	WHILE, DO, 
	IF, THEN, ELSE,
	PRINT, READ
}TOKEN_CLASS;

typedef struct TOKEN{
	TOKEN_CLASS type;
	union{
		char name[128];
		int val;
	}attribute;
} TOKEN;

#endif

void printToken(TOKEN);
TOKEN getToken(void);
int load_source(char * source_file);
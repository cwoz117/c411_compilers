#ifndef GLOBAL
#define GLOBAL
#include "scan.h"
#endif

typedef enum {
	STMT, EXP
}NODE_KIND;

typedef enum {
	N_IF, N_WHILE, N_ASSIGN, N_READ, N_PRINT
}STMT_TYPE;

typedef enum{
	N_OP, N_CONST, N_VAR
}EXP_TYPE;

typedef struct NODE{
	struct NODE * child[3];
	struct NODE * sibling;
	int line_number;
	NODE_KIND kind;
	union{
		STMT_TYPE stmt;
		EXP_TYPE exp;
	}type;
	union{
		TOKEN_CLASS token_class;
		int val;
		char * name;
	}attribute;
}NODE;


NODE parse(void);
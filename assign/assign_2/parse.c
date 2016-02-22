/*==============================================================================








 *============================================================================*/
#include "parse.h"
#include "scan.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static TOKEN token;
static int lineno;

static NODE * stmt_list(void);
static NODE * statement(void);
static NODE * if_stmt(void);
static NODE * while_stmt(void);
static NODE * assign(void);
static NODE * read(void);
static NODE * print(void);
static NODE * exp(void);
static NODE * exp_helper(void);
static NODE * term(void);
static NODE * factor(void);

static NODE * new_stmt(STMT_TYPE type){
	NODE * t = malloc (sizeof(NODE));
	if (t == NULL){
		printf("Could not create new Node\n");
	} else {
		for (int i = 0; i < 3; i++){
			t->child[i] = NULL;
		}
		t->sibling = NULL;
		t->line_number = lineno;
		t->kind = STMT;
		t->type.stmt = type;
	}
	return t;
}

static NODE new_exp(EXP_TYPE type){
	NODE * t = malloc(sizeof(NODE));
	if (t == NULL){
		printf("Could not create new node\n");
	} else {
		for (int i = 0; i < 3; i++){
			t->child[i] = NULL;
		}
		t->sibling = NULL;
		t->line_number = lineno;
		t->kind = EXP;
		t->type.exp = type;
	}
	return t;
}
static char * toHeap(char *s){
	char *t;
	if (s == NULL)
		return NULL;
	int length = strlen(s)+1;
	t = malloc(length);
	if (t == NULL){
		printf("Memory error copying string\n");
	}else {
		strcpy(t, s);
	}
	return t;
}

static void syntax_error(char * message){
	printf("Error on line %d: %s\n", lineno, message);
}

static void match(TOKEN_CLASS expected){
	if (token.type == expected)
		token = getToken();
	else {
		syntax_error("Unexpected Token ->");
		printToken(token);
	}
}

//TODO epsilon mistake (?)
static NODE * stmt_list(void){
	match(BEGIN);
	NODE * t = statement();
	NODE * p = t;
	while ( (token.type != ENDFILE) && (token.type != END) &&
					   (token.type != ELSE)) {
		NODE * q;
		match(SEMICOLON);
		q = statement();
		if (q != NULL){
			if (t == NULL){
				t = q;
				p = q;
			} else {
				p->sibling = q;
				p = q;
			}
		}
	}
	match(END);
	return t;
}

static NODE * statement(void){
	NODE *t = NULL;
	switch (token.type){
	case IF:
		t = if_stmt();
		break;
	case WHILE:
		t = while_stmt();
		break;
	case READ:
		t = read();
		break;
	case VAR:
		t = assign();
		break;
	case PRINT:
		t = print();
		break;
	case BEGIN:
		t = stmt_list();
		break;
	default:
		syntax_error("Unexpected token ->");
		token = getToken();
		break;
	}
	return t;
}

static NODE * if_stmt(void){
	NODE * t = new_stmt(N_IF);
	match(IF);
	if (t != NULL){
		t->child[0] = exp();
	}
	match(THEN);
	if (t != NULL){
		t->child[1] = stmt_list();
	}
	if (token.type == ELSE){
		match(ELSE);
		if (t!= NULL){
			t->child[2] = stmt_list();
		}
	}
	return t;
}

static NODE * while_stmt(void){
	NODE * t = new_stmt(N_WHILE);
	match(WHILE);
	if(t != NULL){
		t->child[0] = exp();
	}
	match(DO);
	if(t != NULL) {
		t->child[1] = stmt_list();
	}
	return t;
}

static NODE * assign_stmt(void){
	NODE * t = new_stmt(N_ASSIGN);
	if ((t != NULL) && (token.type == VAR)){
		t->attribute.name = toHeap(token.attribute.name);
	}
	match(VAR);
	match(ASSIGN);
	if (t != NULL){
		t->child[0] = exp();
	}
	return t;
}

static NODE * read(void){
	NODE * t = new_stmt(N_READ);
	match(READ);
	if ((t != NULL) &&(token.type == VAR)){
		t->attribute.name = toHeap(token.attribute.name);
	}
	match(VAR);
	return t;
}

static NODE * print(void){
	NODE * t = new_stmt(N_PRINT);
	match(PRINT);
	if (t != NULL)
		t->child[0] = exp();
	return t;
}

//TODO PRobably a problem here
static NODE * exp(void){
	NODE * t = new_exp(N_OP);
	if (t != NULL){
		t->child[0] = exp_helper();
		t->attribute.token_class = token.type;
	}
	match(token.type); // ADD or SUB
	if (t != NULL){
		t->child[1] = exp_helper();
	}
}

static NODE *exp_helper(void){
	NODE * t = term();
	while (token.type == ADD || token.type == SUB){
		NODE * temp = new_exp(N_OP);
		if (temp != NULL){
			temp->child[0] = t;
			temp->attribute.token_class = token.type;
			t = temp;
			match(token.type);
			p->child[1] = term();
		}
	}
	return t;
}

static NODE * term(void){
	NODE * t = factor();
	while (token.type == MUL || token.type == DIV){
		NODE * temp = new_exp(N_OP);
		if (temp != NULL){
			temp->child[0] = t;
			temp->attribute.token_class = token.type;
			t = temp;
			match(token.type);
			temp->child[1] = factor();
		}
	}
	return t;
}

static NODE * factor(void){
	NODE * t = NULL;
	switch(token.type){
	case NUM:
		t = new_exp(N_CONST);
		if ((t != NULL) && (token.type == NUM)){
			t->attribute.val = token.attribute.val;
		}
		match(NUM);
		break;
	case VAR:
		t = new_exp(N_VAR);
		if ((t != NULL) && (token.type == VAR)){
			t->attribute.val = toHeap(token.attribute.name);
		}
		match(VAR);
		break;
	case LPAR:
		match(LPAR);
		t = exp();
		match(RPAR);
		break;
	default:
		syntax_error("Unexpected Token -> ");
		token = getToken();
		break;
	}
	return t;
}

Node * parse(void){
	Node * t;
	token = getToken();
	t = stmt_list();
	if (token != ENDFILE){
		syntax_error("still code left in the file\n");
	}
}

int main(int argc, char * argv[]){
	int i = load_source(argv[1]);
	if (i){
		NODE * ast = parse();
	}
}
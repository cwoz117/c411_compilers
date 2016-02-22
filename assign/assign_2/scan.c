#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef GLOBALS
#define GLOBALS
#include "scan.h"
#endif

#define WHITESPACE  ((c == ' ') || (c == '\t') || (c == '\n'))

static char *file = NULL;
static int ptr = 0;
static int init_ptr;

typedef enum {
	D_START, D_DONE, 
	D_NUM, D_VAR, D_ASSIGN, 
	D_BLOCK_COMMENT, D_INLINE_COMMENT,
	D_IF, D_THEN, D_ELSE,
	D_BEGIN, D_END,
	D_DO, D_WHILE,
	D_PRINT, D_READ
}DFA_STATE;

void printToken(TOKEN t){
	char str[16];
	switch (t.type){
		case BEGIN:
			memcpy(&str[0], "BEGIN\0", 6);
			break;
		case END:
			memcpy(&str[0], "END\0", 4);
			break;
		case SEMICOLON:
			memcpy(&str[0], "SEMICOLON\0", 10);
			break;
		case ASSIGN:
			memcpy(&str[0], "ASSIGN\0", 7);
			break;
		case VAR:
			memcpy(&str[0], "VAR\0", 4);
			break;
		case NUM:
			memcpy(&str[0], "NUM\0", 4);
			break;
		case ERROR:
			memcpy(&str[0], "ERROR\0", 6);
			break;
		case PRINT:
			memcpy(&str[0], "PRINT\0", 6);
			break;
		case READ:
			memcpy(&str[0], "READ\0", 5);
			break;
		case IF:
			memcpy(&str[0], "IF\0", 3);
			break;
		case THEN:
			memcpy(&str[0], "THEN\0", 5);
			break;
		case ELSE:
			memcpy(&str[0], "ELSE\0", 5);
			break;
		case DO:
			memcpy(&str[0], "DO\0", 3);
			break;
		case WHILE:
			memcpy(&str[0], "WHILE\0", 6);
			break;
		default:
			memcpy(&str[0], "OPERATOR\0", 9);
	}
	printf("%s\n", str);
}

static TOKEN setToken(char c){
	TOKEN tok;
	switch (c){
		case '+':
			tok.type = ADD;
			break;
		case '-':
			tok.type = SUB;
			break;
		case '*':
			tok.type = MUL;
			break;
		case '/':
			tok.type = DIV;
			break;
		case '(':
			tok.type = LPAR;
			break;
		case ')':
			tok.type = RPAR;
			break;
		case ';':
			tok.type = SEMICOLON;
			break;
		default:
			tok.type = ERROR;
			break;
		}
	return tok;
}

TOKEN getToken(){
	TOKEN tok;
	DFA_STATE dfa = D_START;
	while (dfa != D_DONE){
		char c = file[ptr];
		switch (dfa){
			case D_START:
				if (isdigit(c)){
					dfa = D_NUM;
					init_ptr = ptr;
				} else if (WHITESPACE){
					// Do nothing
				} else if (c == ':'){
					dfa = D_ASSIGN;
				} else if (c == '%'){
					dfa = D_INLINE_COMMENT;
				}else if (c == '/'){
					if (file[ptr+1] == '*'){
							dfa = D_BLOCK_COMMENT;
						}
				}else if (isalpha(c)){
					switch(c){
					case 'b':
					case 'B':		
						dfa = D_BEGIN;
						break;
					case 'e':
					case 'E':
						if (file[ptr+1] == 'N' ||
						    file[ptr+1] == 'n')
							dfa = D_END;
						else
							dfa = D_ELSE;
						break;
					case 'I':
					case 'i': //TODO var error
						dfa = D_IF;
						break;
					case 'T':
					case 't': //TODO var error.
						dfa = D_THEN;
						break;
					case 'W':
					case 'w': //TODO variable error.
						dfa = D_WHILE;
						break;
					case 'D':
					case 'd': //TODO variable error.
						dfa = D_DO;
						break;
					case 'p':
					case 'P':
						dfa = D_PRINT;
						break;
					case 'r':
					case 'R':
						dfa = D_READ;
						break;
					default:
						dfa = D_VAR;
						init_ptr = ptr;
						break;
					}
				
				} else {
					dfa = D_DONE;
					tok = set_token(c);
				}
				break;
			case D_NUM:
				if (!isdigit(c)){
					ptr -=1;
					dfa = D_DONE;
					tok.type = NUM;
					char buf[ptr - init_ptr];
					memcpy(&buf[0], &file[init_ptr], 
					      (ptr - init_ptr));
					tok.attribute.val = atoi(&buf[0]);
				}
				break;
			case D_VAR:
				if (!isalpha(c)){
					ptr -=1;
					dfa = D_DONE;
					tok.type = VAR;
					memcpy(tok.attribute.name, &file[init_ptr], 
					      (ptr - init_ptr));
				}
				break;
			case D_ASSIGN:	//Put in set_token
				dfa = D_DONE;
				if (c == '='){
					tok.type = ASSIGN;
				} else {
					ptr -= 1;
					tok.type = ERROR;
				}
				break;
			case D_IF:	//put in set_token
				dfa = D_DONE;
				if (c == 'f'){
					tok.type = IF;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_THEN:	//put in set_token
				dfa = D_DONE;
				if (c == 'h'){
					tok.type = THEN;
					ptr += 2;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_ELSE:	//put in set_token
				dfa = D_DONE;
				if (c == 'l'){
					tok.type = ELSE;
					ptr += 2;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_WHILE:
				dfa = D_DONE;
				if (c == 'h') {
					tok.type = WHILE;
					ptr += 3;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_DO:
				dfa = D_DONE;
				if (c == 'o') {
					tok.type = DO;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_BEGIN:
				dfa = D_DONE;
				if (c == 'e'){
					tok.type = BEGIN;
					ptr += 3;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_END:
				dfa = D_DONE;
				if (c == 'n'){
					tok.type = END;
					ptr += 1;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_BLOCK_COMMENT:
				dfa = D_BLOCK_COMMENT;
				if (c == '*'){
					if (file[ptr+1] == '/'){
						dfa = D_START;
						ptr += 1;
					}
				}
				break;
			case D_INLINE_COMMENT:
				if (c == '\n')
					dfa = D_START;
				break;
			case D_PRINT:
				dfa = D_DONE;
				if (c == 'r' || c== 'R') {
					tok.type = PRINT;
					ptr += 3;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_READ:
				dfa = D_DONE;
				if (c == 'e' || c== 'E') {
					tok.type = READ;
					ptr += 2;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			default:
				dfa = D_DONE;
				tok.type = ERROR;
				break;
		}
		ptr += 1;
	}
	return tok;
}

int load_source(char *name){
	FILE *fd = fopen(name, "r");
	if (fd != NULL){
		// Determine the buffer size requirement
		if (fseek(fd, 0L, SEEK_END) == 0){
			long buffer_size = ftell(fd);
			if (buffer_size == -1)
				return 0;
			file = malloc(sizeof(char) * (buffer_size+1));
			if (file == NULL)
				return 0;
			if (fseek(fd, 0L, SEEK_SET) != 0)
				return 0;
			// Load the file
			int count = fread(file, sizeof(char), buffer_size, fd);
			if (count == 0)
				return 0;
			else
				file[buffer_size] = '\0';
			fclose(fd);
		}
		return 1;
	} else {
		return 0;
	}
}

int delete(){
	free(file);
}
/*
int main(int argc, char *argv[]){
	int test = load_source(argv[1]); // OK
	if (test == 0)
		exit(0);
	TOKEN t = getToken();		// MAYBE OK.
	while (t.type != ENDFILE){
		printToken(t);
		t = getToken();
	}
	delete();
} */
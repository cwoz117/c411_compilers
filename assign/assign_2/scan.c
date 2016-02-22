#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
}DFA_STATE;

static int isNumber(char a){
	
}
static int isAlpha(char a){
	
}
static TOKEN set_token(char c){
	TOKEN tok;
	switch (c){
		case EOF:
			tok.type = ENDFILE;
			break;
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
				if (isNumber(c)){
					dfa = D_NUM;
					init_ptr = ptr;
				} else if (c == ':'){
					dfa = D_ASSIGN;
				} else if (WHITESPACE){
					// Do nothing
				} else if (c == '/'){
					if (file[ptr+1] == '*')
						dfa = D_BLOCK_COMMENT;
				} else if (c == '%'){
					dfa = D_INLINE_COMMENT;
				} else if (c == 'B') {
					dfa = D_BEGIN;
				} else if (c == 'E') {
					if (file[ptr+1] == 'N')
						dfa = D_END;
					else if (file[ptr+1] == 'L')
						dfa = D_ELSE;
				} else if (c == 'i'){
					dfa = D_IF;
				} else if (c == 't'){
					dfa = D_THEN;
				} else if (c == 'w') {
					dfa = D_WHILE;
				} else if (c == 'd'){
					dfa = D_DO;
				} else if (isAlpha(c)){
					dfa = D_VAR;
					init_ptr = ptr;
				} else {
					dfa = D_DONE;
					tok = set_token(c);
				}
				break;
			case D_NUM:
				if (!isNumber(c)){
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
				if (!isAlpha(c)){
					ptr -=1;
					dfa = D_DONE;
					tok.type = VAR;
					memcpy(tok.attribute.name, &file[init_ptr], 
					      (ptr - init_ptr));
				}
				break;
			case D_ASSIGN:
				dfa = D_DONE;
				if (c == '='){
					tok.type = ASSIGN;
				} else {
					ptr -= 1;
					tok.type = ERROR;
				}
				break;
			case D_IF:
				dfa = D_DONE;
				if (c == 'f'){
					tok.type = IF;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_THEN:
				dfa = D_DONE;
				if (c == 'h'){
					tok.type = THEN;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_ELSE:
				dfa = D_DONE;
				if (c == 'l'){
					tok.type = ELSE;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_WHILE:
				dfa = D_DONE;
				if (c == 'h') {
					tok.type = WHILE;
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
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_END:
				dfa = D_DONE;
				if (c == 'n'){
					tok.type = END;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case D_BLOCK_COMMENT:
				if (c == '*')
					if (file[ptr+1] == '/')
						dfa = D_START;
				break;
			case D_INLINE_COMMENT:
				if (c == '\n')
					dfa = D_START;
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
			if (buffer_size = -1)
				return 0;
			file = malloc(sizeof(char) * (buffer_size+1));
			if (fseek(fd, 0L, SEEK_SET) != 0)
				return 0;
			
			// Load the file
			int count = fread(file, sizeof(char), buffer_size, fd);
			if (count == 0)
				return 0;
			else
				memcpy(&file[count++], '\0', sizeof(char));
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